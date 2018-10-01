#include "rxdicti.h"
#include "acutmacro.h"

ACRX_NO_CONS_DEFINE_MEMBERS(AcRxDictionary, AcRxObject)


AcRxImpDictionary::AcRxImpDictionary()
{

}

AcRxObject* AcRxImpDictionary::at(const ACHAR* key) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpDictionary::at(Adesk::UInt32 id) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpDictionary::atPut(const ACHAR* key, AcRxObject* pObject, Adesk::UInt32& retId)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpDictionary::atPut(Adesk::UInt32 id, AcRxObject* pObject)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

Adesk::Boolean AcRxImpDictionary::resetKey(const ACHAR* oldKey, const ACHAR* newKey)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::Boolean AcRxImpDictionary::resetKey(Adesk::UInt32 id, const ACHAR* newKey)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::Boolean AcRxImpDictionary::atKeyAndIdPut(const ACHAR* newKey, Adesk::UInt32 id, AcRxObject* pObject)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

AcRxObject* AcRxImpDictionary::remove(const ACHAR* key)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

AcRxObject* AcRxImpDictionary::remove(Adesk::UInt32 id)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

Adesk::Boolean AcRxImpDictionary::has(const ACHAR* entryName) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::Boolean AcRxImpDictionary::has(Adesk::UInt32 id) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::UInt32 AcRxImpDictionary::idAt(const ACHAR* key) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

const ACHAR* AcRxImpDictionary::keyAt(Adesk::UInt32 id) const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

Adesk::UInt32 AcRxImpDictionary::numEntries() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

AcRxDictionaryIterator* AcRxImpDictionary::newIterator(AcRx::DictIterType type /*= AcRx::kDictSorted*/)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

Adesk::Boolean AcRxImpDictionary::deletesObjects() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::Boolean AcRxImpDictionary::isCaseSensitive() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

Adesk::Boolean AcRxImpDictionary::isSorted() const
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}
