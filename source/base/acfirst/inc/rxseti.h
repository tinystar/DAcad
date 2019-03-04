#ifndef _AC_RX_SETI_H_
#define _AC_RX_SETI_H_

#include "rxset.h"
#include "rxiter.h"

class AcRxImpSet;
class AcRxGenLinkedList;
class AcRxGenLinkedListIterator;

class AcRxImpSetIterator : public AcRxIterator
{
public:
	ACRX_DECLARE_MEMBERS(AcRxImpSetIterator);

	AcRxImpSetIterator(const AcRxImpSet* pImpSet);
	~AcRxImpSetIterator();

public:
	virtual Adesk::Boolean isEqualTo(const AcRxObject* other) const;

	virtual Adesk::Boolean done() const;
	virtual Adesk::Boolean next();
	virtual AcRxObject*    object() const;

public:
	AcRxGenLinkedListIterator* getCurrentElement(void);

protected:
	AcRxGenLinkedListIterator*	m_pllistIter;
};


class AcRxImpSet : public AcRxSet
{
	friend class AcRxImpSetIterator;

public:
	ACRX_DECLARE_MEMBERS(AcRxImpSet);

	AcRxImpSet();
	virtual ~AcRxImpSet();

public:
	virtual void add(AcRxObject* pElement);
	virtual void remove(const AcRxObject* pElement);
	virtual void removeAllElements(void);
	virtual bool has(const AcRxObject* pElement) const;
	virtual unsigned int nbrOfElements(void);
	virtual void intersectWith(const AcRxSet* pOther);
	virtual void unionWith(const AcRxSet* pOther);
	virtual AcRxImpSetIterator* newIterator(void);

public:
	virtual AcRxImpSetIterator resetIterator(void) const;			// 128//80h
	virtual void addUnique(AcRxObject* pElement);					// 136//88h
	virtual void removeAtIterator(AcRxImpSetIterator* pIter);		// 144//90h

protected:
	AcRxImpSetIterator* findGuts(const AcRxObject* pElement, bool bByPtr);
	AcRxImpSetIterator* find(const AcRxObject* pElement);
	AcRxImpSetIterator* findByPointer(const AcRxObject* pElement);

	void removeGuts(const AcRxObject* pElement, bool bByPtr);
	void removeByPointer(const AcRxObject* pElement);

protected:
	AcRxGenLinkedList*	m_pLinkedList;		// 8
};

#endif // _AC_RX_SETI_H_