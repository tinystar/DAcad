#include "dbHandleTable.h"
#include "dbSysInternals.h"
#include "dbdbi.h"
#include "acutmacro.h"


AcDbHandleTable::AcDbHandleTable(AcDbDatabase* pDb, bool)
	: m_pBelongDb(pDb)
	, m_pDbHeader(NULL)
{
}

AcDbStub* AcDbHandleTable::findStubAtHandle(const AcDbHandle&)
{
	return NULL;
}

Acad::ErrorStatus AcDbHandleTable::getStubAtHandle(AcDbObjectId&, const AcDbHandle&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbHandleTable::addHandle(AcDbStub*&, const AcDbHandle&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}
