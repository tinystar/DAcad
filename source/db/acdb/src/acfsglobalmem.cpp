#include "acfsglobalmem.h"
#include "acfsheap.h"
#include "acutmacro.h"

AcFsGlobalMemory globalMemory;


AcFsGlobalMemory::AcFsGlobalMemory()
	: m_pFsHeap(NULL)
{
	InitializeCriticalSection(&m_cs);
}

AcFsGlobalMemory::~AcFsGlobalMemory()
{
	DeleteCriticalSection(&m_cs);
}

AcFsHeap* AcFsGlobalMemory::GetHeap(void)
{
	lock();
	if (NULL == m_pFsHeap)
		m_pFsHeap = AcFsHeap::createHeap();
	unlock();
	return m_pFsHeap;
}

void AcFsGlobalMemory::IncCount(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void* AcFsGlobalMemory::GetFileThread(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

void AcFsGlobalMemory::resetThreads(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcFsGlobalMemory::DecCount(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void* AcFsGlobalMemory::GetHeapThread(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}
