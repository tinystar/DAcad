#ifndef _DB_UTIL_H_
#define _DB_UTIL_H_

#include "acadstrc.h"

class DbUtil
{
public:
	static Acad::ErrorStatus win32ToAcadError(int);
};

#endif // _DB_UTIL_H_