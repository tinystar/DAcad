#ifndef _AC_DB_SYSTEM_INTERNALS_H_
#define _AC_DB_SYSTEM_INTERNALS_H_

#include "dbmain.h"


class AcDbSystemInternals
{
public:
	static AcDbImpDatabase* getImpDatabase(AcDbDatabase* pDb) { return pDb->mpImpDb; }
};

#endif // _AC_DB_SYSTEM_INTERNALS_H_