#ifndef _AC_RX_MEM_H_
#define _AC_RX_MEM_H_

#include "adesk.h"

#ifdef __cplusplus
extern "C" {
#endif

	void* __acMalloc(size_t size);
	void  __acFree(void* ptr);

#ifdef __cplusplus
}
#endif

class AcBaseClassWithNew
{
#undef new
#undef delete

public:
	void* operator new(size_t size)
	{
		return __acMalloc(size);
	}

	void operator delete(void *p)
	{
		if (p != NULL)
			__acFree(p);
	}
};

#endif // _AC_RX_MEM_H_