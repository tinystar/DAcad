#include "acfsistream.h"
#include "acutmacro.h"
#include <windows.h>
#include "acfsi.h"
#include "acfsnode.h"
#include "acfsheap.h"
#include "acfsmheader.h"


AcFsIStream::AcFsIStream(void)
{
	Reset();
}

AcFsIStream::~AcFsIStream()
{

}

int AcFsIStream::WriteFile(const void*, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsIStream::ReadFile(void*, Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsIStream::SetFilePointer(Adesk::Int64, Adesk::UInt32)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsIStream::SetEndOfFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFsIStream::ZeroData(Adesk::UInt64, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
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

int AcFsIStream::GetStreamInfo(AcFs_STREAMINFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFsIStream::EnableWriteCache(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
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
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
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

int AcFsIStream::GetPhysicalAddr(Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFsIStream::RegisterCallback(AcFsStreamCallBack*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
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
	m_uAcessType = 0;
	m_uMaxCache = 0x200000;
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

// ���������ļ�ָ��λ�ã�nFilePos�����ĸ����ڣ���pBlockOffset�����з����ڿ��ڵ�ƫ��λ��
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
