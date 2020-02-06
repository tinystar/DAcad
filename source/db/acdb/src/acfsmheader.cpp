#include "acfsmheader.h"
#include "acfsglobal.h"
#include "acutmacro.h"
#include "acfsheap.h"
#include "lzo/lzoconf.h"
#include "lzo/lzo1y.h"
#include "acfsmbheader.h"
#include "acfscmprheader.h"

static const int MHEADER_SIZE = 108;	// 0x6C

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
		if (!m_bKeepFile)
			CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	
	DeleteMemory();
}

void AcFs_mheader::Reset(void)
{
	m_pUnk0 = NULL;
	m_pUnk8 = NULL;
	m_pUnk16 = NULL;
	m_pMemBlkHead = NULL;
	m_pMemBlkTail = NULL;
	m_nUnk40 = 0;
	m_nUnk48 = 0;
	m_nCurPos = 0;
	m_nUnk64 = 0;
	m_nUnk68 = 0;
	m_nUnk72 = 32;
	m_nUnk76 = 128;
	m_nUnk80 = 64;
	m_nUnk84 = 0;
	m_nUnk88 = 0;
	m_nUnk92 = 0;
	m_pUnk104 = NULL;
	m_pUnk112 = NULL;
	m_pUnk120 = NULL;
	m_pUnk144 = NULL;
	m_uUnk260 = 0;
	m_bKeepFile = Adesk::kFalse;
	m_nUnk268 = 0;
	m_nUnk272 = 0;

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
	m_bKeepFile = Adesk::kFalse;
	m_nUnk84 = ERROR_SUCCESS;
	m_uUnk140 = desiredAccess;

	if (hFile != NULL)
	{
		m_bKeepFile = Adesk::kTrue;
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
			m_nUnk84 = GetLastError();
			return m_nUnk84;
		}

		if (GetFileType(m_hFile) != FILE_TYPE_DISK)
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
			m_nUnk84 = ERROR_NOT_SUPPORTED;
			return m_nUnk84;
		}
	}

	SetLastError(0);
	int ret = ReadFileHeader(nFileHdrPos);
	if (ret != ERROR_SUCCESS)
	{
		if (ERROR_HANDLE_EOF == ret)
		{
			m_nUnk84 = ret & 0xFFFFFF00;
			return m_nUnk84;
		}
	}
	else
	{
		ret = ReadMemBlks();
	}

	if (ret != ERROR_SUCCESS)
	{
		if (!m_bKeepFile)
			CloseHandle(m_hFile);
		m_hFile = NULL;

		m_nUnk84 = ret;
		return m_nUnk84;
	}

	m_nUnk84 = ERROR_SUCCESS;
	return m_nUnk84;
}

int AcFs_mheader::CloseFile(void)
{
	SetLastError(0);
	int ret = ERROR_SUCCESS;
	if (m_hFile && !m_bKeepFile && !CloseHandle(m_hFile))
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
	return ReadData(pData, nSize, m_uUnk260 + m_nUnk76 + pMBHeader->nUnk16 + nOff);
}

int AcFs_mheader::ReadFileHeader(Adesk::Int64 nPos)
{
	if (0 == nPos)
		nPos = m_uUnk260;

	AC_BYTE header[MHEADER_SIZE];
	int nRet = ReadData(header, MHEADER_SIZE, nPos);
	if (nRet != ERROR_SUCCESS)
	{
		m_fHeader.Reset();
	}
	else
	{
		Adesk::UInt32* pHdrByts = (Adesk::UInt32*)header;
		Adesk::UInt32* pHdrEnd = (Adesk::UInt32*)(header + MHEADER_SIZE);
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

			m_nUnk40 = m_fHeader.m_nUnk44;		// 196 - 152
			m_nUnk48 = m_fHeader.m_nUnk52;		// 204 - 152
			m_nUnk64 = m_fHeader.m_nUnk60;		// 212 - 152
			m_nUnk68 = m_fHeader.m_nUnk64;		// 216 - 152
			m_nUnk72 = m_fHeader.m_nUnk68;		// 220 - 152
			m_nUnk76 = m_fHeader.m_nUnk72;		// 224 - 152
			m_nUnk80 = m_fHeader.m_nUnk76;		// 228 - 152
			m_nUnk268 = m_fHeader.m_nUnk96;		// 248 - 152
			m_nUnk272 = m_fHeader.m_nUnk100;	// 252 - 152
		}
	}

	return nRet;
}

int AcFs_mheader::ReadMemBlks(void)
{
	if (0 == m_nUnk64 + m_nUnk68)
		return ERROR_SUCCESS;

	m_pUnk112 = NULL;
	m_pUnk120 = NULL;

	if (m_nUnk68 != 0)
	{
		m_pUnk120 = (AcFs_mbheader**)m_pFsHeap->zalloc(8 * m_nUnk268);
		if (NULL == m_pUnk120)
			return ERROR_NOT_ENOUGH_MEMORY;
	}
	if (m_nUnk64 != 0)
	{
		m_pUnk112 = (AcFs_mbheader**)m_pFsHeap->zalloc(8 * m_nUnk272);
		if (NULL == m_pUnk112)
			return ERROR_NOT_ENOUGH_MEMORY;
	}

	Adesk::Int64 nBlksHdrPos = m_fHeader.m_nUnk84 + m_uUnk260 + m_nUnk76;
	AcFsComprHeader comprHdr;
	int ret = ReadData(&comprHdr, sizeof(comprHdr), nBlksHdrPos);
	if (ret != ERROR_SUCCESS)
		return ret;

	Adesk::Int64 nBlksPos = nBlksHdrPos + sizeof(comprHdr);
	if (comprHdr.uSignature != 0x41630E3B || nBlksPos + comprHdr.uComprSize > m_nUnk48 || comprHdr.uComprSize < 0)
		return ERROR_FILE_CORRUPT;
	
	AC_BYTE* pMemBlks = (AC_BYTE*)malloc(comprHdr.uComprSize);
	if (NULL == pMemBlks)
		return ERROR_NOT_ENOUGH_MEMORY;

	ret = ReadData(pMemBlks, comprHdr.uComprSize, nBlksPos);
	if (ret != ERROR_SUCCESS)
	{
		free(pMemBlks);
		return ret;
	}

	Adesk::UInt32 uHeaderSum = comprHdr.uCheckSum;
	comprHdr.uCheckSum = 0;

	Adesk::UInt32 uAdler32 = lzo_adler32(0, (const lzo_bytep)&comprHdr, sizeof(comprHdr));
	if (uHeaderSum != lzo_adler32(uAdler32, pMemBlks, comprHdr.uComprSize))
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
 	int nRet = lzo1y_decompress(pMemBlks, comprHdr.uComprSize, pDecomprData, &uDecprSize, NULL);
	free(pMemBlks);
	if (nRet != 0)
	{
		free(pDecomprData);
		return nRet;
	}

	AC_BYTE* pStream = pDecomprData;
	AcFs_mbheader* pLastHdr = NULL;
	Adesk::Int32 i = 0;
	Adesk::Int64 nUnk60 = 0;
	while (i < (m_nUnk64 + m_nUnk68))
	{
		AcFs_mbheader* pBlkHdr = (AcFs_mbheader*)malloc(sizeof(AcFs_mbheader));
		Adesk::Int32 unk = *(Adesk::Int32*)pStream;
		Adesk::Int32* pData32 = (Adesk::Int32*)pStream;
		if (unk >= 0)
		{
			pStream += 8;

			AC_ASSERT(unk > 0);
			m_pUnk120[unk - 1] = pBlkHdr;
			pBlkHdr->pUnk32 = 0;
			pBlkHdr->pUnk40 = 0;
			pBlkHdr->pUnk48 = 0;
			pBlkHdr->pUnk56 = 0;
		}
		else
		{
			pStream += 24;

			m_pUnk112[-unk - 1] = pBlkHdr;
			pBlkHdr->pUnk32 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[2];
			pBlkHdr->pUnk40 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[3];
			pBlkHdr->pUnk48 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[4];
			pBlkHdr->pUnk56 = (AcFs_mbheader*)(Adesk::IntPtr)pData32[5];
		}

		pBlkHdr->nUnk28 = pData32[0];
		pBlkHdr->nUnk24 = pData32[1];

		pBlkHdr->pNext = 0;
		if (pLastHdr != NULL)
			pLastHdr->pNext = pBlkHdr;
		else
			m_pMemBlkHead = pBlkHdr;
		pBlkHdr->pPrev = pLastHdr;
		pLastHdr = pBlkHdr;

		pBlkHdr->nUnk16 = nUnk60;
		nUnk60 += pBlkHdr->nUnk24;
		++i;
	}

	m_pMemBlkTail = pLastHdr;

	if (m_pUnk112 != NULL)
	{
		for (Adesk::Int32 i = 0; i < m_nUnk272; ++i)
		{
			AcFs_mbheader* pBlkHdr = m_pUnk112[i];
			if (pBlkHdr != NULL)
			{
				pBlkHdr->pUnk32 = ConvertID((int)pBlkHdr->pUnk32);
				pBlkHdr->pUnk40 = ConvertID((int)pBlkHdr->pUnk40);
				pBlkHdr->pUnk48 = ConvertID((int)pBlkHdr->pUnk48);
				pBlkHdr->pUnk56 = ConvertID((int)pBlkHdr->pUnk56);
			}
		}
	}

	m_pUnk0 = ConvertID(m_fHeader.m_nUnk24);
	m_pUnk8 = ConvertID(m_fHeader.m_nUnk28);
	m_pUnk16 = ConvertID(m_fHeader.m_nUnk32);
	m_pUnk104 = ConvertID(m_fHeader.m_nUnk80);
	m_pUnk144 = ConvertID(m_fHeader.m_nUnk92);

	free(pDecomprData);

	return ERROR_SUCCESS;
}

AcFs_mbheader* AcFs_mheader::ConvertID(int id)
{
	if (0 == id)
		return NULL;
	else if (id > 0)
		return m_pUnk120[id - 1];
	else
		return m_pUnk112[-id - 1];
}

void AcFs_mheader::FreeXlat(void)
{
	if (m_pUnk112 != NULL)
	{
		free(m_pUnk112);
		m_pUnk112 = NULL;
	}
	if (m_pUnk120 != NULL)
	{
		free(m_pUnk120);
		m_pUnk120 = NULL;
	}
}

void AcFs_mheader::GetAcFsInfo(AcFs_INFO_t* pInfo)
{
	pInfo->uUnk40 = m_nUnk72;
	pInfo->uUnk0 = m_nUnk48;
	pInfo->nUnk32 = m_nUnk68;
	pInfo->pUnk16 = NULL;
	pInfo->pUnk24 = NULL;
	pInfo->nUnk36 = m_nUnk64;

	for (AcFs_mbheader* pmbheader = m_pMemBlkHead; pmbheader; pmbheader = pmbheader->pNext)
	{
		if (pmbheader->nUnk28 >= 0)
			pInfo->pUnk16 += pmbheader->nUnk24;
		else
			pInfo->pUnk24 += pmbheader->nUnk24;
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
