#include "rxobject.h"
#include "rxclass.h"
#include "acutmacro.h"


AcRxClass* AcRxObjectClassDesc = NULL;

void makeAcRxObjectProperties(AcRxMemberCollectionBuilder&, void*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void initAcRxObject()
{
	if (AcRxObjectClassDesc != NULL)
		acrx_abort(ACRX_T("AcRxObject class object not found."));

	AcRxObjectClassDesc = newAcRxClass(ACRX_T("AcRxObject"), NULL, 0, 0, 0, NULL, NULL, NULL, NULL, makeAcRxObjectProperties);
}


//------------------------------------------------------------------
// AcRxObject
//------------------------------------------------------------------
AcRxObject::AcRxObject()
{

}

AcRxClass* AcRxObject::desc()
{
	return AcRxObjectClassDesc;
}

AcRxObject* AcRxObject::cast(const AcRxObject * inPtr)
{
	return const_cast<AcRxObject*>(inPtr);
}

AcRxObject* AcRxObject::queryX(const AcRxClass* protocolClass) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxObject::x(const AcRxClass* protocolClass) const
{
	return isA()->getX(protocolClass);				// 16
}

AcRxClass* AcRxObject::isA() const
{
	return AcRxObjectClassDesc;
}

AcRxObject* AcRxObject::clone() const
{
	AcRxObject* pClone = isA()->create();			// 16
	if (pClone != NULL)
	{
		if (Acad::eOk != pClone->copyFrom(this))	// 32
		{
			delete pClone;							// 8
			pClone = NULL;
		}
	}

	return pClone;
}

Acad::ErrorStatus AcRxObject::copyFrom(const AcRxObject* other)
{
	acrx_abort(ACRX_T("Unimplemented %s::copyFrom() function invoked."), isA()->name());
	return Acad::eNotApplicable;
}

Adesk::Boolean AcRxObject::isEqualTo(const AcRxObject * other) const
{
	return Adesk::kFalse;
}

AcRx::Ordering AcRxObject::comparedTo(const AcRxObject * other) const
{
	return AcRx::kNotOrderable;
}

AcRxObject* AcRxObject::subQueryX(const AcRxClass* protocolClass) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}
