#include "rxclass.h"
#include "rxboiler.h"
#include "acutmacro.h"


AcRxClass* newAcRxClass(const ACHAR* className, const ACHAR* parentClassName, int proxyFlags /*= 0*/, AcRxObject* (*pseudoConstructor)() /*= NULL*/, const ACHAR* dxfName /*= NULL*/, const ACHAR* appName /*= NULL*/)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxClass* newAcRxClass(const ACHAR* className, const ACHAR* parentClassName, int dwgVer, int maintVer, int proxyFlags /*= 0*/, AcRxObject* (*pseudoConstructor)() /*= NULL*/, const ACHAR* dxfName /*= NULL*/, const ACHAR* appName /*= NULL*/, AppNameChangeFuncPtr func /*= NULL*/)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxClass* newAcRxClass(const ACHAR* className, const ACHAR* parentClassName, int dwgVer, int maintVer, int proxyFlags, AcRxObject* (*pseudoConstructor)(), const ACHAR* dxfName, const ACHAR* appName, AppNameChangeFuncPtr func, AcRxMemberCollectionConstructorPtr makeMembers, void* userData /*= NULL*/)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}


//------------------------------------------------------------------
// AcRxClass
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxClass, AcRxObject)

AcRxClass::AcRxClass()
{

}

AcRxClass::AcRxClass(const ACHAR* name, const ACHAR* parent, AcRxMemberCollectionConstructorPtr memberConstruct, void* userData /*= NULL*/)
{

}

AcRxClass::~AcRxClass()
{

}

AcRxObject* AcRxClass::addX(AcRxClass*, AcRxObject*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxClass::getX(const AcRxClass*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxClass::delX(AcRxClass*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxClass::queryX(const AcRxClass*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxClass::create()
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const ACHAR* AcRxClass::appName() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const ACHAR* AcRxClass::dxfName() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const ACHAR* AcRxClass::name() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

void AcRxClass::getClassVersion(int& dwgVer, int& maintVer) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcRxClass::proxyFlags() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

bool AcRxClass::isDerivedFrom(const AcRxClass*) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

AcRxClass* AcRxClass::myParent() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRx::Ordering AcRxClass::comparedTo(const AcRxObject*) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return AcRx::kNotOrderable;
}

AppNameChangeFuncPtr AcRxClass::appNameCallbackPtr() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

const AcRxSet* AcRxClass::descendants() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxMemberCollection* AcRxClass::members() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}
