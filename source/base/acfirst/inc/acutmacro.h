#ifndef _ACUT_MACRO_H_
#define _ACUT_MACRO_H_

#include <assert.h>
#include <malloc.h>

#define AC_ASSERT(expr)					assert(expr)

#define AC_ASSERT_NOT_IMPLEMENTED()							\
	do{														\
		AC_ASSERT(false && ACRX_T("Not Implemented!!!"));	\
	} while (0)


#ifndef AC_MAKE_STR
#define _AC_MAKE_STR(expr)				#expr
#define AC_MAKE_STR(expr)				_AC_MAKE_STR(expr)
#endif

#define AC_COMPILE_MSG(msg)				message("CMSG: " __FILE__ "(" AC_MAKE_STR(__LINE__) "): " msg)


#define AC_SAFE_DELETE(pointer)								\
	do{														\
		if(pointer != NULL)									\
		{													\
			delete pointer;									\
			pointer = NULL;									\
		}													\
	} while (0)

#define AC_SAFE_FREE(pointer)								\
	do{														\
		if(pointer != NULL)									\
		{													\
			free((void*)pointer);							\
			pointer = NULL;									\
		}													\
	} while (0)

#endif