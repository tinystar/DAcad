#include "rxdicti.h"
#include "acutmacro.h"
#include "rxdicthelper.h"
#include "rxset.h"
#include "rxtextstr.h"
#include "rxcommon.h"


ACRX_NO_CONS_DEFINE_MEMBERS(AcRxIterator, AcRxObject)
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxDictionaryIterator, AcRxIterator)

//------------------------------------------------------------------
// AcRxImpDictionaryIterator
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxImpDictionaryIterator, AcRxDictionaryIterator)


AcRxImpDictionaryIterator::AcRxImpDictionaryIterator(AcRxImpDictionary* pImpDict, AcRx::DictIterType dictType)
	: m_pImpDict(pImpDict)
	, m_bUnknown24(false)
{
	AcRx::DictIterType type = AcRx::kDictCollated;
	if (pImpDict->isSorted())	// 200
		type = dictType;

	if (AcRx::kDictCollated == type)
		m_pDicIter = pImpDict->m_pDictArray->newIterator();
	else
		m_pDicIter = pImpDict->m_pDictSkipList->newIterator();
}

AcRxImpDictionaryIterator::~AcRxImpDictionaryIterator()
{
	if (m_pImpDict != NULL)
	{
		m_pImpDict->iteratorDone(this);
		m_pImpDict = NULL;
	}

	if (m_pDicIter != NULL)
		delete m_pDicIter;
}

Adesk::Boolean AcRxImpDictionaryIterator::done() const
{
	Adesk::Boolean bDone = Adesk::kTrue;
	if (m_pImpDict != NULL)
		bDone = m_pDicIter->done();		// 64

	return bDone;
}

Adesk::Boolean AcRxImpDictionaryIterator::next()
{
	Adesk::Boolean result = Adesk::kFalse;
	if (m_bUnknown24)
	{
		m_bUnknown24 = false;
		result = Adesk::kTrue;
	}
	else
	{
		if (m_pImpDict != NULL)
		{
			if (!m_pDicIter->done())	// 64
			{
				if (!m_pDicIter->next())	// 72
				{
					result = Adesk::kTrue;
					m_pImpDict->iteratorDone(this);
					m_pImpDict = NULL;
				}
			}
		}
	}
	return result;
}

AcRxObject* AcRxImpDictionaryIterator::object() const
{
	AcRxObject* pObj = NULL;
	if (m_pImpDict != NULL)
	{
		if (!m_pDicIter->done())		// 64
		{
			pObj = m_pDicIter->object();		// 80
			if (NULL == pObj)
			{
				Adesk::UInt32 uId = id();	// 96
				pObj = m_pImpDict->at(uId);	// 72
			}
		}
	}
	return pObj;
}

const ACHAR* AcRxImpDictionaryIterator::key() const
{
	if (NULL == m_pImpDict || !m_pDicIter->done())	// 64
		return NULL;

	return m_pDicIter->key();	// 88
}

Adesk::UInt32 AcRxImpDictionaryIterator::id() const
{
	if (NULL == m_pImpDict || !m_pDicIter->done())	// 64
		return NULL;

	return m_pDicIter->id();	// 96
}

void AcRxImpDictionaryIterator::entryDeleted(Adesk::UInt32 id)
{

}

void AcRxImpDictionaryIterator::dictionaryDeleted(void)
{

}


//------------------------------------------------------------------
// AcRxImpDictionary
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxDictionary, AcRxObject)
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxImpDictionary, AcRxDictionary)


AcRxImpDictionary::AcRxImpDictionary(bool bSorted, bool bIgnoreCase)
	: m_pDictSkipList(NULL)
	, m_pDictArray(NULL)
	, m_bIgnoreCase(bIgnoreCase)
{
	if (bSorted)
		m_pDictSkipList = new DictSkipList();
	
	m_pDictArray = new DictArray(0, false, bSorted ^ 1);

	m_pRxSet = newAcRxSet();
}

AcRxImpDictionary::AcRxImpDictionary(unsigned int len, bool bDelObjs, bool bSorted, bool bIgnoreCase)
	: m_pDictSkipList(NULL)
	, m_pDictArray(NULL)
	, m_bIgnoreCase(bIgnoreCase)
{
	if (bSorted)
		m_pDictSkipList = new DictSkipList();

	m_pDictArray = new DictArray(len, bDelObjs, bSorted ^ 1);

	m_pRxSet = newAcRxSet();
}

AcRxImpDictionary::~AcRxImpDictionary()
{
	AcRxIterator* pIter = m_pRxSet->newIterator();	// 120
	while (!pIter->done())	// 64
	{
		AcRxObject* pObj = pIter->object();		// 80
		pIter->next();		// 72
		AcRxImpDictionaryIterator* pImpDicItr = AcRxImpDictionaryIterator::cast(pObj);
		pImpDicItr->dictionaryDeleted();
	}
	AC_SAFE_DELETE(pIter);
	AC_SAFE_DELETE(m_pRxSet);

	if (isSorted())		// 200
		AC_SAFE_DELETE(m_pDictSkipList);

	AC_SAFE_DELETE(m_pDictArray);
}

AcRxObject* AcRxImpDictionary::clone() const
{
	AcRxImpDictionary* pCloneDic = new AcRxImpDictionary(numEntries(), !!deletesObjects(), !!isSorted(), false);	// 168// 184// 200
	DictArrayIterator itr = m_pDictArray->resetIterator();
	while (!itr.done())
	{
		AcRxTextString* pCloneKey = new AcRxTextString(itr.key(), m_bIgnoreCase);
		AcRxObject* pCloneObj = NULL;
		if (deletesObjects())	// 184
		{
			pCloneObj = itr.object()->clone();		// 24
		}
		else
		{
			pCloneObj = itr.object();
		}

		pCloneDic->m_pDictArray->atPut(itr.id(), pCloneKey, pCloneObj);
		if (pCloneDic->isSorted())
		{
			pCloneDic->m_pDictSkipList->atPut(pCloneKey, itr.id());
		}

		itr.next();
	}

	return pCloneDic;
}

Acad::ErrorStatus AcRxImpDictionary::copyFrom(const AcRxObject* other)
{
	Acad::ErrorStatus es = Acad::eInvalidInput;

	AcRxImpDictionary* pOtherDic = AcRxImpDictionary::cast(other);
	if (pOtherDic != NULL)
	{
		if (deletesObjects() == pOtherDic->deletesObjects())	// 184// 184
		{
			AcRxDictionaryIterator* pIter = newIterator();		// 176
			while (!pIter->done())	// 64
			{
				remove(pIter->id());	// 128// 96
				pIter->next();
			}
			AC_SAFE_DELETE(pIter);

			DictArrayIterator itr = m_pDictArray->resetIterator();
			while (!itr.done())
			{
				AcRxTextString* pKeyStr = new AcRxTextString(itr.key(), m_bIgnoreCase);
				AcRxObject* pValue = NULL;
				if (deletesObjects())	// 184
					pValue = itr.object()->clone();
				else
					pValue = itr.object();

				m_pDictArray->atPut(itr.id(), pKeyStr, pValue);

				if (isSorted())		// 200
					m_pDictSkipList->atPut(pKeyStr, itr.id());

				itr.next();
			}

			es = Acad::eOk;
		}
	}

	return es;
}

Adesk::Boolean AcRxImpDictionary::isEqualTo(const AcRxObject * other) const
{
	if (!other->isKindOf(AcRxImpDictionary::desc()))
		return Adesk::kFalse;

	Adesk::Boolean result = Adesk::kFalse;
	AcRxImpDictionary* pOtherDic = (AcRxImpDictionary*)other;
	if (deletesObjects() == pOtherDic->deletesObjects())	// 184// 184
	{
		if (numEntries() == pOtherDic->numEntries())		// 168// 168
		{
			if (isSorted() == pOtherDic->isSorted())		// 200// 200
			{
				DictArrayIterator thisItr = m_pDictArray->resetIterator();
				DictArrayIterator otherItr = pOtherDic->m_pDictArray->resetIterator();
				while (true)
				{
					if (otherItr.done() || thisItr.done())
						break;

					if (thisItr.id() != otherItr.id())
						return Adesk::kFalse;

					const ACHAR* pThisKey = thisItr.key();
					const ACHAR* pOtherKey = otherItr.key();
					if (acStricmp_local(pThisKey, pOtherKey) != 0)
						return Adesk::kFalse;

					if (!thisItr.object()->isEqualTo(otherItr.object()))		// 40
						return Adesk::kFalse;

					thisItr.next();
					otherItr.next();
				}

				result = thisItr.done() && otherItr.done();
			}
		}
	}

	return result;
}

AcRxObject* AcRxImpDictionary::at(const ACHAR* key) const
{
	AcRxTextString sKey(key, m_bIgnoreCase);
	return at(&sKey);	// 208
}

AcRxObject* AcRxImpDictionary::at(Adesk::UInt32 id) const
{
	return m_pDictArray->valueAt(id);
}

AcRxObject* AcRxImpDictionary::atPut(const ACHAR* key, AcRxObject* pObject, Adesk::UInt32& retId)
{
	AcRxTextString sKey(key, m_bIgnoreCase);
	return atPut(&sKey, pObject, &retId);	// 216
}

AcRxObject* AcRxImpDictionary::atPut(Adesk::UInt32 id, AcRxObject* pObject)
{
	return m_pDictArray->atPut(id, pObject);
}

Adesk::Boolean AcRxImpDictionary::resetKey(const ACHAR* oldKey, const ACHAR* newKey)
{
	return resetKey(oldKey, new AcRxTextString(newKey, m_bIgnoreCase));		// 224
}

Adesk::Boolean AcRxImpDictionary::resetKey(Adesk::UInt32 id, const ACHAR* newKey)
{
	return resetKey(id, new AcRxTextString(newKey, m_bIgnoreCase));		// 232
}

Adesk::Boolean AcRxImpDictionary::atKeyAndIdPut(const ACHAR* newKey, Adesk::UInt32 id, AcRxObject* pObject)
{
	Adesk::Boolean bRet = Adesk::kFalse;

	AcRxObject* pObj = at(id);	// 72
	if (pObj != NULL)
	{
		AcRxTextString* pKeyStr = new AcRxTextString(newKey, m_bIgnoreCase);
		m_pDictArray->atPut(id, pKeyStr, pObject);
		if (isSorted())		// 200
			m_pDictSkipList->atPut(pKeyStr, id);

		bRet = Adesk::kTrue;
	}

	return bRet;
}

AcRxObject* AcRxImpDictionary::remove(const ACHAR* key)
{
	AcRxTextString sKey(key, m_bIgnoreCase);
	return remove(&sKey);	// 248
}

AcRxObject* AcRxImpDictionary::remove(Adesk::UInt32 id)
{
	AcRxObject* pKey = m_pDictArray->keyAt(id);
	if (pKey != NULL)
		return remove(pKey);	// 248
	else
		return NULL;
}

Adesk::Boolean AcRxImpDictionary::has(const ACHAR* entryName) const
{
	AcRxTextString sKey(entryName, m_bIgnoreCase);
	return has(&sKey);	// 256
}

Adesk::Boolean AcRxImpDictionary::has(Adesk::UInt32 id) const
{
	return m_pDictArray->has(id);
}

Adesk::UInt32 AcRxImpDictionary::idAt(const ACHAR* key) const
{
	AcRxTextString sKey(key, m_bIgnoreCase);
	return idAt(&sKey);		// 264
}

const ACHAR* AcRxImpDictionary::keyAt(Adesk::UInt32 id) const
{
	AcRxObject* pKeyObj = m_pDictArray->keyAt(id);
	AcRxTextString* pKeyStr = AcRxTextString::cast(pKeyObj);
	if (pKeyStr != NULL)
		return pKeyStr->getAsciiPtr();	// 64
	else
		return NULL;
}

Adesk::UInt32 AcRxImpDictionary::numEntries() const
{
	if (isSorted())		// 200
		return m_pDictSkipList->nbrOfEntries();
	else
		return m_pDictArray->nbrOfEntries();
}

AcRxDictionaryIterator* AcRxImpDictionary::newIterator(AcRx::DictIterType type /*= AcRx::kDictSorted*/)
{
	AcRxImpDictionaryIterator* pIter = new AcRxImpDictionaryIterator(this, type);
	m_pRxSet->add(pIter);	// 64
	return pIter;
}

Adesk::Boolean AcRxImpDictionary::deletesObjects() const
{
	return m_pDictArray->delValue();
}

Adesk::Boolean AcRxImpDictionary::isCaseSensitive() const
{
	return !m_bIgnoreCase;
}

Adesk::Boolean AcRxImpDictionary::isSorted() const
{
	return m_pDictSkipList != NULL;
}

AcRxObject* AcRxImpDictionary::at(const AcRxObject* pKey) const
{
	Adesk::UInt32 id = idAt(pKey);		// 264
	if (id)
		return at(id);		// 72
	else
		return 0;
}

AcRxObject* AcRxImpDictionary::atPut(AcRxObject* pKey, AcRxObject* pObject, Adesk::UInt32* pId)
{
	Adesk::UInt32 id = idAt(pKey);
	if (id)
	{
		if (pId != NULL)
			*pId = id;

		return atPut(id, pObject);		// 88
	}
	else
	{
		AcRxObject* pCloneKey = pKey->clone();	// 24
		id = m_pDictArray->add(pCloneKey, pObject);
		if (pId != NULL)
			*pId = id;

		if (isSorted())		// 200
			m_pDictSkipList->atPut(pCloneKey, id);
	
		return 0;
	}
}

Adesk::Boolean AcRxImpDictionary::resetKey(const ACHAR* oldKey, AcRxTextString* newKeyStr)
{
	AcRxTextString sKey(oldKey, m_bIgnoreCase);
	Adesk::UInt32 id = m_pDictSkipList->at(&sKey);
	if (0 == id)
		return Adesk::kFalse;

	m_pDictSkipList->remove(&sKey);
	m_pDictSkipList->atPut(newKeyStr, id);
	m_pDictArray->atPutKey(id, newKeyStr);
	return Adesk::kTrue;
}

Adesk::Boolean AcRxImpDictionary::resetKey(Adesk::UInt32 id, AcRxTextString* newKeyStr)
{
	AcRxObject* pKey = m_pDictArray->keyAt(id);
	if (NULL == pKey)
		return Adesk::kFalse;

	m_pDictSkipList->remove(pKey);
	m_pDictSkipList->atPut(newKeyStr, id);
	m_pDictArray->atPutKey(id, newKeyStr);
	return Adesk::kTrue;
}

Adesk::Boolean AcRxImpDictionary::atKeyAndIdPut(AcRxObject* pKey, Adesk::UInt32 id, AcRxObject* pObject)
{
	AcRxObject* pExistKey = at(id);		// 72
	if (NULL == pExistKey)
	{
		AcRxObject* pCloneKey = pKey->clone();	// 24
		m_pDictArray->atPut(id, pCloneKey);
		if (isSorted())		// 200
			m_pDictSkipList->atPut(pCloneKey, id);

		return Adesk::kTrue;
	}

	return Adesk::kFalse;
}

AcRxObject* AcRxImpDictionary::remove(const AcRxObject* pKey)
{
	Adesk::UInt32 id = idAt(pKey);		// 264
	AcRxIterator* pIter = m_pRxSet->newIterator();	// 120
	while (!pIter->done())	// 64
	{
		AcRxObject* pObj = pIter->object();		// 80
		AcRxImpDictionaryIterator* pDicIter = AcRxImpDictionaryIterator::cast(pObj);
		pIter->next();		// 72
		pDicIter->entryDeleted(id);
	}

	if (pIter != NULL)
		delete pIter;

	Adesk::UInt32 rmId = 0;
	if (isSorted())		// 200
		rmId = m_pDictSkipList->remove(pKey);
	else
		rmId = idAt(pKey);	// 264

	AcRxObject* pRemove = NULL;
	if (rmId)
		pRemove = m_pDictArray->remove(rmId);

	return pRemove;
}

Adesk::Boolean AcRxImpDictionary::has(const AcRxObject* pKey) const
{
	Adesk::Boolean bRet = Adesk::kFalse;
	if (isSorted())		// 200
		bRet = m_pDictSkipList->has(pKey);
	else
		bRet = idAt(pKey) != 0;	// 264

	return bRet;
}

Adesk::UInt32 AcRxImpDictionary::idAt(const AcRxObject* pKey) const
{
	Adesk::UInt32 id = 0;
	if (isSorted())		// 200
	{
		id = m_pDictSkipList->at(pKey);
	}
	else
	{
		AcRxTextString sKey(m_bIgnoreCase);
		DictArrayIterator itr = m_pDictArray->resetIterator();
		while (!itr.done())
		{
			sKey.cpyFromAscii(itr.key());
			Adesk::Boolean bEqual = pKey->isEqualTo(&sKey);		// 40
			if (bEqual)
			{
				id = itr.id();
				break;
			}

			itr.next();
		}
	}

	return id;
}

AcRxImpDictionaryIterator AcRxImpDictionary::resetIterator(AcRx::DictIterType type)
{
	return AcRxImpDictionaryIterator(this, type);
}

Adesk::Boolean AcRxImpDictionary::iteratorDone(AcRxImpDictionaryIterator* pIter)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}
