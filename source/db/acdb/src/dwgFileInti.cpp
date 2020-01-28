#include "dwgFileInti.h"
#include "acutmacro.h"


DwgFileIntImp::DwgFileIntImp(void)
{

}

DwgFileIntImp::~DwgFileIntImp()
{

}

const ACHAR* DwgFileIntImp::getVersionGuid(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const ACHAR* DwgFileIntImp::getFingerPrintGuid(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const ACHAR* DwgFileIntImp::getComment(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

Acad::ErrorStatus DwgFileIntImp::saveUnknownSections(AcDbImpDatabase*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntImp::readUnknownSections(AcDbImpDatabase*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntImp::tryPassword(const wchar_t*, CIPHER*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}
