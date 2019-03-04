#ifndef _AC_RX_LINKEDLIST_H_
#define _AC_RX_LINKEDLIST_H_

#include "rxobject.h"
#include "rxiter.h"
#include "rxmem.h"

class AcRxGenLinkedList;


class AcRxGenLinkedListElement : public AcBaseClassWithNew
{
public:
	static AcRxGenLinkedListElement* create(AcRxObject* pValue);
	static void destroy(AcRxGenLinkedListElement* pElement);

public:
	AcRxGenLinkedListElement* prev() const { return m_pPrev; }
	AcRxGenLinkedListElement* next() const { return m_pNext; }

	void setPrev(AcRxGenLinkedListElement* prev) { m_pPrev = prev; }
	void setNext(AcRxGenLinkedListElement* next) { m_pNext = next; }

	AcRxObject* value() const { return m_pValue; }

private:
	AcRxGenLinkedListElement(AcRxObject* pValue)
		: m_pPrev(NULL)
		, m_pNext(NULL)
		, m_pValue(pValue)
	{}
	~AcRxGenLinkedListElement()
	{}

	// No implementation
	explicit AcRxGenLinkedListElement(const AcRxGenLinkedListElement&);
	AcRxGenLinkedListElement& operator= (const AcRxGenLinkedListElement&);

private:
	AcRxGenLinkedListElement*	m_pNext;		// 0
	AcRxGenLinkedListElement*	m_pPrev;		// 8
	AcRxObject*					m_pValue;		// 16
};

class AcRxGenLinkedListIterator : public AcRxIterator
{
public:
	ACRX_DECLARE_MEMBERS(AcRxGenLinkedListIterator);

	AcRxGenLinkedListIterator(const AcRxGenLinkedList* pList, bool bToLast);
	AcRxGenLinkedListIterator();

	~AcRxGenLinkedListIterator();

public:
	virtual Adesk::Boolean isEqualTo(const AcRxObject* other) const;

	virtual Adesk::Boolean done() const;
	virtual Adesk::Boolean next();
	virtual AcRxObject*    object() const;

public:
	AcRxGenLinkedListElement* getCurrentEntry(void);
	void setCurrentEntry(AcRxGenLinkedListElement* pElement);
	bool goesToLast(void);

protected:
	AcRxGenLinkedListElement*	m_pElement;		// 8
	const AcRxGenLinkedList*	m_pList;		// 16
	bool						m_bToLast;		// 24
};

class AcRxGenLinkedList : public AcRxObject
{
	friend class AcRxGenLinkedListIterator;

public:
	ACRX_DECLARE_MEMBERS(AcRxGenLinkedList);

	AcRxGenLinkedList();
	~AcRxGenLinkedList();

public:
	virtual Adesk::Boolean isEqualTo(const AcRxObject* other) const;

public:
	bool removeFirstEntry(void);
	bool removeLastEntry(void);
	AcRxGenLinkedListElement* addNewFirstEntry(AcRxObject* pEntry);
	AcRxGenLinkedListElement* addNewLastEntry(AcRxObject* pEntry);
	AcRxObject* getFirstEntry(void);
	AcRxObject* getLastEntry(void);

	AcRxGenLinkedListIterator* newIterator(bool bToLast = true);
	void resetIterator(bool);
	AcRxGenLinkedListElement* addBesideIterator(AcRxGenLinkedListIterator* pIter, AcRxObject* pEntry, bool bBefore);
	AcRxGenLinkedListElement* addBeforeIterator(AcRxGenLinkedListIterator* pIter, AcRxObject* pEntry);
	AcRxGenLinkedListElement* addAfterIterator(AcRxGenLinkedListIterator* pIter, AcRxObject* pEntry);
	bool removeAtIterator(AcRxGenLinkedListIterator* pIter);

	unsigned int nbrOfElements(void) { return m_uCount; }

protected:
	AcRxGenLinkedListElement*	m_pHead;		// 8
	AcRxGenLinkedListElement*	m_pTail;		// 16
	Adesk::UInt32				m_uCount;		// 24
};


AcRxGenLinkedList* newAcRxGenLinkedList(void);

#endif // _AC_RX_LINKEDLIST_H_