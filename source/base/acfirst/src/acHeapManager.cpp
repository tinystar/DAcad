#include "acHeapManager.h"
#include <windows.h>
#include <tchar.h>
#include "acMemPool.h"


class HeapManager
{
public:
	HeapManager();
	~HeapManager();

	static void lock() { EnterCriticalSection(&criticalSection); }
	static void unlock() { LeaveCriticalSection(&criticalSection); }

	static bool freeDisabled() { return sm_bFreeDisabled; }
	static bool useAcHeap() { return sm_bUseAcHeap; }

private:
	static CRITICAL_SECTION criticalSection;
	static bool sm_bInited;
	static bool sm_bFreeDisabled;
	static bool sm_bUseAcHeap;
};

CRITICAL_SECTION HeapManager::criticalSection;
bool HeapManager::sm_bInited = false;
bool HeapManager::sm_bFreeDisabled = false;
bool HeapManager::sm_bUseAcHeap = false;


HeapManager::HeapManager()
{
	InitializeCriticalSection(&criticalSection);
	EnterCriticalSection(&criticalSection);

	sm_bInited = true;
	sm_bFreeDisabled = false;

// 	size_t var1Size, var2Size;
// 	_tgetenv_s(&var1Size, NULL, 0, _T("disableAcadHeap"));
// 	_tgetenv_s(&var2Size, NULL, 0, _T("CRTLeakReport"));
// 	if (0 == var1Size && 0 == var2Size)
		sm_bUseAcHeap = true;

	LeaveCriticalSection(&criticalSection);
}

HeapManager::~HeapManager()
{
	EnterCriticalSection(&criticalSection);
	MacSameSizePool::terminate();
	sm_bInited = false;
	LeaveCriticalSection(&criticalSection);
}


//////////////////////////////////////////////////////////////////////////

AcFixedSizeHeap* __acHeapCreate(size_t size)
{
	HeapManager::lock();
	MacSameSizePool* pPool = MacSameSizePool::newPool(size);
	HeapManager::unlock();
	return pPool;
}

bool __acHeapDestroy(AcFixedSizeHeap* pHeap)
{
	HeapManager::lock();
	if (pHeap != NULL)
		delete pHeap;
	HeapManager::unlock();
	return true;
}

void* __acHeapMalloc(AcFixedSizeHeap* pHeap)
{
	HeapManager::lock();
	void* pMem = pHeap->allocate();
	HeapManager::unlock();
	return pMem;
}

size_t __acHeapSize(const AcFixedSizeHeap* pHeap)
{
	return pHeap->size();
}

void __acFreeAligned(void* pMem)
{
	if (pMem != NULL)
	{
		MacSameSizePool* pPool = MacSameSizePool::getPoolByBlock(pMem);
		if (pPool != NULL)
		{
			if (!HeapManager::freeDisabled())
			{
				HeapManager::lock();
				pPool->release(pMem);
				HeapManager::unlock();
			}
		}
		else
		{
			free(pMem);
		}
	}
}

void* __acMallocAligned(size_t size)
{
	if (size > 0x80000000)
		return NULL;

	if (size > 0 && HeapManager::useAcHeap())
	{
		HeapManager::lock();
		MacSameSizePool* pPool = MacSameSizePool::getDefaultPool(size, true);
		void* pMem = NULL;
		if (pPool != NULL)
			pMem = pPool->allocate();
		HeapManager::unlock();

		return pMem;
	}

	return malloc(size);
}
