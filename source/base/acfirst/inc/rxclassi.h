#ifndef _AC_RX_CLASSI_H_
#define _AC_RX_CLASSI_H_

#include "rxclass.h"

typedef AcRxObject* (*AcRxObjectPseudoConstructorPtr)(void);


class AcRxImpClass
{
public:
	AcRxImpClass(const ACHAR* className, 
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
				 AcRxClass* pRxClass);
	~AcRxImpClass();

public:
	static AcRxClass* newAcRxClassWorker(const ACHAR* className, 
										 const ACHAR* parentClassName, 
										 int dwgVer, 
										 int maintVer, 
										 int proxyFlags, 
										 AcRxObjectPseudoConstructorPtr pObjConstructor, 
										 const ACHAR* dxfName, 
										 const ACHAR* appName, 
										 AppNameChangeFuncPtr nameChangeFunc, 
										 AcRxMemberCollectionConstructorPtr makeMembers,
										 void* userData = NULL,
										 AcRxClass* pRxClass = NULL);
};

#endif // _AC_RX_CLASSI_H_