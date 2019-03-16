#include "AcHeapOpers.h"
#include "rxmem.h"


void* AcHeapOperators::allocRawMem(size_t size)
{
	return __acMalloc(size);
}

void* AcHeapOperators::allocRawMem(size_t size, const char* pFName, int nLine)
{
	return __acMalloc(size);
}

void AcHeapOperators::freeRawMem(void* p)
{
	__acFree(p);
}

void AcHeapOperators::freeRawMem(void* p, const char* pFName, int nLine)
{
	__acFree(p);
}
