#ifndef _AC_DB_SYMRECI_H_
#define _AC_DB_SYMRECI_H_

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

#endif // _AC_DB_SYMRECI_H_