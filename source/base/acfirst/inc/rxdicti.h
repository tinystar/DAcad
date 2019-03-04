#ifndef _AC_RX_DICTI_H_
#define _AC_RX_DICTI_H_

#include "rxdict.h"
#include "rxditer.h"

class DictSkipList;
class DictArray;
class AcRxSet;
class AcRxTextString;
class AcRxImpDictionary;

class AcRxImpDictionaryIterator : public AcRxDictionaryIterator
{
public:
	ACRX_DECLARE_MEMBERS(AcRxImpDictionaryIterator);

	AcRxImpDictionaryIterator(AcRxImpDictionary* pImpDict, AcRx::DictIterType dictType);
	virtual ~AcRxImpDictionaryIterator();

public:
	virtual Adesk::Boolean done() const;
	virtual Adesk::Boolean next();
	virtual AcRxObject*    object() const;

	virtual const ACHAR*   key() const;
	virtual Adesk::UInt32  id() const;

public:
	void entryDeleted(Adesk::UInt32 id);
	void dictionaryDeleted(void);

protected:
	AcRxImpDictionary*		m_pImpDict;		// 8
	AcRxDictionaryIterator*	m_pDicIter;		// 16
	bool					m_bUnknown24;	// 24
};

class AcRxImpDictionary : public AcRxDictionary
{
	friend class AcRxImpDictionaryIterator;

public:
	ACRX_DECLARE_MEMBERS(AcRxImpDictionary);

	AcRxImpDictionary(bool bSorted, bool bIgnoreCase);
	AcRxImpDictionary(unsigned int len, bool bDelObjs, bool bSorted, bool bIgnoreCase);
	virtual ~AcRxImpDictionary();

protected:
	virtual AcRxObject*       clone() const;
	virtual Acad::ErrorStatus copyFrom(const AcRxObject* other);
	virtual Adesk::Boolean    isEqualTo(const AcRxObject * other) const;

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

public:
	virtual AcRxObject*	   at(const AcRxObject* pKey) const;					// 208//D0h
	virtual AcRxObject*    atPut(AcRxObject* pKey, AcRxObject* pObject, Adesk::UInt32* pId);	// 216//D8h

	virtual Adesk::Boolean resetKey(const ACHAR* oldKey, AcRxTextString* newKeyStr);			// 224//E0h
	virtual Adesk::Boolean resetKey(Adesk::UInt32 id, AcRxTextString* newKeyStr);				// 232//E8h

	virtual Adesk::Boolean atKeyAndIdPut(AcRxObject* pKey, Adesk::UInt32 id, AcRxObject* pObject);	// 240//F0h

	virtual AcRxObject*	   remove(const AcRxObject* pKey);						// 248//F8h

	virtual Adesk::Boolean has(const AcRxObject* pKey) const;					// 256//100h

	virtual Adesk::UInt32  idAt(const AcRxObject* pKey) const;					// 264//108h

	virtual AcRxImpDictionaryIterator resetIterator(AcRx::DictIterType type);	// 272//110h
	
protected:
	Adesk::Boolean iteratorDone(AcRxImpDictionaryIterator* pIter);

protected:
	DictSkipList*		m_pDictSkipList;		// 8
	DictArray*			m_pDictArray;			// 16
	AcRxSet*			m_pRxSet;				// 24
	bool				m_bIgnoreCase;			// 32
};

#endif // _AC_RX_DICTI_H_