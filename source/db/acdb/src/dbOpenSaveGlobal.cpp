#include "dbOpenSaveGlobal.h"
#include "adesk.h"
#include "dbapserv.h"
#include "acutmacro.h"

ACHAR* pCurFilename = NULL;


ACHAR*& curOpenSaveFilename(void)
{
	return pCurFilename;
}

Acad::ErrorStatus getDWGTempFileName(const ACHAR*, const ACHAR*, ACHAR*)
{
	AcDbHostApplicationServices* pAppServices = acdbHostApplicationServices();
	if (NULL == pAppServices)
		return Acad::eNotApplicable;

	AcadInternalServices* pIntrServices = pAppServices->acadInternalServices();		// 432

	AC_ASSERT_NOT_IMPLEMENTED();

	return Acad::eNotImplementedYet;
}

void notifyAcDbDsDwgFilerBeingClosed(DwgFileIntImp*, bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
