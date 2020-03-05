#include "dbfilerctrl.h"
#include "dbapserv.h"
#include "acutmacro.h"


AcDbFilerController::AcDbFilerController()
	: m_bDoCRCCheck(false)
	, m_nLInfoState(1)
	, m_uMIFCPIdx(0)
{
	AcDbHostApplicationServices* pAppServices = acdbHostApplicationServices();
	if (pAppServices != NULL)
		m_CodePageId = pAppServices->getSystemCodePage();		// 56
}

void AcDbFilerController::doCRCAccumulation(const void* pBytes, Adesk::UInt64 uBytes)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcDbFilerController::setCodePageId(code_page_id id)
{
	m_CodePageId = id;
	code_page_id sysCodePage = acdbHostApplicationServices()->getSystemCodePage();
	if (sysCodePage != id)
	{
		AC_ASSERT_NOT_IMPLEMENTED();
	}
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcDbFilerController::startCollectingLocaleInfo(void)
{
	m_uMIFCPIdx = 0;
	m_nLInfoState = 2;
}

void AcDbFilerController::stopCollectingLocaleInfo(void)
{
	m_nLInfoState = 1;
}

bool AcDbFilerController::gotLocaleInfo(void)
{
	return m_uMIFCPIdx != 0;
}

Adesk::UInt8 AcDbFilerController::getMIFCodePageIndex(void)
{
	return m_uMIFCPIdx;
}

void AcDbFilerController::setMIFCodePageIndex(Adesk::UInt8 idx)
{
	m_uMIFCPIdx = idx;
}

bool AcDbFilerController::collectingLocaleInfo(void)
{
	return m_nLInfoState == 2;
}

bool AcDbFilerController::applyingLocaleInfo(void)
{
	return m_nLInfoState == 3;
}

void AcDbFilerController::startApplyingLocaleInfo(Adesk::UInt8 idx)
{
	m_uMIFCPIdx = idx;
	m_nLInfoState = 3;
}

void AcDbFilerController::stopApplyingLocaleInfo(void)
{
	m_uMIFCPIdx = 0;
	m_nLInfoState = 1;
}
