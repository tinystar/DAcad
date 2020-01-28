#include "dbdbi.h"
#include "dbmain.h"
#include "dbHandleTable.h"
#include "acutmacro.h"
#include "dbapserv.h"
#include <windows.h>
#include "cstrop.h"
#include "dwgFileImpBase.h"
#include "dbGlobals.h"

const int _SH_DENY_AC2004 = 0x0ACAD2004;

const ACHAR* kszAdeHackString = _T("ADE hack to ignore file lock of__");

bool acdbDwkFileExists(const ACHAR* fileName)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}


AcDbImpDatabase::AcDbImpDatabase(AcDbDatabase* pDb, bool noDocument)
	: m_pDb(pDb)
	, m_header(pDb)
	, m_pHandleTable(NULL)
	, m_pGlobals(NULL)
	, m_bUnk3995(false)
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

Acad::ErrorStatus AcDbImpDatabase::readDwgFile(const ACHAR* fileName, int shmode, bool bAllowCPConversion, const wchar_t* wszPassword)
{
	if (NULL == fileName || 0 == *fileName)
		return Acad::eInvalidInput;

	Acad::ErrorStatus es = Acad::eOk;
	AcDbHostApplicationServices* pAppServices = acdbHostApplicationServices();
	ACHAR szLocalFile[MAX_PATH] = { 0 };
	if (pAppServices != NULL && pAppServices->isURL(fileName))		// 440
	{
		if ((es = pAppServices->getRemoteFile(fileName, szLocalFile)) > Acad::eInetInCache)		// 456
			return es;

		fileName = szLocalFile;
	}

	bool bNotAdeFile = ac_strncmp(fileName, kszAdeHackString, 33) != 0;
	const ACHAR* pszFileRead = fileName;
	if (!bNotAdeFile)
		pszFileRead = fileName + 33;

	unsigned int desiredAccess = 0;
	unsigned int shareMode = 0;
	if (_SH_DENYRW == shmode)
	{
		if (bNotAdeFile)
		{
			if (acdbDwkFileExists(pszFileRead))
				return Acad::eDwkLockFileFound;
		}

		desiredAccess = GENERIC_READ | GENERIC_WRITE;
	}
	else if (_SH_DENYRD == shmode)
	{
		return Acad::eInvalidInput;
	}
	else if (_SH_DENYNO == shmode)
	{
		desiredAccess = GENERIC_READ;
		shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	}
	else if (_SH_DENYWR == shmode)
	{
		desiredAccess = GENERIC_READ;
		shareMode = FILE_SHARE_READ;
	}
	else if (_SH_DENY_AC2004 == shmode)
	{
		desiredAccess = GENERIC_READ;
		shareMode = FILE_SHARE_READ;
	}
	else
	{
		return Acad::eInvalidInput;
	}

	//m_pGlobals->m_bDbLoading = true;		// 3384// 1272

	es = Acad::eNotImplementedYet;
	DwgFileImpBase* pDwgFileBase = DwgFileImpBase::openWithModes(pszFileRead, desiredAccess, shareMode, false, false, es, NULL);

	return es;
}
