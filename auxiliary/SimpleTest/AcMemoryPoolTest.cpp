#include "acFixedHeap.h"


void testFixedSizeHeap()
{
	// 	AcFixedSizeHeap* pHeap = __acHeapCreate(0x4000);
	// 
	// 	void* pMem1 = __acHeapMalloc(pHeap);
	// 	size_t size = __acHeapSize(pHeap);
	// 
	// 	pMem1 = __acHeapMalloc(pHeap);
	// 	pMem1 = __acHeapMalloc(pHeap);
	// 	pMem1 = __acHeapMalloc(pHeap);
	// 	pMem1 = __acHeapMalloc(pHeap);
	// 
	// 	__acHeapDestroy(pHeap);

	void* pMem2 = __acMallocAligned(0x4000);
	void* pMem3 = __acMallocAligned(20);

	void* pMem4 = __acMallocAligned(0x4000);
	void* pMem5 = __acMallocAligned(0x4000);
	void* pMem6 = __acMallocAligned(0x4000);

	__acFreeAligned(pMem4);
	__acFreeAligned(pMem2);

	void* pMem7 = __acMallocAligned(0x4000);
	void* pMem8 = __acMallocAligned(0x4000);
	void* pMem9 = __acMallocAligned(0x4000);

	__acFreeAligned(pMem5);
	__acFreeAligned(pMem7);
	__acFreeAligned(pMem8);
	__acFreeAligned(pMem6);

	void* pMem10 = __acMallocAligned(0x4000);
}
