#ifndef _ACUT_MACRO_H_
#define _ACUT_MACRO_H_

#include <assert.h>

#define AC_ASSERT(expr)					assert(expr)

#define AC_ASSERT_NOT_IMPLEMENTED()							\
	do{														\
		AC_ASSERT(false && ACRX_T("Not Implemented!!!"));	\
	} while (0)

#endif