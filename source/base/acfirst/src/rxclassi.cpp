#include "rxclassi.h"
#include "cstrop.h"
#include "rxcommon.h"
#include "rxtypecbs.h"


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
{

}

AcRxImpClass::~AcRxImpClass()
{

}
