#ifndef __DB_SYMRECI_H__
#define __DB_SYMRECI_H__

#include "dbobji.h"

class AcDbSymbolTableRecord;

class AcDbImpSymbolTableRecord : public AcDbImpObject
{
public:
	AcDbImpSymbolTableRecord(AcDbSymbolTableRecord* pObj);
	virtual ~AcDbImpSymbolTableRecord();

public:
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

};

#endif // __DB_SYMRECI_H__