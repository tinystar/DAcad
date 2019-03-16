#include "dbmain.h"
#include "dbobji.h"
#include "acutmacro.h"


AcDbObjectId AcDbObject::objectId() const
{
	return mpImpObject->objectId();
}

void AcDbObject::assertReadEnabled() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcDbObject::assertWriteEnabled(Adesk::Boolean autoUndo /*= true*/, Adesk::Boolean recordModified /*= true*/)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcDbObject::assertNotifyEnabled() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
