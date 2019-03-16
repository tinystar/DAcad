#include "dbdbi.h"
#include "dbmain.h"
#include "dbHandleTable.h"
#include "acutmacro.h"


AcDbImpDatabase::AcDbImpDatabase(AcDbDatabase* pDb, bool noDocument)
	: m_header(pDb)
	, m_pHandleTable(NULL)
{

}

AcDbImpDatabase::~AcDbImpDatabase()
{

}

Acad::ErrorStatus AcDbImpDatabase::buildDefaultDwg(bool b)
{
	m_pHandleTable = new AcDbHandleTable(m_pDb, true);
	m_bUnk3995 = b;
	return defaultTheTables(b);
}

Acad::ErrorStatus AcDbImpDatabase::defaultTheTables(bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbImpDatabase::addAcDbObject(AcDbObject* pObj)
{
	AcDbObjectId dummyId;
	return addAcDbObject(dummyId, pObj);
}

Acad::ErrorStatus AcDbImpDatabase::addAcDbObject(AcDbObjectId& objId, AcDbObject* pObj)
{
	if (NULL == pObj)
		return Acad::eNullObjectPointer;

// 	v6 = *((_QWORD *)this + 422);
// 	if (!v6
// 		|| AcDbImpLock::m_bLockingDisabled & 1
// 		|| *(_QWORD *)(v6 + 8) == AcDbImpLock::m_pCurProc
// 		|| (v7 = AcDbImpLock::slowIsMyLock((AcDbImpLock *)v6), result = 320LL, v7))
// 	{
// 		v8 = *(_QWORD *)((char *)v4 + 820);
// 		v10 = v8 + 1;
// 		AcDbHeader::setHandseed((AcDbImpDatabase *)((char *)v4 + 120), (const AcDbHandle *)&v10);
// 		v9 = v8;
// 		result = AcDbImpDatabase::addAcDbObjectAtAcDbHandle(v4, a2, v3, (const AcDbHandle *)&v9, 1u);
// 	}

	AcDbHandle oldSeed = header()->handseed();
	header()->setHandseed(oldSeed + (Adesk::ULongPtr)1);
	return addAcDbObjectAtAcDbHandle(objId, pObj, oldSeed, true);
}

Acad::ErrorStatus AcDbImpDatabase::addAcDbObjectAtAcDbHandle(AcDbObjectId& objId, AcDbObject* pObj, const AcDbHandle& handle, bool b)
{
	if (pObj != NULL)
	{
		AcRxClass* pObjClass = pObj->isA();		// 16
		if (NULL == pObjClass)
			return Acad::eNoClassId;

		if (!pObj->objectId().isNull())
			return Acad::eAlreadyInDb;

		pObj->assertWriteEnabled(Adesk::kFalse, Adesk::kFalse);
	}

	AcDbObjectId retId;
	Acad::ErrorStatus es = m_pHandleTable->getStubAtHandle(retId, handle);
	if (es != Acad::eOk)
		return es;

	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eOk;
}

Acad::ErrorStatus AcDbImpDatabase::createDefaultLayout(int, bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

void AcDbImpDatabase::fixupHeaderData(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcDbImpDatabase::sendConstructedNotification(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
