#include "acfsheap.h"
#include "adesk.h"
#include <memory.h>
#include <malloc.h>


AcFsHeap* AcFsHeap::createHeap(void)
{
	return new AcFsHeap();
}

void AcFsHeap::destroyHeap(AcFsHeap* pHeap)
{
	if (pHeap != NULL)
		delete pHeap;
}

void* AcFsHeap::zalloc(size_t sz)
{
	void* pMem = malloc(sz);
	if (pMem != NULL)
		memset(pMem, 0, sz);

	return pMem;
}
