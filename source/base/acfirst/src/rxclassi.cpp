#include "rxclassi.h"
#include "cstrop.h"
#include "rxcommon.h"
#include "rxtypecbs.h"
#include "acutmacro.h"
#include "rxditer.h"
#include "acarray.h"


static bool deletingClasses = false;

static AcRxObject* makeNothing(void)
{
	return NULL;
}

void acrxConnectNewClass(AcRxClass* pClass)
{
	AcRxImpClass* pImpClass = NULL;
	if (pClass != NULL)
		pImpClass = AcRxImpClass::getRxImpClass(pClass);		// 8
	if (pImpClass->m_pParentClsName != NULL)					// 16
	{
		AcRxObject* pParentClsObj = acrxSysClasses()->at(pImpClass->m_pParentClsName);		// 64
		pImpClass->m_pParentClass = (AcRxClass*)pParentClsObj;								// 24
		if (NULL == pParentClsObj)
		{
			if (AcRxObject::desc() != pClass)
				acrx_abort(ACRX_T("Class '%s' parent named '%s' not found."), pImpClass->m_pClassName, pImpClass->m_pParentClass);
			pParentClsObj = pImpClass->m_pParentClass;
		}
		if (pImpClass != NULL)
		{
			AC_ASSERT_NOT_IMPLEMENTED();
		}
		AC_ASSERT_NOT_IMPLEMENTED();
	}
}


//------------------------------------------------------------------
// AcRxImpClass
//------------------------------------------------------------------
AcRxClass* AcRxImpClass::newAcRxClassWorker(const ACHAR* className, 
											const ACHAR* parentClassName, 
											int dwgVer, 
											int maintVer, 
											int proxyFlags, 
											AcRxObjectPseudoConstructorPtr pObjConstructor, 
											const ACHAR* dxfName, 
											const ACHAR* appName, 
											AppNameChangeFuncPtr nameChangeFunc, 
											AcRxMemberCollectionConstructorPtr makeMembers,
											void* userData /*= NULL*/, 
											AcRxClass* pRxClass /*= NULL*/)
{
	AcRxClass* pNewClass = NULL;

	if (parentClassName != NULL &&
	   (0 == ac_strcmp(parentClassName, ACRX_T("AcDb2dPolyline")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDb3dPolyline")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbPolyfaceMesh")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbPolygonMesh")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbViewport")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbBlockBegin")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbBlockEnd")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbSequenceEnd")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbMInsertBlock")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbFaceRecord")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDb2dVertex")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDb3dPolylineVertex")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbPolyfaceMeshVertex")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbPolygonMeshVertex")) ||
		0 == ac_strcmp(parentClassName, ACRX_T("AcDbVertex")) &&
		0 != ac_strcmp(className, ACRX_T("AcDbFaceRecord")) &&
		0 != ac_strcmp(className, ACRX_T("AcDb2dVertex")) &&
		0 != ac_strcmp(className, ACRX_T("AcDb3dPolylineVertex")) &&
		0 != ac_strcmp(className, ACRX_T("AcDbPolyfaceMeshVertex")) &&
		0 != ac_strcmp(className, ACRX_T("AcDbPolygonMeshVertex"))
	   ))
	{
		acrx_abort(ACRX_T("Deriving from class %s is not allowed."), parentClassName);
	}
	else
	{
		pNewClass = pRxClass;
		if (NULL == pNewClass)
			pNewClass = new AcRxClass();

		AcRxImpClass* pImpClass = new AcRxImpClass(className,
												   parentClassName,
												   dwgVer,
												   maintVer,
												   proxyFlags,
												   pObjConstructor,
												   dxfName,
												   appName,
												   nameChangeFunc,
												   makeMembers,
												   userData,
												   pNewClass);

		pNewClass->m_pImp = pImpClass;

		AcRxObject* pExistClass = acrxSysClasses()->atPut(pNewClass->name(), pNewClass);			// 80
		if (pExistClass != NULL)
		{
			acrx_abort(ACRX_T("Attempt to register class '%s' twice."), pNewClass->name());
		}

		if (pNewClassCallback)
			pNewClassCallback(pNewClass, pNewClass->name());
	}

	return pNewClass;
}

void AcRxImpClass::delClasses(void)
{
	deletingClasses = true;
	
	//AcArray<const ACHAR*> clsNames;
	//AcRxDictionaryIterator* pIter = acrxSysClasses()->newIterator();		// 176
	//for (; !pIter->done(); pIter->next())				// 64//72
	//{
	//	AcRxObject* pObject = pIter->object();			// 80
	//	AcRxValueType* pValType = AcRxValueType::cast(pObject);
	//	if (pValType && *(pValType + 24))
	//	{
	//		const ACHAR* pClsName = pIter->key();		// 88
	//		clsNames.append(pClsName);
	//	}
	//}
	//delete pIter;

	//for (int i = 0; i < clsNames.logicalLength(); ++i)
	//{
	//	acrxSysClasses()->remove(clsNames[i]);			// 120
	//}

	delSysClasses();

	deletingClasses = false;
}

AcRxImpClass::AcRxImpClass(const ACHAR* className, 
						   const ACHAR* parentClassName, 
						   int dwgVer, 
						   int maintVer, 
						   int proxyFlags, 
						   AcRxObjectPseudoConstructorPtr pObjConstructor, 
						   const ACHAR* dxfName, 
						   const ACHAR* appName, 
						   AppNameChangeFuncPtr nameChangeFunc, 
						   AcRxMemberCollectionConstructorPtr makeMembers, 
						   void* userData, 
						   AcRxClass* pRxClass)
	: m_pApiClass(pRxClass)
	, m_pClassName(NULL)
	, m_pParentClsName(NULL)
	, m_pParentClass(NULL)
	, m_iDwgVer(dwgVer)
	, m_iMaintVer(maintVer)
	, m_iProxyFlags(proxyFlags)
	, m_pPseudoCons(NULL)
	, m_pDxfName(NULL)
	, m_pAppName(NULL)
	, m_pNameChgFun(nameChangeFunc)
	, m_pMemberCollCons(makeMembers)
	, m_pUserData(userData)
{
	AC_ASSERT(m_pClassName != NULL && *m_pClassName != 0);
	AC_ASSERT(m_pParentClsName != NULL && *m_pParentClsName != 0);

	m_pClassName = acStrdup(className);
	if (parentClassName != NULL)
		m_pParentClsName = acStrdup(parentClassName);

	m_pPseudoCons = pObjConstructor != NULL ? pObjConstructor : makeNothing;

	if (dxfName != NULL)
		m_pDxfName = acStrdup(dxfName);
	if (appName != NULL)
		m_pAppName = acStrdup(appName);
}

AcRxImpClass::~AcRxImpClass()
{
	if (!deletingClasses)
	{
		AC_ASSERT_NOT_IMPLEMENTED();
	}

	AC_SAFE_FREE(m_pClassName);			// 8
	AC_SAFE_FREE(m_pParentClsName);		// 16
	AC_SAFE_FREE(m_pDxfName);			// 56
	AC_SAFE_FREE(m_pAppName);			// 48
}

AcRxObject* AcRxImpClass::addX(AcRxClass* pClass, AcRxObject* pObject)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpClass::getX(const AcRxClass* pClass)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpClass::delX(AcRxClass* pClass)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpClass::queryX(const AcRxClass* pClass)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpClass::create()
{
	return m_pPseudoCons();
}

const ACHAR* AcRxImpClass::appName() const
{
	return m_pAppName;
}

const ACHAR* AcRxImpClass::dxfName() const
{
	return m_pDxfName;
}

const ACHAR* AcRxImpClass::name() const
{
	return m_pClassName;
}

void AcRxImpClass::getClassVersion(int& dwgVer, int& maintVer) const
{
	dwgVer = m_iDwgVer;
	maintVer = m_iMaintVer;
}

int AcRxImpClass::proxyFlags() const
{
	return m_iProxyFlags;
}

bool AcRxImpClass::isDerivedFrom(const AcRxClass* pDstClass) const
{
	bool bRet = false;
	for (AcRxClass* pThisOrParent = m_pApiClass; pThisOrParent != NULL; pThisOrParent = pThisOrParent->myParent())
	{
		if (pThisOrParent == pDstClass)
			bRet = true;
	}

	return bRet;
}

AcRx::Ordering AcRxImpClass::comparedTo(const AcRxObject* pOther) const
{
	AcRx::Ordering ret = AcRx::kEqual;

	AcRxClass* pOtherClass = AcRxClass::cast(pOther);
	const ACHAR* pOtherName = pOtherClass->name();
	int icmp = ac_strcmp(m_pClassName, pOtherName);
	if (icmp > 0)
		ret = AcRx::kGreaterThan;
	else if (icmp < 0)
		ret = AcRx::kLessThan;

	return ret;
}

AppNameChangeFuncPtr AcRxImpClass::appNameCallbackPtr() const
{
	return m_pNameChgFun;
}

const AcRxSet* AcRxImpClass::descendants() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxMemberCollection* AcRxImpClass::members() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

void AcRxImpClass::initClassPETable(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcRxImpClass::removeClassPETable(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcRxImpClass::establishPEIndex(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcRxImpClass::setPEObject(int, AcRxObject *, AcRxObject *)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
