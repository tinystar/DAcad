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
			m_bUnk380 = Adesk::kTrue;
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
	if (m_uAcessType & 0x40000000)
	{
		ret = ERROR_SUCCESS;
		m_pBufferCur = NULL;
		m_pBufferEnd = NULL;
		m_bUnk380 = Adesk::kTrue;
		if (m_uFilePtr < m_uFileSize)
			ret = ZeroData(m_uFilePtr, m_uFileSize);
		m_uFileSize = m_uFilePtr;
	}

	return ret;
}

int AcFsIStream::ZeroData(Adesk::UInt64 uStartPos, Adesk::UInt64 uEndPos)
{
	if (!(m_uAcessType & 0x40000000))
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
	pInfo->uUnk16 = m_uFileSize;
	pInfo->nUnk28 = m_nComprLevel;
	pInfo->nUnk24 = m_nComprType;
	pInfo->uUnk32 = m_uBlockSize;
	pInfo->nUnk36 = m_nStreamId;
	pInfo->uUnk40 = m_uAppFlags;
	ac_strcpy(pInfo->szStreamName, m_szStreamName);
	pInfo->nUnk0 = 0;
	pInfo->nUnk8 = 0;

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
						pInfo->nUnk0 += pmnode->m_pMbHeader->nUnk24;
					pInfo->nUnk8 += pmnode->m_nUnk8;
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
		*pAddr = pmnode->m_pMbHeader->nUnk16 +
				 m_pAcFsI->getAcFsmheader()->getUnk260() +
				 m_pAcFsI->getAcFsmheader()->getUnk76();
		m_bUnk424 = true;
	}

	return ERROR_SUCCESS;
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
	m_bEnableWrCache = Adesk::kTrue;
	m_uAcessType = 0;
	m_uMaxCache = 0x200000;
	m_pCallback = NULL;
	m_uAppFlags = 0;
	m_pFsHeap = NULL;
}

void AcFsIStream::CopyFtoM(AcFsI* pAcFsI, AcFs_mheader* pmheader, AcFs_fstream* pfstream)
{
	m_pAcFsI = pAcFsI;
	m_pMHeader = pmheader;
	m_uFileSize = pfstream->m_nUnk0;
	m_uBlockSize = pfstream->m_nUnk12;
	m_nComprLevel = pfstream->m_nUnk16;
	m_nComprType = pfstream->m_nUnk20;
	m_nStreamId = pfstream->m_nUnk24;
	m_uAppFlags = pfstream->m_nUnk28;
	pfstream->GetName(m_szStreamName);
}

void AcFsIStream::CopyFNtoMN(AcFs_fnode* pfnode, AcFs_mnode* pmnode)
{
	pmnode->m_pMbHeader = m_pMHeader->ConvertID(pfnode->m_blkId);
	pmnode->m_nUnk8 = pfnode->m_nUnk4;
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
		m_bUnk380 = Adesk::kTrue;
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
		m_bUnk380 = Adesk::kTrue;
		m_pBufferCur = m_pAcFsI->getTempBuff464().m_pBuffer + nBlockOffset;
	}

	if (NULL == m_pmnodeTable[idx1])
	{
		m_bUnk380 = Adesk::kTrue;
		m_pBufferCur = m_pAcFsI->getTempBuff464().m_pBuffer + nBlockOffset;
	}

	AcFs_mnode* pmnode = m_pmnodeTable[idx1] + idx2;
	if (NULL == pmnode->m_pBuffer)
	{
		if (pmnode->m_pMbHeader != NULL)
		{
			int ret = ReadBlock(pmnode);
			if (ret != ERROR_SUCCESS)
			{
				m_bUnk380 = Adesk::kTrue;
				m_pBufferCur = m_pAcFsI->getTempBuff464().m_pBuffer + nBlockOffset;
			}
			else
			{
				m_bUnk380 = Adesk::kFalse;
				m_pBufferCur = pmnode->m_pBuffer + nBlockOffset;
			}
		}
		else
		{
			m_bUnk380 = Adesk::kTrue;
			m_pBufferCur = m_pAcFsI->getTempBuff464().m_pBuffer + nBlockOffset;
		}
	}
	else
	{
		LRUMoveToFront(pmnode);
		m_bUnk380 = Adesk::kFalse;
		m_pBufferCur = pmnode->m_pBuffer + nBlockOffset;
	}

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

	AC_BYTE* pBuffer = m_pAcFsI->getTempBuff488().m_pBuffer;
	ret = m_pMHeader->ReadBlock(pmnode->m_pMbHeader, pBuffer, 0, pmnode->m_nUnk8 + 32);
	if (0 == ret)
	{
		Adesk::UInt32* pUInt32 = (Adesk::UInt32*)pBuffer;
		Adesk::UInt32 uMask = (pmnode->m_pMbHeader->nUnk16 + 
							   m_pAcFsI->getAcFsmheader()->getUnk260() +
							   m_pAcFsI->getAcFsmheader()->getUnk76()) ^ 0x4164536B;


		for (int i = 8; i > 0; --i)
			*pUInt32++ ^= uMask;

		AC_BYTE* pData = pBuffer + 32;
		Adesk::UInt32 uAdler32 = lzo_adler32(0, pData, pmnode->m_nUnk8);
		Adesk::UInt32 uSum = *(Adesk::UInt32*)(pBuffer + 24);
		*(Adesk::UInt32*)(pBuffer + 24) = 0;
		if (uSum != lzo_adler32(uAdler32, pBuffer, 0x20))
		{
			m_pAcFsI->unlock();
			return ERROR_FILE_CORRUPT;
		}
		
		if (m_pCallback != NULL)
		{
			AC_ASSERT_NOT_IMPLEMENTED();
		}
		
		if (uAdler32 != *(Adesk::UInt32*)(pBuffer + 28))
		{
			m_pAcFsI->unlock();
			return ERROR_FILE_CORRUPT;
		}

		Adesk::UInt32 uDstLen = m_uBlockSize;
		ret = m_pAcFsI->uncompress(pmnode->m_pBuffer, &uDstLen, pData, pmnode->m_nUnk8, m_nComprType);
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
	if (m_uUnk392 + m_uBlockSize > m_uMaxCache && m_pMNodeUnk368 != NULL)
	{
		if (m_pMNodeUnk368->m_nUnk40)
		{
			ret = WriteBlock(m_pMNodeUnk368);
			if (ret != ERROR_SUCCESS)
				return ret;
		}

		pmnode->m_pBuffer = m_pMNodeUnk368->m_pBuffer;
		AcFs_mnode* pTmpNode = m_pMNodeUnk368->m_pUnk24;
		m_pMNodeUnk368->m_pBuffer = NULL;
		m_pMNodeUnk368->m_pUnk24 = NULL;
		m_pMNodeUnk368 = pTmpNode;
		if (pTmpNode)
			pTmpNode->m_pUnk32 = NULL;
		else
			m_pMNodeUnk360 = NULL;
	}
	else
	{
		pmnode->m_pBuffer = (AC_BYTE*)malloc(m_uBlockSize);
		if (NULL == pmnode->m_pBuffer)
			return ERROR_NOT_ENOUGH_MEMORY;

		m_uUnk392 += m_uBlockSize;
	}

	if (NULL == pmnode->m_pMbHeader)
	{
		if (pmnode->m_pBuffer)
			memset(pmnode->m_pBuffer, 0, m_uBlockSize);
	}

	AcFs_mnode* pmnode1 = m_pMNodeUnk360;
	pmnode->m_pUnk24 = NULL;
	pmnode->m_pUnk32 = m_pMNodeUnk360;
	m_pMNodeUnk360 = pmnode;
	if (pmnode1)
		pmnode1->m_pUnk24 = pmnode;
	else
		m_pMNodeUnk368 = pmnode;

	return ERROR_SUCCESS;
}

void AcFsIStream::LRURemove(AcFs_mnode* pmnode)
{
	if (pmnode->m_pBuffer != NULL)
	{
		m_uUnk392 -= m_uBlockSize;
		if (pmnode->m_pUnk24 != NULL)
			pmnode->m_pUnk24->m_pUnk32 = pmnode->m_pUnk32;
		else
			m_pMNodeUnk360 = pmnode->m_pUnk32;
		if (pmnode->m_pUnk32 != NULL)
			pmnode->m_pUnk32->m_pUnk24 = pmnode->m_pUnk24;
		else
			m_pMNodeUnk368 = pmnode->m_pUnk24;

		pmnode->m_pUnk24 = NULL;
		pmnode->m_pUnk32 = NULL;
		free(pmnode->m_pBuffer);
		pmnode->m_pBuffer = NULL;
	}
}

void AcFsIStream::LRUMoveToFront(AcFs_mnode* pmnode)
{
	if (m_pMNodeUnk360 != pmnode)
	{
		pmnode->m_pUnk24->m_pUnk32 = pmnode->m_pUnk32;
		if (pmnode->m_pUnk32 != NULL)
			pmnode->m_pUnk32->m_pUnk24 = pmnode->m_pUnk24;
		else
			m_pMNodeUnk368 = pmnode->m_pUnk24;

		AcFs_mnode* pTmpNode = m_pMNodeUnk360;
		m_pMNodeUnk360 = pmnode;
		pmnode->m_pUnk24 = NULL;
		pmnode->m_pUnk32 = pTmpNode;
		pTmpNode->m_pUnk24 = pmnode;
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
							m_uUnk392 -= m_uBlockSize;
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

	m_pMNodeUnk368 = NULL;
	m_pMNodeUnk360 = NULL;
	m_uUnk392 = 0;
	if (m_pPrevStream != NULL)
	{
		m_pPrevStream->m_pNextStream = m_pNextStream;
		if (m_pNextStream != NULL)
			m_pNextStream->m_pPrevStream = m_pPrevStream;

		m_pPrevStream = NULL;
		m_pNextStream = NULL;
	}
}
