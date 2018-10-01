#ifndef _AC_RX_DICTI_H_
#define _AC_RX_DICTI_H_

#include "rxdict.h"


class AcRxImpDictionary : public AcRxDictionary
{
public:
	AcRxImpDictionary();

protected:
	virtual AcRxObject*    at(const ACHAR* key) const;
	virtual AcRxObject*    at(Adesk::UInt32 id) const;

	virtual AcRxObject*    atPut(const ACHAR* key, AcRxObject* pObject,
								 Adesk::UInt32& retId);
	virtual AcRxObject*    atPut(Adesk::UInt32 id, AcRxObject* pObject);

	virtual Adesk::Boolean resetKey(const ACHAR* oldKey, const ACHAR* newKey);
	virtual Adesk::Boolean resetKey(Adesk::UInt32 id, const ACHAR* newKey);

	virtual Adesk::Boolean atKeyAndIdPut(const ACHAR* newKey, Adesk::UInt32 id,
										 AcRxObject* pObject);

	virtual AcRxObject*    remove(const ACHAR* key);
	virtual AcRxObject*    remove(Adesk::UInt32 id);

	virtual Adesk::Boolean has(const ACHAR* entryName) const;
	virtual Adesk::Boolean has(Adesk::UInt32 id) const;

	virtual Adesk::UInt32  idAt(const ACHAR* key) const;

	virtual const ACHAR*    keyAt(Adesk::UInt32 id) const;

	virtual Adesk::UInt32  numEntries() const;

	virtual AcRxDictionaryIterator* newIterator(AcRx::DictIterType type = AcRx::kDictSorted);

	virtual Adesk::Boolean deletesObjects() const;
	virtual Adesk::Boolean isCaseSensitive() const;
	virtual Adesk::Boolean isSorted() const;
};

#endif // _AC_RX_DICTI_H_