#ifndef _AC_DB_OBJI_H_
#define _AC_DB_OBJI_H_

#include "dbmain.h"
#include "dbidmap.h"


class AcDbImpObject
{
public:
	AcDbImpObject(AcDbObject* pObj);
	virtual ~AcDbImpObject();

public:
	AcDbObject* apiObject() const { return m_pApiObj; }

public:
	AcDbObjectId objectId() const { return m_id; }
	AcDbObjectId ownerId() const;

	virtual Acad::ErrorStatus deepClone(AcDbObject* pOwnerObject,
										AcDbObject*& pClonedObject,
										AcDbIdMapping& idMap,
										Adesk::Boolean isPrimary = true) const;

	virtual Acad::ErrorStatus wblockClone(AcRxObject* pOwnerObject,
										  AcDbObject*& pClonedObject,
										  AcDbIdMapping& idMap,
										  Adesk::Boolean isPrimary = true) const;

protected:

protected:
	void*			m_pUnknown8;	// 8
	AcDbObject*		m_pApiObj;		// 16
	AcDbObjectId	m_id;			// 24
};

#endif // _AC_DB_OBJI_H_