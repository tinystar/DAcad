#ifndef _AC_RX_DICT_HELPER_H_
#define _AC_RX_DICT_HELPER_H_

#include "rxditer.h"

class DictArray;
class DictSkipList;

class DictArrayIterator : public AcRxDictionaryIterator
{
public:
	DictArrayIterator(DictArray* pDicArray);
	virtual ~DictArrayIterator();

public:
	virtual Adesk::Boolean done() const;
	virtual Adesk::Boolean next();
	virtual AcRxObject*    object() const;

	virtual const ACHAR *  key() const;
	virtual Adesk::UInt32  id() const;

private:
	DictArray*		m_pDictArray;			// 8
	Adesk::UInt32	m_uCurId;				// 16
};


class DictArray
{
public:
	DictArray(unsigned int length, bool bDelVal, bool bDelKey);
	~DictArray();

public:
	void setLength(unsigned int uLen);
	AcRxObject* keyAt(Adesk::UInt32 id);
	AcRxObject* valueAt(Adesk::UInt32 id);
	AcRxObject* atPut(Adesk::UInt32 id, AcRxObject* value);
	AcRxObject* atPutKey(Adesk::UInt32 id, AcRxObject* key);
	AcRxObject* atPut(Adesk::UInt32 id, AcRxObject* key, AcRxObject* value);
	void extendArray(unsigned int uLen);
	Adesk::UInt32 add(AcRxObject* key, AcRxObject* value);
	bool has(Adesk::UInt32 id);
	AcRxObject* remove(Adesk::UInt32 id);
	unsigned int setTop(void);
	Adesk::UInt32 nbrOfEntries(void);
	DictArrayIterator* newIterator(void);
	DictArrayIterator resetIterator(void);

	unsigned int getCapacity() const { return m_uCapacity; }

public:
	bool delValue() const { return m_bDelValue; }

protected:
	struct RxObjectPair
	{
		AcRxObject* key;
		AcRxObject* value;

		RxObjectPair(AcRxObject* k = NULL, AcRxObject* v = NULL)
			: key(k)
			, value(v)
		{}
	};

	RxObjectPair*		m_pPairsArray;			// 0
	unsigned int		m_uSize;				// 8
	bool				m_bDelValue;			// 12
	bool				m_bDelKey;				// 13
	unsigned int		m_uCapacity;			// 16
};


class DSLElement
{
public:
	DSLElement(unsigned int level);
	~DSLElement();

public:
	DSLElement* getNext(unsigned int level);
	Adesk::UInt32 id(void);
	void setID(Adesk::UInt32 id);
	void setNext(unsigned int level, DSLElement* pNext);
	void setKey(AcRxObject* pKey);
	AcRxObject* key(void);

private:
	Adesk::UInt32	m_id;				// 0
	AcRxObject*		m_pKey;				// 8
	DSLElement**	m_pNexts;			// 16
};

class SkipListFinder
{
public:
	SkipListFinder(const DictSkipList*);

public:
	bool find(const AcRxObject*, bool);

	DSLElement* element(void) const;
	DSLElement* pointerList(unsigned int) const;
	void setPointerList(unsigned int, DSLElement*);
	int keyLevel(void) const;

private:
	static const size_t MAXLEVEL = 32;

	const DictSkipList*	m_pDSList;					// 0
	DSLElement*			m_pElement;					// 8
	int					m_nKeyLevel;				// 16
	DSLElement*			m_ElmPtrLists[MAXLEVEL];	// 24
};


class DictSkipListIterator : public AcRxDictionaryIterator
{
public:
	DictSkipListIterator(const DictSkipList*);
	virtual ~DictSkipListIterator();

public:
	virtual Adesk::Boolean done() const;
	virtual Adesk::Boolean next();
	virtual AcRxObject*    object() const;

	virtual const ACHAR *  key() const;
	virtual Adesk::UInt32  id() const;

protected:
	const DictSkipList*	m_pDicSkipList;		// 8
	DSLElement*			m_pCurElement;		// 16
};


class DictSkipList
{
	friend class SkipListFinder;
	friend class DictSkipListIterator;

public:
	DictSkipList();
	~DictSkipList();

public:
	Adesk::UInt32 at(const AcRxObject* pKey);
	SkipListFinder resetFinder(void) const;
	Adesk::UInt32 atPut(AcRxObject* pKey, Adesk::UInt32 id);
	bool has(const AcRxObject* pKey);
	Adesk::UInt32 remove(const AcRxObject* pKey);
	Adesk::UInt32 nbrOfEntries(void);
	DictSkipListIterator* newIterator(void);
	DictSkipListIterator resetIterator(void);

private:
	int				m_nLevel;			// 0
	int				m_nNumEntries;		// 4
	int				m_nUnknown8;		// 8
	int				m_nUnknown12;		// 12
	DSLElement*		m_pHead;			// 16
};

#endif // _AC_RX_DICT_HELPER_H_