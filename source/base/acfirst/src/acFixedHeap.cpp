#include "acFixedHeap.h"
#include "acMemPool.h"


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
