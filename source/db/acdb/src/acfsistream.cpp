#include "acfsistream.h"
#include "acutmacro.h"
#include <windows.h>
#include "acfsi.h"
#include "acfsnode.h"


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
	m_nComprLevel = 1;
	m_nUnk72 = 2;
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
	m_nUnk72 = pfstream->m_nUnk20;
	m_nStreamId = pfstream->m_nUnk24;
	m_uAppFlags = pfstream->m_nUnk28;
	pfstream->GetName(m_szStreamName);
}

void AcFsIStream::CopyFNtoMN(AcFs_fnode*, AcFs_mnode*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
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

AcFs_mnode* AcFsIStream::MapLinear(Adesk::Int64 unk64, int* pUnkInt)
{
	Adesk::Int64 u8 = unk64 / (m_uBlockSize << 8);
	Adesk::Int64 u5 = unk64 - (m_uBlockSize << 8) * u8;
	*pUnkInt = u5 - m_uBlockSize * (u5 / m_uBlockSize);

	if (u8 >= m_nUnk64)
	{
		v10 = malloc(8 * (m_nUnk64 + 256));
		v11 = *((_DWORD *)this + 16);
		v12 = v10;
		v13 = *((_DWORD *)this + 16);
	}


}
