#include "acfsi.h"
#include "acutmacro.h"
#include "acfsmheader.h"
#include "acfscmprheader.h"
#include "acfsmbheader.h"
#include "lzo/lzoconf.h"
#include "lzo/lzo1y.h"
#include "acfsfstream.h"
#include "cstrop.h"


AcFsI::AcFsI(AcFsHeap* pHeap)
	: m_pFsHeap(pHeap)
	, m_pMHeader(NULL)
	, m_pCallback(NULL)
{
	m_DefStream.m_pFsHeap = m_pFsHeap;
	m_ErrorTempBuff.m_pFsHeap = m_pFsHeap;
	m_OrgFileBuff.m_pFsHeap = m_pFsHeap;
	m_BuffUnk512.m_pFsHeap = m_pFsHeap;
	m_BuffUnk536.m_pFsHeap = m_pFsHeap;

	Reset();
	InitializeCriticalSection(&m_cs);
}

AcFsI::~AcFsI()
{
	DeleteMemory();
	DeleteCriticalSection(&m_cs);
}

int AcFsI::CreateFile(const char*, unsigned int, unsigned int, unsigned int, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::CreateFile(const wchar_t*, unsigned int, unsigned int, unsigned int, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::OpenUsing(HANDLE hFile, unsigned int desiredAccess)
{
	if (NULL == hFile)
		return ERROR_INVALID_PARAMETER;

	return iCreateFile(NULL, desiredAccess, 0, 0, 0, false, hFile, 0);
}

int AcFsI::OpenUsing(AcStream*, unsigned int)
{
	return ERROR_SUCCESS;
}

Adesk::Boolean AcFsI::isOpen(void)
{
	if (NULL == m_pMHeader)
		return Adesk::kFalse;

	if (NULL == m_pMHeader->getFileHandle())
		return Adesk::kFalse;

	return Adesk::kTrue;
}

int AcFsI::SetFileTime(const FILETIME*, const FILETIME*, const FILETIME*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetFileTime(FILETIME* lpCreationTime, FILETIME* lpLastAccessTime, FILETIME* lpLastWriteTime)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;

	if (!::GetFileTime(m_pMHeader->getFileHandle(), lpCreationTime, lpLastAccessTime, lpLastWriteTime))
		return GetLastError();

	return ERROR_SUCCESS;
}

int AcFsI::GetFileInformationByHandle(BY_HANDLE_FILE_INFORMATION* pInfo)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;
	if (NULL == pInfo)
		return ERROR_INVALID_PARAMETER;

	if (!::GetFileInformationByHandle(m_pMHeader->getFileHandle(), pInfo))
		return GetLastError();

	return ERROR_SUCCESS;
}

int AcFsI::CloseFile(void)
{
	lock();

	int ret1 = iFlush(true);
	int ret2 = m_pMHeader->CloseFile();
	DeleteMemory();
	if (ERROR_SUCCESS == ret1)
		ret1 = ret2;

	unlock();
	return ret1;
}

int AcFsI::Flush(void)
{
	lock();
	int ret = iFlush(false);
	unlock();
	return ret;
}

int AcFsI::WriteFile(const void*, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::ReadFile(void* pBytes, Adesk::UInt64* pLength)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;

	return m_DefStream.ReadFile(pBytes, pLength);
}

int AcFsI::SetFilePointer(Adesk::Int64 nOffset, unsigned int method)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;

	return m_DefStream.SetFilePointer(nOffset, method);
}

int AcFsI::SetEndOfFile(void)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;

	return m_DefStream.SetEndOfFile();
}

int AcFsI::ZeroData(Adesk::UInt64 u1, Adesk::UInt64 u2)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;

	return m_DefStream.ZeroData(u1, u2);
}

int AcFsI::GetFilePointer(Adesk::UInt64* pFilePos)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;

	return m_DefStream.GetFilePointer(pFilePos);
}

int AcFsI::GetFileSize(Adesk::UInt64* pFileSize)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;

	return m_DefStream.GetFileSize(pFileSize);
}

void AcFsI::SetCompressionType(int nType)
{
	if (NULL == m_pMHeader)
	{
		if (nType <= 2)
		{
			m_nComprType = nType;
			m_DefStream.m_nComprType = nType;
		}
	}
}

int AcFsI::GetCompressionType(void)
{
	return m_nComprType;
}

void AcFsI::SetCompressionLevel(int level)
{
	m_DefStream.SetCompressionLevel(level);
}

int AcFsI::GetCompressionLevel(void)
{
	return m_DefStream.GetCompressionLevel();
}

void AcFsI::SetBlockSize(Adesk::UInt64 uSize)
{
	Adesk::UInt64 uAlignSize = (uSize + 127) & 0xFFFFFF80;	// µÈ¼ÛÓÚ/128
	if (uAlignSize <= 0x1000000 && (Adesk::Int32)uAlignSize >= m_nAlignment && NULL == m_pMHeader)
	{
		m_uBlockSize = uAlignSize;
		m_DefStream.m_uBlockSize = uAlignSize;
	}
}

void AcFsI::SetMaxCache(int nMaxCache)
{
	if (nMaxCache < 0)
		nMaxCache = 0;

	m_nMaxCache = nMaxCache;
	m_DefStream.m_uMaxCache = nMaxCache;
}

void AcFsI::SetAlignment(int align)
{
	if (align <= 0x1000000 && align <= m_uBlockSize && NULL == m_pMHeader)
	{
		m_nAlignment = align;
		m_nDoubleAlign = 2 * align;
	}
}

int AcFsI::GetAlignment(void)
{
	return m_nAlignment;
}

void AcFsI::EnableWriteCache(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Adesk::UInt64 AcFsI::GetBlockSize(void)
{
	return m_uBlockSize;
}

void AcFsI::SetPrivateHeaderSize(int nSize)
{
	if (NULL == m_pMHeader && nSize > 0)
		m_uPrivtHdrSize = nSize;
}

int AcFsI::GetPrivateHeaderSize(void)
{
	return m_uPrivtHdrSize;
}

int AcFsI::GetAcFsInfo(AcFs_INFO_t* pInfo)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;
	if (NULL == pInfo)
		return ERROR_INVALID_PARAMETER;

	lock();

	pInfo->uTotalSize = 0;
	pInfo->nStreamCount = m_nStreamCount;
	pInfo->nAppFlags = m_nAppFlags;
	AcFsIStream* pStream = &m_DefStream;
	for (Adesk::Int32 i = 0; i < m_nStreamCount; ++i)
	{
		pInfo->uTotalSize += pStream->m_uFileSize;
		pStream = pStream->m_pNextStream;
	}

	m_pMHeader->GetAcFsInfo(pInfo);

	unlock();

	return ERROR_SUCCESS;
}

int AcFsI::GetAcFs6Info(AcFs6_INFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetStreamInfo(int uStreamId, AcFs_STREAMINFO_t* pStreamInfo)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;
	if (NULL == pStreamInfo || (uStreamId < 0 || uStreamId >= m_nStreamCount))
		return ERROR_INVALID_PARAMETER;

	lock();

	AcFsIStream* pStream = &m_DefStream;
	for (int i = 0; i < uStreamId && pStream != NULL; ++i)
		pStream = pStream->m_pNextStream;

	int ret = ERROR_INVALID_PARAMETER;
	if (pStream != NULL)
		ret = pStream->GetStreamInfo(pStreamInfo);	// 80

	unlock();

	return ret;
}

void AcFsI::ResyncSystemFilePointer(void)
{
	lock();
	if (m_pMHeader != NULL)
		m_pMHeader->m_nCurPos = -1;
	unlock();
}

int AcFsI::WritePrivateHeader(const void*, int, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::ReadPrivateHeader(void* pData, int nSize, int nPos)
{
	if (NULL == m_pMHeader)
		return ERROR_INVALID_HANDLE;
	if (NULL == pData || nSize < 0 || nPos < 0 || nSize + nPos > m_uPrivtHdrSize)
		return ERROR_INVALID_PARAMETER;

	int ret = ERROR_SUCCESS;
	if (nSize > 0)
	{
		lock();
		ret = m_pMHeader->ReadData(pData, nSize, nPos);
		unlock();
	}

	return ret;
}

int AcFsI::OpenStream(const char* pStreamName, unsigned int uStreamId, AcFsStream** ppStream)
{
	wchar_t szWStream[MAX_STREAM_NAME_LEN] = { 0 };
	if (pStreamName)
	{
		if (!MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, pStreamName, -1, szWStream, MAX_STREAM_NAME_LEN))
			return GetLastError();
	}

	return OpenStream(szWStream, uStreamId, ppStream);
}

int AcFsI::OpenStream(const wchar_t* pStreamName, unsigned int uStreamId, AcFsStream** ppStream)
{
	lock();

	AcFsIStream* pFound = FindStream(pStreamName, uStreamId);
	*ppStream = pFound;
	if (NULL == pFound)
	{
		unlock();
		return ERROR_NOT_FOUND;
	}

	pFound->m_uAcessType |= 0x80000000;
	if (m_pCallback)
	{
		AC_ASSERT_NOT_IMPLEMENTED();
	}

	return ERROR_SUCCESS;
}

int AcFsI::CreateStream(const char*, unsigned int*, unsigned int, unsigned int, AcFsStream**, int, Adesk::UInt64, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::CreateStream(const wchar_t*, unsigned int*, unsigned int, unsigned int, AcFsStream**, int, Adesk::UInt64, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::DeleteStream(const char*, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::DeleteStream(const wchar_t*, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFsI::RegisterCallback(AcFsCallBack* pCallback)
{
	m_pCallback = pCallback;
}

void AcFsI::DefineAppFlags(unsigned int, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFsI::GetAppFlags(void)
{
	return m_nAppFlags;
}

void AcFsI::SetKey(const unsigned int*)
{

}

Adesk::Boolean AcFsI::IsRealAcFsFile(void)
{
	return Adesk::kTrue;
}

void AcFsI::SetV6StreamCallbackFix(bool)
{

}

int AcFsI::GetLastWriteError(void)
{
	return m_pMHeader->m_nLastWrError;
}

int AcFsI::GetNumberOfWriteErrors(void)
{
	return m_pMHeader->m_nNumOfWrErrs;
}

void AcFsI::SetThreading(int)
{
	
}

int AcFsI::iCreateFile(const void* pFileName, 
					   unsigned int desiredAccess, 
					   unsigned int shareMode,
					   unsigned int dwCreationDisposition,
					   unsigned int dwFlagsAndAttributes,
					   bool bUnicode,
					   HANDLE hFile, 
					   Adesk::Int64 nPos)
{
	if (m_pMHeader != NULL)
		return ERROR_BUSY;

	lock();

	m_pMHeader = new AcFs_mheader();
	if (NULL == m_pMHeader)
	{
		unlock();
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	
	m_pMHeader->m_pFsHeap = m_pFsHeap;		// 35 * 8 = 280

	if (desiredAccess & GENERIC_WRITE)
		desiredAccess |= GENERIC_READ;
	m_uAccessMode = desiredAccess;
	m_pMHeader->m_nAlignment = m_nAlignment;
	m_pMHeader->m_nDoubleAlign = m_nDoubleAlign;
	m_pMHeader->m_uFileHdrAddr = ((m_uPrivtHdrSize + 127) / 128) << 7;

	int nRet = m_pMHeader->OpenFile(pFileName, desiredAccess, shareMode, dwCreationDisposition, dwFlagsAndAttributes, bUnicode, hFile, nPos);
	if (ERROR_SUCCESS == nRet)
	{
		m_nAlignment = m_pMHeader->m_nAlignment;
		m_nDoubleAlign = m_pMHeader->m_nDoubleAlign;

		int ret = processLogicalHeaders();
		if (ERROR_SUCCESS == ret)
		{
			if (m_pCallback)
			{
				//if ()
				AC_ASSERT_NOT_IMPLEMENTED();
			}
		}
		else
		{
			m_pMHeader->CloseFile();
			DeleteMemory();
		}
	}
	else
	{
		DeleteMemory();
	}

	unlock();

	return nRet;
}

int AcFsI::processLogicalHeaders(void)
{
	int ret = ReadHeader();
	if (ERROR_SUCCESS == ret)
	{
		Adesk::UInt32 uBlockSize = m_DefStream.m_uBlockSize;
		AcFsIStream* pStream = m_DefStream.m_pNextStream;
		while (pStream)
		{
			if (uBlockSize < pStream->m_uBlockSize)
				uBlockSize = pStream->m_uBlockSize;
			pStream = pStream->m_pNextStream;
		}
		ret = CheckBlockSize(uBlockSize);
	}
	m_pMHeader->FreeXlat();
	return ret;
}

int AcFsI::ReadHeader(void)
{
	int nRet = ERROR_SUCCESS;
	m_nStreamCount = 1;
	
	m_DefStream.m_uBlockSize = m_uBlockSize;
	m_DefStream.m_pAcFsI = this;
	m_DefStream.m_uAcessType = m_uAccessMode;
	m_DefStream.m_uAppFlags = 0;
	m_DefStream.m_pMHeader = m_pMHeader;

	AcFs_mbheader* pStreamsBlkHdr = m_pMHeader->m_pStrmsBlkHdr;
	if (NULL == pStreamsBlkHdr)
		return ERROR_SUCCESS;

	AcFsComprHeader comprHdr;
	nRet = m_pMHeader->ReadBlock(pStreamsBlkHdr, &comprHdr, 0, sizeof(comprHdr));
	if (nRet != ERROR_SUCCESS)
		return nRet;

	if (comprHdr.uSignature != STRMBLK_HEADER_SIGN || comprHdr.nComprSize > pStreamsBlkHdr->nBlkSize)
		return ERROR_FILE_CORRUPT;

	AC_BYTE* pComprData = (AC_BYTE*)malloc(comprHdr.nComprSize);
	if (NULL == pComprData)
		return ERROR_NOT_ENOUGH_MEMORY;

	nRet = m_pMHeader->ReadBlock(pStreamsBlkHdr, pComprData, 20, comprHdr.nComprSize);
	if (nRet != ERROR_SUCCESS)
	{
		free(pComprData);
		return nRet;
	}

	Adesk::UInt32 uHeaderSum = comprHdr.uCheckSum;
	comprHdr.uCheckSum = 0;

	Adesk::UInt32 uAdler32 = lzo_adler32(0, (const lzo_bytep)&comprHdr, sizeof(comprHdr));
	if (uHeaderSum != lzo_adler32(uAdler32, pComprData, comprHdr.nComprSize))
	{
		free(pComprData);
		return ERROR_FILE_CORRUPT;
	}

	AC_BYTE* pDecomprData = (AC_BYTE*)malloc(comprHdr.uDecomprSize);
	if (NULL == pDecomprData)
	{
		free(pComprData);
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	lzo_uint uDecprSize = comprHdr.uDecomprSize;
	nRet = lzo1y_decompress(pComprData, comprHdr.nComprSize, pDecomprData, &uDecprSize, NULL);
	free(pComprData);
	if (nRet != 0)
	{
		free(pDecomprData);
		return nRet;
	}

	Adesk::Int32* pData = (Adesk::Int32*)pDecomprData;
	m_nStreamCount = *pData++;
	m_nComprType = *pData++;
	m_uBlockSize = *pData++;
	m_nAppFlags = *pData++;
	m_nUnk668 = *pData++;

	char* pBytes = (char*)pData;
	AcFsIStream* pLastStream = NULL;
	AcFsIStream* pIStream = &m_DefStream;
	Adesk::Int32 nIdx = 0;
	while (nIdx < m_nStreamCount && ERROR_SUCCESS == nRet)
	{
		AcFs_fstream* pFStream = (AcFs_fstream*)pBytes;
		pBytes = (char*)(pFStream + 1);
		if (pLastStream != NULL)
		{			
			pIStream = new AcFsIStream();
			if (NULL == pIStream)
			{
				nRet = ERROR_NOT_ENOUGH_MEMORY;
				break;
			}

			pIStream->m_pFsHeap = m_pFsHeap;
			pIStream->m_pPrevStream = pLastStream;
			pLastStream->m_pNextStream = pIStream;
		}

		pIStream->SetMaxCache(m_nMaxCache);
		pIStream->m_uAcessType = m_uAccessMode;
		pIStream->CopyFtoM(this, m_pMHeader, pFStream);
		pIStream->ReadNodes(&pBytes, pFStream->m_nNodeCount);
		pLastStream = pIStream;
		++nIdx;
	}

	free(pDecomprData);
	return nRet;
}

int AcFsI::CheckBlockSize(int nBlockSize)
{
	if (nBlockSize > m_ErrorTempBuff.m_uSize)
	{
		int nSize = 2 * ((nBlockSize >> 3) + nBlockSize) + 1024;
		m_OrgFileBuff.SetSize(nSize);
		m_BuffUnk512.SetSize(nSize);
		m_ErrorTempBuff.SetSize(nBlockSize);

		ResetAllCachePtrs();

		if (NULL == m_OrgFileBuff.m_pBuffer ||
			NULL == m_BuffUnk512.m_pBuffer ||
			NULL == m_ErrorTempBuff.m_pBuffer)
			return ERROR_NOT_ENOUGH_MEMORY;
	}

	return ERROR_SUCCESS;
}

void AcFsI::DeleteMemory(void)
{
	AcFsIStream* pStream = &m_DefStream;
	while (pStream != NULL)
	{
		AcFsIStream* pNextStream = pStream->m_pNextStream;
		pStream->DeleteMemory();
		if (pStream->GetStreamID())		// 120
			delete pStream;				// 184

		pStream = pNextStream;
	}

	m_OrgFileBuff.Reset();
	m_BuffUnk512.Reset();
	m_BuffUnk536.Reset();
	m_ErrorTempBuff.Reset();

	if (m_pMHeader != NULL)
	{
		delete m_pMHeader;
		m_pMHeader = NULL;
	}

	m_DefStream.Reset();
	Reset();
}

AcFsIStream* AcFsI::FindStream(const wchar_t* pStreamName, unsigned int uStreamId)
{
	AcFsIStream* pStream = &m_DefStream;
	for (; pStream != NULL; pStream = pStream->m_pNextStream)
	{
		bool bFound = pStreamName && *pStreamName ? 0 == ac_strcmp(pStreamName, pStream->m_szStreamName) : uStreamId == pStream->m_nStreamId;
		if (bFound)
			break;
	}
	return pStream;
}

void AcFsI::Reset(void)
{
	m_pMHeader = NULL;
	m_nStreamCount = 1;
	m_nMaxCache = 0x200000;

	m_OrgFileBuff.Reset();
	m_BuffUnk512.Reset();
	m_ErrorTempBuff.Reset();
	m_BuffUnk536.Reset();

	m_nAlignment = 32;
	m_nDoubleAlign = 64;
	m_uUnk584 = 0;
	m_uBlockSize = 29696;
	m_nComprType = 2;
	m_uPrivtHdrSize = 0;
	m_pCallback = NULL;
	m_nAppFlags = 0;
	m_nUnk668 = 1;
}

void AcFsI::ResetAllCachePtrs(void)
{
	AcFsIStream* pStream = &m_DefStream;
	for (; pStream != NULL; pStream = pStream->m_pNextStream)
	{
		pStream->m_pBufferEnd = NULL;
		pStream->m_pBufferCur = NULL;
		pStream->m_bUnk380 = Adesk::kTrue;
	}
}

int AcFsI::uncompress(void* pDstBuf, Adesk::UInt32* pDstLen, const void* pSrcBuf, Adesk::UInt32 nSrcLen, int nType)
{
	int ret = -1;
	if (2 == nType)
	{
		lzo_uint uDst = *pDstLen;
		ret = lzo1y_decompress((const lzo_bytep)pSrcBuf, nSrcLen, (lzo_bytep)pDstBuf, &uDst, NULL);
		*pDstLen = uDst;
	}
	else if (nType != 3)
	{
		*pDstLen = nSrcLen;
		memcpy(pDstBuf, pSrcBuf, nSrcLen);
		ret = 0;
	}

	return ret;
}

int AcFsI::iFlush(bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}
