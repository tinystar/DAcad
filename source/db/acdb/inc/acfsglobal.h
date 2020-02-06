#ifndef _ACFS_GLOBAL_H_
#define _ACFS_GLOBAL_H_

#include <windows.h>
#include "adesk.h"

class AcFs;
class AcFsHeap;

int AcFsRecover(HANDLE, HANDLE, int, int(*)(void *, int, int), void*);

AcFsHeap* GetAcFsHeap(void);

AcFs* AcFsNewClass();
AcFs* AcFs6NewClass();
void AcFsDeleteClass(AcFs*);
void AcFs6DeleteClass(AcFs*);

int iFileSeek(HANDLE, Adesk::Int64);

#endif // _ACFS_GLOBAL_H_