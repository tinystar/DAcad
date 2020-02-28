#include "acfsmheader.h"
#include "acfsglobal.h"
#include "acutmacro.h"
#include "acfsheap.h"
#include "lzo/lzoconf.h"
#include "lzo/lzo1y.h"
#include "acfsmbheader.h"
#include "acfscmprheader.h"


AcFs_mheader::AcFs_mheader()
{
	m_fHeader.Reset();
	m_hFile = NULL;
	Reset();
}

AcFs_mheader::~AcFs_mheader()
{
	if (m_hFile != NULL)
	{
		if (!m_bRefFile)
			CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	
	DeleteMemory();
}

void AcFs_mheader::Reset(void)
{
	m_pUnkHdr0 = NULL;
	m_pUnkHdr8 = NULL;
	m_pUnkHdr16 = NULL;
	m_pMemBlkHead = NULL;
	m_pMemBlkTail = NULL;
	m_nUnk40 = 0;
	m_nMBEndAddr = 0;
	m_nCurPos = 0;
	m_nNegBlksCount = 0;
	m_nPosBlksCount = 0;
	m_nAlignment = 32;
	m_nAlignFileHdrSize = 128;
	m_nDoubleAlign = 64;
	m_nLastRdError = 0;
	m_nLastWrError = 0;
	m_nNumOfWrErrs = 0;
	m_pBlksInfoHdr = NULL;
	m_pNegBlkHdrs = NULL;
	m_pPosBlkHdrs = NULL;
	m_pStrmsBlkHdr = NULL;
	m_uFileHdrAddr = 0;
	m_bRefFile = Adesk::kFalse;
	m_nPosBlksMaxId = 0;
	m_nNegBlksMaxId = 0;

	Adesk::UInt32 uSeed = 1;
	for (int i = 0; i < 256; ++i)
	{
		uSeed = 0x343FD * uSeed + 0x269EC3;
		m_magic[i] = (AC_BYTE)(uSeed >> 16);
	}
}

int AcFs_mheader::OpenFile(const void* pFileName, 
						   unsigned int desiredAccess, 
						   unsigned int shareMode, 
						   unsigned int dwCreationDisposition, 
						   unsigned int dwFlagsAndAttributes, 
						   bool bUnicode, 
						   HANDLE hFile, 
						   Adesk::Int64 nFileHdrPos)
{
	m_bRefFile = Adesk::kFalse;
	m_nLastRdError = ERROR_SUCCESS;
	m_uDesiredAccess = desiredAccess;

	if (hFile != NULL)
	{
		m_bRefFile = Adesk::kTrue;
		m_hFile = hFile;
	}
	else
	{
		if (bUnicode)
			m_hFile = CreateFileW((LPCWSTR)pFileName, desiredAccess, shareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
		else
			m_hFile = CreateFileA((LPCSTR)pFileName, desiredAccess, shareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			m_hFile = NULL;
			m_nLastRdError = GetLastError();
			return m_nLastRdError;
		}

		if (GetFileType(m_hFile) != FILE_TYPE_DISK)
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
			m_nLastRdError = ERROR_NOT_SUPPORTED;
			return m_nLastRdError;
		}
	}

	SetLastError(0);
	int ret = ReadFileHeader(nFileHdrPos);
	if (ret != ERROR_SUCCESS)
	{
		if (ERROR_HANDLE_EOF == ret)
		{
			m_nLastRdError = ret & 0xFFFFFF00;
			return m_nLastRdError;
		}
	}
	else
	{
		ret = ReadMemBlks();
	}

	if (ret != ERROR_SUCCESS)
	{
		if (!m_bRefFile)
			CloseHandle(m_hFile);
		m_hFile = NULL;

		m_nLastRdError = ret;
		return m_nLastRdError;
	}

	m_nLastRdError = ERROR_SUCCESS;
	return m_nLastRdError;
}

int AcFs_mheader::CloseFile(void)
{
	SetLastError(0);
	int ret = ERROR_SUCCESS;
	if (m_hFile && !m_bRefFile && !CloseHandle(m_hFile))
		ret = GetLastError();
	m_hFile = NULL;
	DeleteMemory();
	return ret;
}

int AcFs_mheader::ReadData(void* pData, int nSize, Adesk::Int64 nPos)
{
	int nRet = ERROR_SUCCESS;
	if (nPos == m_nCurPos || (m_nCurPos = nPos, (nRet = iFileSeek(m_hFile, nPos)) == 0))
	{
		DWORD dwBytesRead = 0;
		BOOL b = ReadFile(m_hFile, pData, nSize, &dwBytesRead, NULL);
		m_nCurPos += dwBytesRead;
		if (!b || (DWORD)nSize != dwBytesRead)
		{
			m_nCurPos = -1;
			nRet = GetLastError();
			if (0 == nRet)
				nRet = ERROR_HANDLE_EOF;
		}
	}

	return nRet;
}

int AcFs_mheader::ReadBlock(AcFs_mbheader* pMBHeader, void* pData, int nOff, int nSize)
{
	return ReadData(pData, nSize, m_uFileHdrAddr + m_nAlignFileHdrSize + pMBHeader->nOffset + nOff);
}

int AcFs_mheader::ReadFileHeader(Adesk::Int64 nPos)
{
	if (0 == nPos)
		nPos = m_uFileHdrAddr;

	AC_BYTE header[ACFS_HEADER_SIZE];
	int nRet = ReadData(header, ACFS_HEADER_SIZE, nPos);
	if (nRet != ERROR_SUCCESS)
	{
		m_fHeader.Reset();
	}
	else
	{
		Adesk::UInt32* pHdrByts = (Adesk::UInt32*)header;
		Adesk::UInt32* pHdrEnd = (Adesk::UInt32*)(header + ACFS_HEADER_SIZE);
		Adesk::UInt32* pMagic = (Adesk::UInt32*)m_magic;

		while (pHdrByts < pHdrEnd)
		{
			*pHdrByts++ ^= *pMagic++;		
		}

		nRet = ERROR_FILE_CORRUPT;

		memcpy(&m_fHeader, header, sizeof(m_fHeader));
		if (m_fHeader.VerifyHeader())
		{
			nRet = ERROR_SUCCESS;

			m_nUnk40 = m_fHeader.m_nUnk44;					// 196 - 152
			m_nMBEndAddr = m_fHeader.m_nMBEndAddr;			// 204 - 152
			m_nNegBlksCount = m_fHeader.m_nNegBlks;			// 212 - 152
			m_nPosBlksCount = m_fHeader.m_nPosBlks;			// 216 - 152
			m_nAlignment = m_fHeader.m_nAlignment;			// 220 - 152
			m_nAlignFileHdrSize = m_fHeader.m_nAlignFHdrSize;		// 224 - 152
			m_nDoubleAlign = m_fHeader.m_nDoubleAlign;		// 228 - 152
			m_nPosBlksMaxId = m_fHeader.m_nPosBlksMaxId;	// 248 - 152
			m_nNegBlksMaxId = m_fHeader.m_nNegBlksMaxId;	// 252 - 152
		}
	}

	return nRet;
}

int AcFs_mheader::ReadMemBlks(void)
{
	if (0 == m_nNegBlksCount + m_nPosBlksCount)
		return ERROR_SUCCESS;

	m_pNegBlkHdrs = NULL;
	m_pPosBlkHdrs = NULL;

	if (m_nPosBlksCount != 0)
	{
		m_pPosBlkHdrs = (AcFs_mbheader**)m_pFsHeap->zalloc(8 * m_nPosBlksMaxId);
		if (NULL == m_pPosBlkHdrs)
			return ERROR_NOT_ENOUGH_MEMORY;
	}
	if (m_nNegBlksCount != 0)
	{
		m_pNegBlkHdrs = (AcFs_mbheader**)m_pFsHeap->zalloc(8 * m_nNegBlksMaxId);
		if (NULL == m_pNegBlkHdrs)
			return ERROR_NOT_ENOUGH_MEMORY;
	}

	Adesk::Int64 nBlksHdrPos = m_fHeader.m_nBlksHdrOffset + m_uFileHdrAddr + m_nAlignFileHdrSize;
	AcFsComprHeader comprHdr;
	int ret = ReadData(&comprHdr, sizeof(comprHdr), nBlksHdrPos);
	if (ret != ERROR_SUCCESS)
		return ret;

	Adesk::Int64 nBlksPos = nBlksHdrPos + sizeof(comprHdr);
	if (comprHdr.uSignature != MEMBLKS_HEADER_SIGN || nBlksPos + comprHdr.nComprSize > m_nMBEndAddr || comprHdr.nComprSize < 0)
		return ERROR_FILE_CORRUPT;
	
	AC_BYTE* pMemBlks = (AC_BYTE*)malloc(comprHdr.nComprSize);
	if (NULL == pMemBlks)
		return ERROR_NOT_ENOUGH_MEMORY;

	ret = ReadData(pMemBlks, comprHdr.nComprSize, nBlksPos);
	if (ret != ERROR_SUCCESS)
	{
		free(pMemBlks);
		return ret;
	}

	Adesk::UInt32 uHeaderSum = comprHdr.uCheckSum;
	comprHdr.uCheckSum = 0;

	Adesk::UInt32 uAdler32 = lzo_adler32(0, (const lzo_bytep)&comprHdr, sizeof(comprHdr));
	if (uHeaderSum != lzo_adler32(uAdler32, pMemBlks, comprHdr.nComprSize))
	{
		free(pMemBlks);
		return ERROR_FILE_CORRUPT;
	}

	AC_BYTE* pDecomprData = (AC_BYTE*)malloc(comprHdr.uDecomprSize);
	if (NULL == pDecomprData)
	{
		free(pMemBlks);
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	lzo_uint uDecprSize = comprHdr.uDecomprSize;
 	int nRet = lzo1y_decompress(pMemBlks, comprHdr.nComprSize, pDecomprData, &uDecprSize, NULL);
	free(pMemBlks);
	if (nRet != 0)
	{
		free(pDecomprData);
		return nRet;
	}

	AC_BYTE* pStream = pDecomprData;
	AcFs_mbheader* pLastHdr = NULL;
	Adesk::Int32 i = 0;
	Adesk::Int64 nOffset = 0;
	while (i < (m_nNegBlksCount + m_nPosBlksCount))
	{
		AcFs_mbheader* pBlkHdr = (AcFs_mbheader*)malloc(sizeof(AcFs_mbheader));
		Adesk::Int32 nBlkHdrId = *(Adesk::Int32*)pStream;
		Adesk::Int32* pData32 = (Adesk::Int32*)pStream;
		if (nBlkHdrId >= 0)
		{
			pStream += 8;

			AC_ASSERT(nBlkHdrId > 0);
			m_pPosBlkHdrs[nBlkHdrId - 1] = pBlkHdr;
			pBlkHdr->pUnk32 = 0;
			pBlkHdr->pUnk40 = 0;
			pBlkHdr->pUnk48 = 0;
			pBlkHdr->pUnk56 = 0;
		}
		else
		{
			pStream += 24;

			m_pNegBlkHdrs[-nBlkHdrId - 1] = pBlkHdr;
			pBlkHdr->pUnk32 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[2];
			pBlkHdr->pUnk40 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[3];
			pBlkHdr->pUnk48 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[4];
			pBlkHdr->pUnk56 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[5];
		}

		pBlkHdr->nBlkId = pData32[0];
		pBlkHdr->nBlkSize = pData32[1];

		pBlkHdr->pNext = 0;
		if (pLastHdr != NULL)
			pLastHdr->pNext = pBlkHdr;
		else
			m_pMemBlkHead = pBlkHdr;
		pBlkHdr->pPrev = pLastHdr;
		pLastHdr = pBlkHdr;

		pBlkHdr->nOffset = nOffset;
		nOffset += pBlkHdr->nBlkSize;
		++i;
	}

	m_pMemBlkTail = pLastHdr;

	if (m_pNegBlkHdrs != NULL)
	{
		for (Adesk::Int32 i = 0; i < m_nNegBlksMaxId; ++i)
		{
			AcFs_mbheader* pBlkHdr = m_pNegBlkHdrs[i];
			if (pBlkHdr != NULL)
			{
				pBlkHdr->pUnk32 = ConvertID((int)pBlkHdr->pUnk32);
				pBlkHdr->pUnk40 = ConvertID((int)pBlkHdr->pUnk40);
				pBlkHdr->pUnk48 = ConvertID((int)pBlkHdr->pUnk48);
				pBlkHdr->pUnk56 = ConvertID((int)pBlkHdr->pUnk56);
			}
		}
	}

	m_pUnkHdr0 = ConvertID(m_fHeader.m_nUnk24);
	m_pUnkHdr8 = ConvertID(m_fHeader.m_nUnk28);
	m_pUnkHdr16 = ConvertID(m_fHeader.m_nUnk32);
	m_pBlksInfoHdr = ConvertID(m_fHeader.m_nBlksInfoBlkId);
	m_pStrmsBlkHdr = ConvertID(m_fHeader.m_nStreamsBlkId);

	free(pDecomprData);

	return ERROR_SUCCESS;
}

AcFs_mbheader* AcFs_mheader::ConvertID(int id)
{
	if (0 == id)
		return NULL;
	else if (id > 0)
		return m_pPosBlkHdrs[id - 1];
	else
		return m_pNegBlkHdrs[-id - 1];
}

void AcFs_mheader::FreeXlat(void)
{
	if (m_pNegBlkHdrs != NULL)
	{
		free(m_pNegBlkHdrs);
		m_pNegBlkHdrs = NULL;
	}
	if (m_pPosBlkHdrs != NULL)
	{
		free(m_pPosBlkHdrs);
		m_pPosBlkHdrs = NULL;
	}
}

void AcFs_mheader::GetAcFsInfo(AcFs_INFO_t* pInfo)
{
	pInfo->nAlignment = m_nAlignment;
	pInfo->nMemBlkEndAddr = m_nMBEndAddr;
	pInfo->nPosBlkCount = m_nPosBlksCount;
	pInfo->nTotalPosBlkSize = 0;
	pInfo->nTotalNegBlkSize = 0;
	pInfo->nNegBlkCount = m_nNegBlksCount;

	for (AcFs_mbheader* pmbheader = m_pMemBlkHead; pmbheader; pmbheader = pmbheader->pNext)
	{
		if (pmbheader->nBlkId >= 0)
			pInfo->nTotalPosBlkSize += pmbheader->nBlkSize;
		else
			pInfo->nTotalNegBlkSize += pmbheader->nBlkSize;
	}
}

void AcFs_mheader::DeleteMemory(void)
{
	FreeXlat();
	AcFs_mbheader* pmbheader = m_pMemBlkHead;
	while (pmbheader != NULL)
	{
		AcFs_mbheader* pNextHeader = pmbheader->pNext;
		free(pmbheader);
		pmbheader = pNextHeader;
	}

	Reset();
}
