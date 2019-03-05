#include "dbobji.h"
#include "acutmacro.h"


Acad::ErrorStatus AcDbImpObject::deepClone(AcDbObject* pOwnerObject, AcDbObject*& pClonedObject, AcDbIdMapping& idMap, Adesk::Boolean isPrimary /*= true*/) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbImpObject::wblockClone(AcRxObject* pOwnerObject, AcDbObject*& pClonedObject, AcDbIdMapping& idMap, Adesk::Boolean isPrimary /*= true*/) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}
