#include "dwgFileInt.h"
#include "dwgFileImpBase.h"
#include "dbOpenSaveGlobal.h"
#include "acutmacro.h"


Adesk::UInt32 DwgFileInt::numberOfOpenDwgFiles(void)
{
	return DwgFileImpBase::numberOfOpenDwgFiles();
}

Adesk::Boolean DwgFileInt::openForWrite(const ACHAR*, AcDb::AcDbDwgVersion ver, AcDbDatabase*, Acad::ErrorStatus&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::Boolean DwgFileInt::open(const ACHAR*, bool, Acad::ErrorStatus&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::Boolean DwgFileInt::openDemandLoadXref(const ACHAR*, Acad::ErrorStatus&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Acad::ErrorStatus DwgFileInt::copyCloseAndDelete(AcDwgFileHandle*&, const ACHAR*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}
