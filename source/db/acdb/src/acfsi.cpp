#include "acfsi.h"
#include "acutmacro.h"
#include "acfsmheader.h"
#include "acfscmprheader.h"
#include "acfsmbheader.h"
#include "lzo/lzoconf.h"
#include "lzo/lzo1y.h"
#include "acfsfstream.h"


AcFsI::AcFsI()
	: m_pMHeader(NULL)
	, m_pFsHeap(NULL)
	, m_pCallback(NULL)
{
	InitializeCriticalSection(&m_cs);
}

AcFsI::~AcFsI()
{
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

int AcFsI::OpenUsing(HANDLE hFile, unsigned int uUnk)
{
	if (NULL == hFile)
		return ERROR_INVALID_PARAMETER;

	return iCreateFile(NULL, uUnk, 0, 0, 0, false, hFile, 0);
}

int AcFsI::OpenUsing(AcStream*, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

Adesk::Boolean AcFsI::isOpen(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

int AcFsI::SetFileTime(const FILETIME*, const FILETIME*, const FILETIME*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetFileTime(FILETIME*, FILETIME*, FILETIME*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetFileInformationByHandle(BY_HANDLE_FILE_INFORMATION*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::CloseFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::Flush(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::WriteFile(const void*, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::ReadFile(void*, Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::SetFilePointer(Adesk::Int64, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::SetEndOfFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::ZeroData(Adesk::UInt64, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetFilePointer(Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetFileSize(Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFsI::SetCompressionType(int nType)
{
	if (NULL == m_pMHeader)
	{
		if (nType <= 2)
		{
			m_nComprType = nType;
			m_stream1.m_nComprType = nType;
		}
	}
}

int AcFsI::GetCompressionType(void)
{
	return m_nComprType;
}

void AcFsI::SetCompressionLevel(int level)
{
	m_stream1.SetCompressionLevel(level);
}

int AcFsI::GetCompressionLevel(void)
{
	return m_stream1.GetCompressionLevel();
}

void AcFsI::SetBlockSize(Adesk::UInt64 uSize)
{
	Adesk::UInt64 v2 = (uSize + 127) & 0xFFFFFF80;
	Adesk::UInt64 result = v2 - 128;
	if ((unsigned int)result <= 0xFFFF80 && (signed int)v2 >= m_nUnk568 && NULL == m_pMHeader)
	{
		m_uUnk560 = v2;
		m_stream1.m_uBlockSize = v2;
	}
}

void AcFsI::SetMaxCache(int nMaxCache)
{
	if (nMaxCache < 0)
		nMaxCache = 0;

	m_nMaxCache = nMaxCache;
	m_stream1.m_uMaxCache = nMaxCache;
}

void AcFsI::SetAlignment(int align)
{
	unsigned int result = (unsigned int)(align - 16);
	if ((unsigned int)result <= 0xFFFFF0 && align <= m_uUnk560 && NULL == m_pMHeader)
	{
		result = (unsigned int)(2 * align);
		m_nUnk568 = align;
		m_nUnk572 = result;
	}
}

int AcFsI::GetAlignment(void)
{
	return m_nUnk568;
}

void AcFsI::EnableWriteCache(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Adesk::UInt64 AcFsI::GetBlockSize(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

void AcFsI::SetPrivateHeaderSize(int nSize)
{
	Adesk::UInt8 u = ~((Adesk::UInt32)nSize) >> 31;
	if (NULL == m_pMHeader && u != 0)
		m_uPrivtHdrSize = nSize;
}

int AcFsI::GetPrivateHeaderSize(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

int AcFsI::GetAcFsInfo(AcFs_INFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetAcFs6Info(AcFs6_INFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetStreamInfo(int, AcFs_STREAMINFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFsI::ResyncSystemFilePointer(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFsI::WritePrivateHeader(const void*, int, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::ReadPrivateHeader(void*, int, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::OpenStream(const char*, unsigned int, AcFsStream**)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::OpenStream(const wchar_t*, unsigned int, AcFsStream**)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
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
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFsI::SetKey(const unsigned int*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Adesk::Boolean AcFsI::IsRealAcFsFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

void AcFsI::SetV6StreamCallbackFix(bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFsI::GetLastWriteError(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsI::GetNumberOfWriteErrors(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFsI::SetThreading(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
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
	m_pMHeader->m_nUnk72 = m_nUnk568;
	m_pMHeader->m_nUnk80 = m_nUnk572;
	m_pMHeader->m_uUnk260 = ((m_uPrivtHdrSize + 127) / 128) << 7;

	int nRet = m_pMHeader->OpenFile(pFileName, desiredAccess, shareMode, dwCreationDisposition, dwFlagsAndAttributes, bUnicode, hFile, nPos);
	if (ERROR_SUCCESS == nRet)
	{
		m_nUnk568 = m_pMHeader->m_nUnk72;
		m_nUnk572 = m_pMHeader->m_nUnk80;

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
		Adesk::UInt32 uBlockSize = m_stream1.m_uBlockSize;
		AcFsIStream* pStream = m_stream1.m_pNextStream;
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
	m_nUnk8 = 1;
	
	m_stream1.m_uBlockSize = m_uUnk560;
	m_stream1.m_pAcFsI = this;
	m_stream1.m_uAcessType = m_uAccessMode;
	m_stream1.m_uAppFlags = 0;
	m_stream1.m_pMHeader = m_pMHeader;

	AcFs_mbheader* pMBHeader = m_pMHeader->m_pUnk144;
	if (NULL == pMBHeader)
		return ERROR_SUCCESS;

	AcFsComprHeader comprHdr;
	nRet = m_pMHeader->ReadBlock(pMBHeader, &comprHdr, 0, sizeof(comprHdr));
	if (nRet != ERROR_SUCCESS)
		return nRet;

	if (comprHdr.uSignature != 0x4163003B || comprHdr.uComprSize > pMBHeader->nUnk24)
		return ERROR_FILE_CORRUPT;

	AC_BYTE* pComprData = (AC_BYTE*)malloc(comprHdr.uComprSize);
	if (NULL == pComprData)
		return ERROR_NOT_ENOUGH_MEMORY;

	nRet = m_pMHeader->ReadBlock(pMBHeader, pComprData, 20, comprHdr.uComprSize);
	if (nRet != ERROR_SUCCESS)
	{
		free(pComprData);
		return nRet;
	}

	Adesk::UInt32 uHeaderSum = comprHdr.uCheckSum;
	comprHdr.uCheckSum = 0;

	Adesk::UInt32 uAdler32 = lzo_adler32(0, (const lzo_bytep)&comprHdr, sizeof(comprHdr));
	if (uHeaderSum != lzo_adler32(uAdler32, pComprData, comprHdr.uComprSize))
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
	nRet = lzo1y_decompress(pComprData, comprHdr.uComprSize, pDecomprData, &uDecprSize, NULL);
	free(pComprData);
	if (nRet != 0)
	{
		free(pDecomprData);
		return nRet;
	}

	Adesk::Int32* pData = (Adesk::Int32*)pDecomprData;
	m_nUnk8 = *pData++;
	m_nComprType = *pData++;
	m_uUnk560 = *pData++;
	m_nUnk664 = *pData++;
	m_nUnk668 = *pData++;

	char* pBytes = (char*)pData;
	AcFsIStream* pLastStream = NULL;
	AcFsIStream* pIStream = &m_stream1;
	Adesk::Int32 nIdx = 0;
	while (nIdx < m_nUnk8 && ERROR_SUCCESS == nRet)
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
		pIStream->ReadNodes(&pBytes, pFStream->m_nUnk8);
		pLastStream = pIStream;
		++nIdx;
	}

	free(pDecomprData);
	return nRet;
}

int AcFsI::CheckBlockSize(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

void AcFsI::DeleteMemory(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
