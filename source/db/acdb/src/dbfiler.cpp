#include "dbfiler.h"
#include "dbfilerctrl.h"

ACRX_NO_CONS_DEFINE_MEMBERS(AcDbDwgFiler, AcRxObject)


AcDbDwgFiler::AcDbDwgFiler()
: mController(*(new AcDbFilerController()))
{
}

AcDbDwgFiler::~AcDbDwgFiler()
{
	if (&mController != NULL)
		delete &mController;
}

Acad::ErrorStatus AcDbDwgFiler::dwgVersion(AcDb::AcDbDwgVersion& ver, AcDb::MaintenanceReleaseVersion& maintVer) const
{
	ver = AcDb::kDHL_CURRENT;
	maintVer = AcDb::kMRelease67;
	return Acad::eOk;
}

Acad::ErrorStatus AcDbDwgFiler::readAddress(void **)
{
	acrx_abort(ACRX_T("!"));
	return Acad::eOk;
}

Acad::ErrorStatus AcDbDwgFiler::writeAddress(const void *)
{
	acrx_abort(ACRX_T("!"));
	return Acad::eOk;
}

Acad::ErrorStatus AcDbDwgFiler::addReferences(AcDbIdRefQueue& /*qToAbsorb*/)
{
	return Acad::eOk;
}

bool AcDbDwgFiler::usesReferences() const
{
	return false;
}

AcDbAuditInfo * AcDbDwgFiler::getAuditInfo() const
{
	return NULL;
}

AcDbFilerController& AcDbDwgFiler::controller() const
{
	return mController;
}
