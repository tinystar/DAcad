#ifndef _AC_RX_CLASSI_H_
#define _AC_RX_CLASSI_H_

#include "rxclass.h"

typedef AcRxObject* (*AcRxObjectPseudoConstructorPtr)(void);


class AcRxImpClass
{
	friend void acrxConnectNewClass(AcRxClass* pClass);

private:
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
public:
	~AcRxImpClass();

private:
	static AcRxImpClass* getRxImpClass(AcRxClass* pClass) { return pClass->m_pImp; }

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
	static void delClasses(void);

public:
	void initClassPETable(void);
	void removeClassPETable(void);
	void establishPEIndex(void);
	void setPEObject(int, AcRxObject *, AcRxObject *);

public:
	AcRxObject* addX(AcRxClass* pClass, AcRxObject* pObject);
	AcRxObject* getX(const AcRxClass* pClass);
	AcRxObject* delX(AcRxClass* pClass);
	AcRxObject* queryX(const AcRxClass* pClass);

	AcRxObject* create();

	const ACHAR* appName() const;
	const ACHAR* dxfName() const;
	const ACHAR* name() const;

	void getClassVersion(int& dwgVer, int& maintVer) const;
	int proxyFlags() const;

	bool isDerivedFrom(const AcRxClass* pDstClass) const;

	AcRxClass* getParentClass() const { return m_pParentClass; }

	AcRx::Ordering comparedTo(const AcRxObject* pOther) const;

	AppNameChangeFuncPtr appNameCallbackPtr() const;

	const AcRxSet* descendants() const;
	AcRxMemberCollection* members() const;

private:
	AcRxClass*							m_pApiClass;				// 128
	const ACHAR*						m_pClassName;				// 8
	const ACHAR*						m_pParentClsName;			// 16
	AcRxClass*							m_pParentClass;				// 24
	int									m_iDwgVer;					// 68
	int									m_iMaintVer;				// 72
	int									m_iProxyFlags;				// 64
	AcRxObjectPseudoConstructorPtr		m_pPseudoCons;				// 40
	const ACHAR*						m_pDxfName;					// 56
	const ACHAR*						m_pAppName;					// 48
	AppNameChangeFuncPtr				m_pNameChgFun;				// 104
	AcRxMemberCollectionConstructorPtr	m_pMemberCollCons;			// 136
	void*								m_pUserData;				// 144
};

#endif // _AC_RX_CLASSI_H_