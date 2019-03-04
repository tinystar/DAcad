#include "rxmem.h"
#include <stdlib.h>


void* __acMalloc(size_t size)
{
	return ::malloc(size);
}

void __acFree(void* ptr)
{
	::free(ptr);
}
