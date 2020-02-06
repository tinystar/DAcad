#include "dwgFileInti.h"
#include "acutmacro.h"
#include "dbheader.h"
#include "acfsdefs.h"
#include "dbfastfiler.h"


DwgFileIntImp::DwgFileIntImp(void)
	: m_pDbHeader(NULL)
	, m_nUnk113(0)
{

}

DwgFileIntImp::~DwgFileIntImp()
{

}

const ACHAR* DwgFileIntImp::getVersionGuid(void)
{
	if (NULL == m_pDbHeader)
		readAcDbHdr();

	if (NULL == m_pDbHeader)
		return NULL;

	if (m_dwgVer1 >= AcDb::kDHL_1500)
	{
		AC_ASSERT_NOT_IMPLEMENTED();
		return NULL;
	}

	return NULL;
}

const ACHAR* DwgFileIntImp::getFingerPrintGuid(void)
{
	if (NULL == m_pDbHeader)
		readAcDbHdr();

	if (NULL == m_pDbHeader)
		return NULL;

	if (m_dwgVer1 >= AcDb::kDHL_1500)
	{
		AC_ASSERT_NOT_IMPLEMENTED();
		return NULL;
	}

	return NULL;
}

const ACHAR* DwgFileIntImp::getComment(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

Acad::ErrorStatus DwgFileIntImp::saveUnknownSections(AcDbImpDatabase*)
{
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntImp::readUnknownSections(AcDbImpDatabase*)
{
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntImp::tryPassword(const wchar_t*, CIPHER*)
{
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntImp::tryPassword(AcFs*, const wchar_t*, CIPHER*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eSecErrorDecryptingData;
}

void DwgFileIntImp::readAcDbHdr(void)
{
	if (0 == m_nUnk113)
	{
		m_nUnk113 = 1;
		if (!hasPassword())		// 152
		{
			AcDbHeader* pHeader = new AcDbHeader(NULL);
			if (Acad::eOk != startSectionRead(kSecHeader))		// 400
				delete pHeader;

			AcDbFastDwgFiler* pDwgFiler = fastDwgFiler();		// 328
			Acad::ErrorStatus es = pHeader->dwgIn(pDwgFiler);
			endSectionRead();		// 408
			if (es != Acad::eOk)
				delete pHeader;

			m_pDbHeader = pHeader;
			m_nUnk113 = 2;
		}
	}
}
