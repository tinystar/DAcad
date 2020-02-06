#include "dbGlobalFuncs.h"
#include "rxmem.h"


void acdbDelString(ACHAR*& pStr)
{
	if (pStr)
	{
		__acFree(pStr);
		pStr = NULL;
	}
}
