#include "acfsistream.h"
#include "acutmacro.h"
#include <windows.h>
#include "acfsi.h"
#include "acfsnode.h"
#include "acfsheap.h"
#include "acfsmheader.h"
#include "acfsmbheader.h"
#include "lzo/lzoconf.h"
#include "cstrop.h"
#include "acfscmprheader.h"


AcFsIStream::AcFsIStream(void)
{
	Reset();
}

AcFsIStream::~AcFsIStream()
{
	DeleteMemory();
}

int AcFsIStream::WriteFile(const void*, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsIStream::ReadFile(void* pBuf, Adesk::UInt64* pSize)
{
	int ret = ERROR_SUCCESS;
	Adesk::UInt64 uReadSize = *pSize;
	if (m_pBufferCur + uReadSize > m_pBufferEnd)
	{
		AC_BYTE* pReadBuf = (AC_BYTE*)pBuf;
		while (uReadSize > 0)
		{
			Adesk::UInt64 uRemainSize = m_pBufferEnd - m_pBufferCur;
			if (uReadSize <= uRemainSize)
				uRemainSize = uReadSize;
			if (uRemainSize > 0)
			{
				memcpy(pReadBuf, m_pBufferCur, uRemainSize);
				pReadBuf += uRemainSize;
				m_pBufferCur += uRemainSize;
				m_uFilePtr += uRemainSize;
				uReadSize -= uRemainSize;
				if (0 == uReadSize)
					break;
			}

			ret = GetReadCache();
			if (ret != ERROR_SUCCESS)
				break;
		}

		*pSize -= uReadSize;
	}
	else
	{
		memcpy(pBuf, m_pBufferCur, uReadSize);
		m_pBufferCur += uReadSize;
		m_uFilePtr += uReadSize;
	}
	return ret;
}

int AcFsIStream::SetFilePointer(Adesk::Int64 nOffset, Adesk::UInt32 method)
{
	Adesk::Int64 nSeekPos = 0;
	switch (method)
	{
	case AcDb::kSeekFromStart:
		nSeekPos = 0;
		break;
	case AcDb::kSeekFromCurrent:
		nSeekPos = m_uFilePtr;
		break;
	case AcDb::kSeekFromEnd:
		nSeekPos = m_uFileSize;
		break;
	default:
		return ERROR_INVALID_PARAMETER;
	}

	nSeekPos += nOffset;
	if (nSeekPos < 0)
		return ERROR_NEGATIVE_SEEK;

	if (nSeekPos != m_uFilePtr)
	{
		Adesk::Int64 nOffToCur = nSeekPos - (Adesk::Int64)m_uFilePtr;
		m_uFilePtr = nSeekPos;
		if (nOffToCur <= 0 || m_pBufferCur + nOffToCur >= m_pBufferEnd)
		{
			m_pBufferCur = NULL;
			m_pBufferEnd = NULL;
			m_bEndOfFile = Adesk::kTrue;
		}
		else
		{
			m_pBufferCur = m_pBufferCur + nOffToCur;
		}
	}

	return ERROR_SUCCESS;
}

int AcFsIStream::SetEndOfFile(void)
{
	int ret = ERROR_ACCESS_DENIED;
	if (m_uAcessType & GENERIC_WRITE)
	{
		ret = ERROR_SUCCESS;
		m_pBufferCur = NULL;
		m_pBufferEnd = NULL;
		m_bEndOfFile = Adesk::kTrue;
		if (m_uFilePtr < m_uFileSize)
			ret = ZeroData(m_uFilePtr, m_uFileSize);
		m_uFileSize = m_uFilePtr;
	}

	return ret;
}

int AcFsIStream::ZeroData(Adesk::UInt64 uStartPos, Adesk::UInt64 uEndPos)
{
	if (!(m_uAcessType & GENERIC_WRITE))
		return ERROR_ACCESS_DENIED;
	if (uStartPos > uEndPos)
		return ERROR_INVALID_PARAMETER;

	if (uStartPos < m_uFileSize)
	{
		AC_ASSERT_NOT_IMPLEMENTED();
	}

	return ERROR_SUCCESS;
}

int AcFsIStream::GetFilePointer(Adesk::UInt64* pPointer)
{
	if (NULL == pPointer)
		return ERROR_INVALID_PARAMETER;

	*pPointer = m_uFilePtr;
	return ERROR_SUCCESS;
}

int AcFsIStream::GetFileSize(Adesk::UInt64* pSize)
{
	if (NULL == pSize)
		return ERROR_INVALID_PARAMETER;

	*pSize = m_uFileSize;
	return ERROR_SUCCESS;
}

Adesk::UInt32 AcFsIStream::GetBlockSize(void)
{
	return m_uBlockSize;
}

void AcFsIStream::SetCompressionLevel(int level)
{
	if (level <= 9)
		m_nComprLevel = level;
}

int AcFsIStream::GetCompressionLevel(void)
{
	return m_nComprLevel;
}

int AcFsIStream::GetStreamInfo(AcFs_STREAMINFO_t* pInfo)
{
	pInfo->uFileSize = m_uFileSize;
	pInfo->nComprLevel = m_nComprLevel;
	pInfo->nComprType = m_nComprType;
	pInfo->uBlockSize = m_uBlockSize;
	pInfo->nStreamId = m_nStreamId;
	pInfo->uAppFlags = m_uAppFlags;
	ac_strcpy(pInfo->szStreamName, m_szStreamName);
	pInfo->nTotalSize = 0;
	pInfo->nComprSize = 0;

	if (m_pmnodeTable != NULL)
	{
		for (Adesk::Int32 i = 0; i < m_mnodeTblSize; ++i)
		{
			if (m_pmnodeTable[i] != NULL)
			{
				for (int j = 0; j < 256; ++j)
				{
					AcFs_mnode* pmnode = m_pmnodeTable[i] + j;
					if (pmnode->m_pMbHeader != NULL)
						pInfo->nTotalSize += pmnode->m_pMbHeader->nBlkSize;
					pInfo->nComprSize += pmnode->m_uComprSize;
				}
			}
		}
	}

	return ERROR_SUCCESS;
}

void AcFsIStream::EnableWriteCache(int enable)
{
	m_bEnableWrCache = enable != 0;
}

void AcFsIStream::SetMaxCache(int cache)
{
	if (cache < 0)
		cache = 0;
	m_uMaxCache = (Adesk::UInt32)cache;
}

int AcFsIStream::Flush(bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

const ACHAR* AcFsIStream::GetStreamName(void)
{
	return m_szStreamName;
}

int AcFsIStream::GetStreamID(void)
{
	return m_nStreamId;
}

int AcFsIStream::ScanPhysical(const void**, Adesk::UInt64*, Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsIStream::GetPhysicalAddr(Adesk::UInt64* pAddr)
{
	if (NULL == pAddr)
		return ERROR_INVALID_PARAMETER;

	Adesk::UInt64 uAddr = *pAddr;
	if (uAddr > m_uFileSize)
		return ERROR_HANDLE_EOF;

	int nDmy = 0;
	AcFs_mnode* pmnode = MapLinear(uAddr, &nDmy);
	if (NULL == pmnode)
		return ERROR_NOT_ENOUGH_MEMORY;

	*pAddr = 0;
	if (pmnode->m_pMbHeader != NULL)
	{
		*pAddr = GetBlockAddress(pmnode->m_pMbHeader);
		m_bUnk424 = true;
	}

	return ERROR_SUCCESS;
}

Adesk::Int64 AcFsIStream::GetBlockAddress(AcFs_mbheader* pHeader)
{
	AcFs_mheader* pMHdr = m_pAcFsI->getAcFsmheader();
	return pHeader->nOffset + pMHdr->getFileHeaderAddr() + pMHdr->getAlignedFileHdrSize();
}

void AcFsIStream::RegisterCallback(AcFsStreamCallBack pCallback)
{
	m_pCallback = pCallback;
}

void AcFsIStream::DefineAppFlags(Adesk::UInt32, Adesk::UInt32)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Adesk::UInt32 AcFsIStream::GetAppFlags(void)
{
	return m_uAppFlags;
}

void AcFsIStream::SetAccessType(Adesk::UInt32 type)
{
	m_uAcessType = m_pAcFsI->getAccessMode() & type;
}

void AcFsIStream::Reset(void)
{
	m_pNextStream = NULL;
	m_pPrevStream = NULL;
	m_pAcFsI = NULL;
	m_pMHeader = NULL;
	m_uFilePtr = 0;
	m_uFileSize = 0;
	m_pmnodeTable = NULL;
	m_mnodeTblSize = 0;
	m_nComprLevel = 1;
	m_nComprType = 2;
	m_nStreamId = 0;
	m_szStreamName[0] = 0;
	m_uBlockSize = 29696;
	m_pBufferCur = NULL;
	m_pBufferEnd = NULL;
	m_pLRUHead = NULL;
	m_pLRUTail = NULL;
	m_bEnableWrCache = Adesk::kTrue;
	m_bEndOfFile = Adesk::kTrue;
	m_uAcessType = 0;
	m_uMaxCache = 0x200000;
	m_uCachedSize = 0;
	m_pCallback = NULL;
	m_uAppFlags = 0;
	m_bUnk424 = false;
}

void AcFsIStream::CopyFtoM(AcFsI* pAcFsI, AcFs_mheader* pmheader, AcFs_fstream* pfstream)
{
	m_pAcFsI = pAcFsI;
	m_pMHeader = pmheader;
	m_uFileSize = pfstream->m_uFileSize;
	m_uBlockSize = pfstream->m_uBlockSize;
	m_nComprLevel = pfstream->m_nComprLevel;
	m_nComprType = pfstream->m_nComprType;
	m_nStreamId = pfstream->m_nStreamId;
	m_uAppFlags = pfstream->m_uAppFlags;
	pfstream->GetName(m_szStreamName);
}

void AcFsIStream::CopyFNtoMN(AcFs_fnode* pfnode, AcFs_mnode* pmnode)
{
	pmnode->m_pMbHeader = m_pMHeader->ConvertID(pfnode->m_blkId);
	pmnode->m_uComprSize = pfnode->m_uComprSize;
}

int AcFsIStream::ReadNodes(char** ppBytes, int count)
{
	int nRet = 0;
	if (count > 0)
	{
		int idx = 0;
		char* pBytes = *ppBytes;
		while (idx < count)
		{
			AcFs_fnode* pfnode = (AcFs_fnode*)pBytes;
			pBytes += sizeof(AcFs_fnode);
			int nDmy = 0;
			AcFs_mnode* pmnode = MapLinear(*(Adesk::Int64*)pBytes, &nDmy);
			pBytes += sizeof(Adesk::Int64);
			if (NULL == pmnode)
			{
				nRet = ERROR_NOT_ENOUGH_MEMORY;
				break;
			}

			CopyFNtoMN(pfnode, pmnode);
			++idx;
		}

		*ppBytes = pBytes;
	}
	
	return nRet;
}

// 函数计算文件指针位置（nFilePos）在哪个块内，在pBlockOffset参数中返回在块内的偏移位置
AcFs_mnode* AcFsIStream::MapLinear(Adesk::Int64 nFilePos, int* pBlockOffset)
{
	Adesk::Int64 blockIdx = nFilePos / m_uBlockSize;
	Adesk::Int64 idx1 = blockIdx / 256;
	Adesk::Int64 idx2 = blockIdx % 256;
	*pBlockOffset = nFilePos - idx1 * m_uBlockSize * 256 - idx2 * m_uBlockSize;

	if (idx1 >= m_mnodeTblSize)
	{
		AcFs_mnode** pTable = (AcFs_mnode**)malloc((m_mnodeTblSize + 256) * sizeof(AcFs_mnode*));
		for (Adesk::Int32 i = m_mnodeTblSize; i < m_mnodeTblSize + 256; ++i)
			pTable[i] = NULL;
		for (Adesk::Int32 i = 0; i < m_mnodeTblSize; ++i)
			pTable[i] = m_pmnodeTable[i];
		if (m_pmnodeTable)
			free(m_pmnodeTable);
		m_pmnodeTable = pTable;
		m_mnodeTblSize += 256;
	}

	if (NULL == m_pmnodeTable[idx1])
		m_pmnodeTable[idx1] = (AcFs_mnode*)m_pFsHeap->zalloc(sizeof(AcFs_mnode) * 256);
	AcFs_mnode* pmnode = m_pmnodeTable[idx1] + idx2;
	if (0 == pmnode->m_nStartPos)
		pmnode->m_nStartPos = blockIdx * m_uBlockSize;

	return pmnode;
}

int AcFsIStream::GetReadCache(void)
{
	if (m_uFilePtr >= m_uFileSize)
	{
		m_bEndOfFile = Adesk::kTrue;
		m_pBufferCur = NULL;
		m_pBufferEnd = NULL;
		return ERROR_HANDLE_EOF;
	}

	Adesk::Int64 blockIdx = m_uFilePtr / m_uBlockSize;
	Adesk::Int64 idx1 = blockIdx / 256;
	Adesk::Int64 idx2 = blockIdx % 256;
	Adesk::Int64 nBlockOffset = m_uFilePtr - idx1 * m_uBlockSize * 256 - idx2 * m_uBlockSize;
	if (idx1 >= m_mnodeTblSize)
	{
		m_bEndOfFile = Adesk::kTrue;
		m_pBufferCur = m_pAcFsI->getErrorTempBuff().m_pBuffer + nBlockOffset;
		goto Exit;
	}

	if (NULL == m_pmnodeTable[idx1])
	{
		m_bEndOfFile = Adesk::kTrue;
		m_pBufferCur = m_pAcFsI->getErrorTempBuff().m_pBuffer + nBlockOffset;
		goto Exit;
	}

	AcFs_mnode* pmnode = m_pmnodeTable[idx1] + idx2;
	if (NULL == pmnode->m_pBuffer)
	{
		if (pmnode->m_pMbHeader != NULL)
		{
			int ret = ReadBlock(pmnode);
			if (ret != ERROR_SUCCESS)
			{
				m_bEndOfFile = Adesk::kTrue;
				m_pBufferCur = m_pAcFsI->getErrorTempBuff().m_pBuffer + nBlockOffset;
			}
			else
			{
				m_bEndOfFile = Adesk::kFalse;
				m_pBufferCur = pmnode->m_pBuffer + nBlockOffset;
			}
		}
		else
		{
			m_bEndOfFile = Adesk::kTrue;
			m_pBufferCur = m_pAcFsI->getErrorTempBuff().m_pBuffer + nBlockOffset;
		}
	}
	else
	{
		LRUMoveToFront(pmnode);
		m_bEndOfFile = Adesk::kFalse;
		m_pBufferCur = pmnode->m_pBuffer + nBlockOffset;
	}

Exit:
	Adesk::UInt64 uRemain = m_uFileSize - m_uFilePtr;
	if (uRemain > (m_uBlockSize - nBlockOffset))
		uRemain = m_uBlockSize - nBlockOffset;
	m_pBufferEnd = m_pBufferCur + uRemain;
	return ERROR_SUCCESS;
}

int AcFsIStream::ReadBlock(AcFs_mnode* pmnode)
{
	m_pAcFsI->lock();

	int ret = LRUAddToFront(pmnode);
	if (ret != ERROR_SUCCESS)
	{
		m_pAcFsI->unlock();
		return ret;
	}

	if (NULL == pmnode->m_pMbHeader)
	{
		m_pAcFsI->unlock();
		return ERROR_SUCCESS;
	}

	AC_BYTE* pBuffer = m_pAcFsI->getOriginalFileBuff().m_pBuffer;
	ret = m_pMHeader->ReadBlock(pmnode->m_pMbHeader, pBuffer, 0, pmnode->m_uComprSize + sizeof(AcFsCodedComprHeader));
	if (ERROR_SUCCESS == ret)
	{
		Adesk::UInt32* pUInt32 = (Adesk::UInt32*)pBuffer;
		Adesk::UInt32 uMask = (Adesk::UInt32)GetBlockAddress(pmnode->m_pMbHeader) ^ BLOCKHDR_DECODE_MASK;

		for (int i = sizeof(AcFsCodedComprHeader) / sizeof(Adesk::UInt32); i > 0; --i)
			*pUInt32++ ^= uMask;

		AcFsCodedComprHeader* pComprHdr = (AcFsCodedComprHeader*)pBuffer;
		AC_BYTE* pData = pBuffer + sizeof(AcFsCodedComprHeader);
		Adesk::UInt32 uAdler32 = lzo_adler32(0, pData, pmnode->m_uComprSize);
		Adesk::UInt32 uSum = pComprHdr->uCheckSum1;
		pComprHdr->uCheckSum1 = 0;
		if (uSum != lzo_adler32(uAdler32, pBuffer, sizeof(AcFsCodedComprHeader)))
		{
			m_pAcFsI->unlock();
			return ERROR_FILE_CORRUPT;
		}
		
		if (m_pCallback != NULL)
		{
			AC_ASSERT_NOT_IMPLEMENTED();
		}
		
		if (uAdler32 != pComprHdr->uCheckSum2)
		{
			m_pAcFsI->unlock();
			return ERROR_FILE_CORRUPT;
		}

		Adesk::UInt32 uDstLen = m_uBlockSize;
		ret = m_pAcFsI->uncompress(pmnode->m_pBuffer, &uDstLen, pData, pmnode->m_uComprSize, m_nComprType);
		if (ret != ERROR_SUCCESS || m_uBlockSize != uDstLen)
		{
			m_pAcFsI->unlock();
			return ERROR_FILE_CORRUPT;
		}

		m_pAcFsI->unlock();
		return ERROR_SUCCESS;
	}

	if (pmnode->m_pBuffer)
		memset(pmnode->m_pBuffer, 0, m_uBlockSize);

	m_pAcFsI->unlock();
	return ret;
}

int AcFsIStream::WriteBlock(AcFs_mnode*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsIStream::LRUAddToFront(AcFs_mnode* pmnode)
{
	int ret = ERROR_SUCCESS;
	if (m_uCachedSize + m_uBlockSize > m_uMaxCache && m_pLRUTail != NULL)
	{
		if (m_pLRUTail->m_nUnk40)
		{
			ret = WriteBlock(m_pLRUTail);
			if (ret != ERROR_SUCCESS)
				return ret;
		}

		pmnode->m_pBuffer = m_pLRUTail->m_pBuffer;
		AcFs_mnode* pPrev = m_pLRUTail->m_pPrevNode;
		m_pLRUTail->m_pBuffer = NULL;
		m_pLRUTail->m_pPrevNode = NULL;
		m_pLRUTail = pPrev;
		if (pPrev)
			pPrev->m_pNextNode = NULL;
		else
			m_pLRUHead = NULL;
	}
	else
	{
		pmnode->m_pBuffer = (AC_BYTE*)malloc(m_uBlockSize);
		if (NULL == pmnode->m_pBuffer)
			return ERROR_NOT_ENOUGH_MEMORY;

		m_uCachedSize += m_uBlockSize;
	}

	if (NULL == pmnode->m_pMbHeader)
	{
		if (pmnode->m_pBuffer)
			memset(pmnode->m_pBuffer, 0, m_uBlockSize);
	}

	AcFs_mnode* pOldHead = m_pLRUHead;
	pmnode->m_pPrevNode = NULL;
	pmnode->m_pNextNode = m_pLRUHead;
	m_pLRUHead = pmnode;
	if (pOldHead)
		pOldHead->m_pPrevNode = pmnode;
	else
		m_pLRUTail = pmnode;

	return ERROR_SUCCESS;
}

void AcFsIStream::LRURemove(AcFs_mnode* pmnode)
{
	if (pmnode->m_pBuffer != NULL)
	{
		m_uCachedSize -= m_uBlockSize;
		if (pmnode->m_pPrevNode != NULL)
			pmnode->m_pPrevNode->m_pNextNode = pmnode->m_pNextNode;
		else
			m_pLRUHead = pmnode->m_pNextNode;
		if (pmnode->m_pNextNode != NULL)
			pmnode->m_pNextNode->m_pPrevNode = pmnode->m_pPrevNode;
		else
			m_pLRUTail = pmnode->m_pPrevNode;

		pmnode->m_pPrevNode = NULL;
		pmnode->m_pNextNode = NULL;
		free(pmnode->m_pBuffer);
		pmnode->m_pBuffer = NULL;
	}
}

void AcFsIStream::LRUMoveToFront(AcFs_mnode* pmnode)
{
	if (m_pLRUHead != pmnode)
	{
		pmnode->m_pPrevNode->m_pNextNode = pmnode->m_pNextNode;
		if (pmnode->m_pNextNode != NULL)
			pmnode->m_pNextNode->m_pPrevNode = pmnode->m_pPrevNode;
		else
			m_pLRUTail = pmnode->m_pPrevNode;

		AcFs_mnode* pTmpNode = m_pLRUHead;
		m_pLRUHead = pmnode;
		pmnode->m_pPrevNode = NULL;
		pmnode->m_pNextNode = pTmpNode;
		pTmpNode->m_pPrevNode = pmnode;
	}
}

void AcFsIStream::DeleteMemory(void)
{
	if (m_pmnodeTable != NULL)
	{
		for (Adesk::Int32 i = 0; i < m_mnodeTblSize; ++i)
		{
			if (m_pmnodeTable[i] != NULL)
			{
				for (int j = 0; j < 256; ++j)
				{
					AcFs_mnode* pmnode = m_pmnodeTable[i] + j;
					if (pmnode->m_pMbHeader != NULL)
					{
						if (pmnode->m_pBuffer != NULL)
							m_uCachedSize -= m_uBlockSize;
						if (pmnode->m_pBuffer != NULL)
						{
							free(pmnode->m_pBuffer);
							pmnode->m_pBuffer = NULL;
						}
					}
				}
				free(m_pmnodeTable[i]);
				m_pmnodeTable[i] = NULL;
			}
		}
		free(m_pmnodeTable);
		m_pmnodeTable = NULL;
	}

	m_pLRUTail = NULL;
	m_pLRUHead = NULL;
	m_uCachedSize = 0;
	if (m_pPrevStream != NULL)
	{
		m_pPrevStream->m_pNextStream = m_pNextStream;
		if (m_pNextStream != NULL)
			m_pNextStream->m_pPrevStream = m_pPrevStream;

		m_pPrevStream = NULL;
		m_pNextStream = NULL;
	}
}
