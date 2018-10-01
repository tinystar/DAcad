#include "dbsymreci.h"
#include "dbsymtb.h"


Acad::ErrorStatus AcDbImpSymbolTableRecord::deepClone(AcDbObject* pOwnerObject, AcDbObject*& pClonedObject, AcDbIdMapping& idMap, Adesk::Boolean isPrimary /*= true*/) const
{

}

Acad::ErrorStatus AcDbImpSymbolTableRecord::wblockClone(AcRxObject* pOwnerObject, AcDbObject*& pClonedObject, AcDbIdMapping& idMap, Adesk::Boolean isPrimary /*= true*/) const
{
	Acad::ErrorStatus es = Acad::eOk;

	AcDbDatabase* pDestDb = NULL;
	idMap.destDb(pDestDb);
	AcDb::DeepCloneType cloneType = idMap.deepCloneContext();
	AcDb::DuplicateRecordCloning drcType = idMap.duplicateRecordCloning();
	
	if (AcDb::kDcWblkObjects == cloneType || (AcDb::kDcWblock == cloneType && drcType != AcDb::kDrcNotApplicable))
	{
		AcDbIdPair idPair(objectId(), AcDbObjectId::kNull, true);
		if (idMap.compute(idPair) && !idPair.value().isNull())
			return Acad::eOk;

		AcDbSymbolTable* pSymTbl = AcDbSymbolTable::cast(pOwnerObject);
		if (NULL == pSymTbl)
		{
			idPair.setKey(ownerId());
			idPair.setValue(AcDbObjectId::kNull);
			if (!idMap.compute(idPair) || idPair.value().isNull())
				return Acad::eInvalidOwnerObject;

			es = acdbOpenObject(pSymTbl, idPair.value(), AcDb::kForWrite);
			if (es != Acad::eOk)
			{
				// Using back door!!!
				// Todo:
				return es;
			}
		}

		bool bV84 = false;
		bool bHidden = false;

		AcRxClass* pClass = apiObject()->isA();
		if (pClass == AcDbTextStyleTableRecord::desc())
		{
			bV84 = false;	// Todo:
		}
		else if (pClass == AcDbLayerTableRecord::desc())
		{
			bHidden = ((AcDbLayerTableRecord*)apiObject())->isHidden();
		}

		if (!bV84 && !bHidden)
		{
			if (pClass != AcDbRegAppTableRecord::desc())
				idMap.duplicateRecordCloning();
		}

		// 488

		if (isPrimary)
		{

		}
	}





	return es;
}
