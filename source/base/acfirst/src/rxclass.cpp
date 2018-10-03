#include "rxclass.h"
#include "rxclassi.h"
#include "rxboiler.h"
#include "acutmacro.h"


AcRxClass* newAcRxClass(const ACHAR* className, const ACHAR* parentClassName, int proxyFlags /*= 0*/, AcRxObject* (*pseudoConstructor)() /*= NULL*/, const ACHAR* dxfName /*= NULL*/, const ACHAR* appName /*= NULL*/)
{
	return AcRxImpClass::newAcRxClassWorker(className, parentClassName, 0, 0, proxyFlags, pseudoConstructor, dxfName, appName, NULL, NULL, NULL, NULL);
}

AcRxClass* newAcRxClass(const ACHAR* className, const ACHAR* parentClassName, int dwgVer, int maintVer, int proxyFlags /*= 0*/, AcRxObject* (*pseudoConstructor)() /*= NULL*/, const ACHAR* dxfName /*= NULL*/, const ACHAR* appName /*= NULL*/, AppNameChangeFuncPtr func /*= NULL*/)
{
	return AcRxImpClass::newAcRxClassWorker(className, parentClassName, dwgVer, maintVer, proxyFlags, pseudoConstructor, dxfName, appName, func, NULL, NULL, NULL);
}

AcRxClass* newAcRxClass(const ACHAR* className, const ACHAR* parentClassName, int dwgVer, int maintVer, int proxyFlags, AcRxObject* (*pseudoConstructor)(), const ACHAR* dxfName, const ACHAR* appName, AppNameChangeFuncPtr func, AcRxMemberCollectionConstructorPtr makeMembers, void* userData /*= NULL*/)
{
	return AcRxImpClass::newAcRxClassWorker(className, parentClassName, dwgVer, maintVer, proxyFlags, pseudoConstructor, dxfName, appName, func, makeMembers, userData, NULL);
}

AcRxClass* newAcRxClass(const ACHAR* className, const ACHAR* parentClassName, AcRxMemberCollectionConstructorPtr makeMembers, void* userData, AcRxClass* pExistClass)
{
	return AcRxImpClass::newAcRxClassWorker(className, parentClassName, 0, 0, 0, NULL, NULL, NULL, NULL, makeMembers, userData, pExistClass);
}


//------------------------------------------------------------------
// AcRxClass
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxClass, AcRxObject)

AcRxClass::AcRxClass()
	: m_pImp(NULL)
{

}

AcRxClass::AcRxClass(const ACHAR* name, const ACHAR* parent, AcRxMemberCollectionConstructorPtr memberConstruct, void* userData /*= NULL*/)
	: m_pImp(NULL)
{
	newAcRxClass(name, parent, memberConstruct, userData, this);
	acrxConnectNewClass(this);
}

AcRxClass::~AcRxClass()
{
	if (m_pImp)
		delete m_pImp;
}

AcRxObject* AcRxClass::addX(AcRxClass* protocolClass, AcRxObject* pPEObject)
{
	return m_pImp->addX(protocolClass, pPEObject);
}

AcRxObject* AcRxClass::getX(const AcRxClass* protocolClass)
{
	return m_pImp->getX(protocolClass);
}

AcRxObject* AcRxClass::delX(AcRxClass* protocolClass)
{
	return m_pImp->delX(protocolClass);
}

AcRxObject* AcRxClass::queryX(const AcRxClass* protocolClass)
{
	return m_pImp->queryX(protocolClass);
}

AcRxObject* AcRxClass::create()
{
	return m_pImp->create();
}

const ACHAR* AcRxClass::appName() const
{
	return m_pImp->appName();
}

const ACHAR* AcRxClass::dxfName() const
{
	return m_pImp->dxfName();
}

const ACHAR* AcRxClass::name() const
{
	return m_pImp->name();
}

void AcRxClass::getClassVersion(int& dwgVer, int& maintVer) const
{
	m_pImp->getClassVersion(dwgVer, maintVer);
}

int AcRxClass::proxyFlags() const
{
	return m_pImp->proxyFlags();
}

bool AcRxClass::isDerivedFrom(const AcRxClass* pDstClass) const
{
	return m_pImp->isDerivedFrom(pDstClass);
}

AcRxClass* AcRxClass::myParent() const
{
	return m_pImp->getParentClass();
}

AcRx::Ordering AcRxClass::comparedTo(const AcRxObject* pOther) const
{
	return m_pImp->comparedTo(pOther);
}

AppNameChangeFuncPtr AcRxClass::appNameCallbackPtr() const
{
	return m_pImp->appNameCallbackPtr();
}

const AcRxSet* AcRxClass::descendants() const
{
	return m_pImp->descendants();
}

AcRxMemberCollection* AcRxClass::members() const
{
	return m_pImp->members();
}
