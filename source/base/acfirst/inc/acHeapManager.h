#ifndef _AC_HEAP_MANAGER_H_
#define _AC_HEAP_MANAGER_H_

class MacSameSizePool;

typedef MacSameSizePool AcFixedSizeHeap;

#ifdef __cplusplus
extern "C" {
#endif

AcFixedSizeHeap* __acHeapCreate(size_t size);
bool __acHeapDestroy(AcFixedSizeHeap* pHeap);
void* __acHeapMalloc(AcFixedSizeHeap* pHeap);
size_t __acHeapSize(const AcFixedSizeHeap* pHeap);

void* __acMallocAligned(size_t size);
void __acFreeAligned(void* pMem);

#ifdef __cplusplus
}
#endif

#endif // _AC_HEAP_MANAGER_H_