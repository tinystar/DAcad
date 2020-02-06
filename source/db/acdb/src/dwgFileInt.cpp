#include "dwgFileInt.h"
#include "dwgFileInti.h"
#include "dbOpenSaveGlobal.h"
#include "acutmacro.h"
#include "dbSysInternals.h"
#include "dbutil.h"


Adesk::UInt32 DwgFileInt::numberOfOpenDwgFiles(void)
{
	return DwgFileImpBase::numberOfOpenDwgFiles();
}

Adesk::Boolean DwgFileInt::openForWrite(const ACHAR* pFileName, AcDb::AcDbDwgVersion ver, AcDbDatabase* pDatabase, Acad::ErrorStatus& es)
{
	DwgFileIntImp* pDwgFileInt = NULL;
	if (ver < AcDb::kDHL_1800a)
		pDwgFileInt = DwgFileImpBase::newDwgFileIntAFile();
	else
		pDwgFileInt = DwgFileImpBase::newDwgFileIntAcFs();

	AcDb::MaintenanceReleaseVersion maintRelVer = AcDb::kMRelease0;
	switch (ver)
	{
	case AcDb::kDHL_1014:	// 21
		maintRelVer = AcDb::kMRelease10;
		break;
	case AcDb::kDHL_1015:	// 23
		maintRelVer = AcDb::kMRelease15;
		break;
	case AcDb::kDHL_1800:	// 25
		maintRelVer = AcDb::kMRelease104;
		break;
	case AcDb::kDHL_1021:	// 27
		maintRelVer = AcDb::kMRelease50;
		break;
	case AcDb::kDHL_1024:	// 29
		maintRelVer = (AcDb::MaintenanceReleaseVersion)167;
		break;
	case AcDb::kDHL_1027:	// 31
		maintRelVer = AcDb::kMRelease67;
		break;
	default:
		maintRelVer = AcDb::kMRelease0;
		break;
	}

	pDwgFileInt->setValid(true);
	pDwgFileInt->setDwgVersion(ver, maintRelVer);
	pDwgFileInt->setUnk65(true);

	pDwgFileInt->attachDb(AcDbSystemInternals::getImpDatabase(pDatabase), true);	// 280
	es = pDwgFileInt->openForWrite(pFileName);		// 312
	if (es != Acad::eOk)
	{
		pDwgFileInt->detachDb(AcDbSystemInternals::getImpDatabase(pDatabase), true, true);		// 288
		delete pDwgFileInt;
		return Adesk::kFalse;
	}

	return pDwgFileInt != NULL ? Adesk::kTrue : Adesk::kFalse;
}

Adesk::Boolean DwgFileInt::open(const ACHAR* pFileName, bool bRead, Acad::ErrorStatus& es)
{
	unsigned int desiredAccess = GENERIC_READ | GENERIC_WRITE;	// 0xC0000000
	if (bRead)
		desiredAccess = GENERIC_READ;	// 0x80000000

	unsigned int shareMode = bRead ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : FILE_SHARE_READ;

	return DwgFileImpBase::openWithModes(pFileName, desiredAccess, shareMode, false, false, es, NULL) != NULL ? Adesk::kTrue : Adesk::kFalse;
}

Adesk::Boolean DwgFileInt::openDemandLoadXref(const ACHAR* pFileName, Acad::ErrorStatus& es)
{
	return DwgFileImpBase::openWithModes(pFileName, GENERIC_READ, FILE_SHARE_READ, false, false, es, NULL) != NULL ? Adesk::kTrue : Adesk::kFalse;
}

Acad::ErrorStatus DwgFileInt::copyCloseAndDelete(AcDwgFileHandle*& pDwgFileHandle, const ACHAR* pFileName)
{
	DwgFileImpBase* pDwgFileInt = (DwgFileImpBase*)pDwgFileHandle;
	const ACHAR* pOrgFileName = pDwgFileInt->getFileName();		// 32
	HANDLE hOrgFile = NULL;
	pDwgFileInt->freeData(&hOrgFile);			// 264

	int nWinErr = 0;
	HANDLE hFile = CreateFile(pFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		nWinErr = ERROR_OPEN_FAILED;
		if (int err = GetLastError())
			nWinErr = err;
	}
	else
	{
		AC_ASSERT_NOT_IMPLEMENTED();
	}
	
	AC_ASSERT_NOT_IMPLEMENTED();
	return DbUtil::win32ToAcadError(nWinErr);
}
