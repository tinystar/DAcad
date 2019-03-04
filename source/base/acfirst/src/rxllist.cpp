#include "rxllist.h"
#include "acutmacro.h"


//------------------------------------------------------------------
// AcRxGenLinkedListElement
//------------------------------------------------------------------
AcRxGenLinkedListElement* AcRxGenLinkedListElement::create(AcRxObject* pValue)
{
	return new AcRxGenLinkedListElement(pValue);
}

void AcRxGenLinkedListElement::destroy(AcRxGenLinkedListElement* pElement)
{
	if (NULL == pElement)
		return;

	if (pElement->m_pNext != NULL)
		pElement->m_pNext->m_pPrev = pElement->m_pPrev;
	if (pElement->m_pPrev != NULL)
		pElement->m_pPrev->m_pNext = pElement->m_pNext;

	delete pElement;
}


//------------------------------------------------------------------
// AcRxGenLinkedListIterator
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxGenLinkedListIterator, AcRxIterator)

AcRxGenLinkedListIterator::AcRxGenLinkedListIterator(const AcRxGenLinkedList* pList, bool bToLast)
	: m_pList(pList)
	, m_bToLast(bToLast)
{
	if (m_bToLast)
		m_pElement = pList->m_pHead;
	else
		m_pElement = pList->m_pTail;
}

AcRxGenLinkedListIterator::AcRxGenLinkedListIterator()
	: m_pList(NULL)
	, m_bToLast(true)
	, m_pElement(NULL)
{
}

AcRxGenLinkedListIterator::~AcRxGenLinkedListIterator()
{

}

Adesk::Boolean AcRxGenLinkedListIterator::isEqualTo(const AcRxObject* other) const
{
	if (other->isKindOf(AcRxGenLinkedListIterator::desc()))
	{
		AcRxGenLinkedListIterator* pOtherIt = (AcRxGenLinkedListIterator*)other;
		return m_pElement == pOtherIt->m_pElement;		// 8
	}

	return Adesk::kFalse;
}

Adesk::Boolean AcRxGenLinkedListIterator::done() const
{
	return m_pElement == NULL;
}

Adesk::Boolean AcRxGenLinkedListIterator::next()
{
	if (m_pElement != NULL)
	{
		if (m_bToLast)
			m_pElement = m_pElement->next();
		else
			m_pElement = m_pElement->prev();
	}

	return m_pElement != NULL;
}

AcRxObject* AcRxGenLinkedListIterator::object() const
{
	if (m_pElement != NULL)
		return m_pElement->value();

	return NULL;
}

AcRxGenLinkedListElement* AcRxGenLinkedListIterator::getCurrentEntry(void)
{
	return m_pElement;
}

void AcRxGenLinkedListIterator::setCurrentEntry(AcRxGenLinkedListElement* pElement)
{
	m_pElement = pElement;
}

bool AcRxGenLinkedListIterator::goesToLast(void)
{
	return m_bToLast;
}


//------------------------------------------------------------------
// AcRxGenLinkedList
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxGenLinkedList, AcRxObject)

AcRxGenLinkedList::AcRxGenLinkedList()
	: m_pHead(NULL)
	, m_pTail(NULL)
	, m_uCount(0)
{

}

AcRxGenLinkedList::~AcRxGenLinkedList()
{
	bool bRet = false;
	do
	{
		bRet = removeFirstEntry();
	} while (bRet);
}

Adesk::Boolean AcRxGenLinkedList::isEqualTo(const AcRxObject* other) const
{
	Adesk::Boolean bEqual = Adesk::kFalse;
	if (other->isKindOf(AcRxGenLinkedList::desc()))
	{
		AcRxGenLinkedList* pOtherList = (AcRxGenLinkedList*)other;
		if (m_uCount == pOtherList->m_uCount)
		{
			bEqual = Adesk::kTrue;
			AcRxGenLinkedListIterator thisIt(this, true);
			AcRxGenLinkedListIterator otherIt(pOtherList, true);
			while (!thisIt.done())
			{
				if (thisIt.object() != otherIt.object())
				{
					bEqual = Adesk::kFalse;
					break;
				}

				thisIt.next();
				otherIt.next();
			}
		}
	}

	return bEqual;
}

bool AcRxGenLinkedList::removeFirstEntry(void)
{
	bool bRemoved = false;
	if (m_uCount > 0)
	{
		AcRxGenLinkedListElement* pNext = m_pHead->next();		// 8// 0
		AcRxGenLinkedListElement::destroy(m_pHead);
		m_pHead = pNext;										// 8
		if (NULL == pNext)
			m_pTail = NULL;										// 16

		--m_uCount;												// 24
		bRemoved = true;
	}

	return bRemoved;
}

bool AcRxGenLinkedList::removeLastEntry(void)
{
	bool bRemoved = false;
	if (m_uCount > 0)
	{
		AcRxGenLinkedListElement* pPrev = m_pTail->prev();
		AcRxGenLinkedListElement::destroy(m_pTail);
		m_pTail = pPrev;
		if (NULL == pPrev)
			m_pHead = NULL;

		--m_uCount;
		bRemoved = true;
	}

	return bRemoved;
}

AcRxGenLinkedListElement* AcRxGenLinkedList::addNewFirstEntry(AcRxObject* pEntry)
{
	AcRxGenLinkedListElement* pElement = AcRxGenLinkedListElement::create(pEntry);

	pElement->setNext(m_pHead);		// 0// 8
	if (m_pHead != NULL)
		m_pHead->setPrev(pElement);	// 8
	m_pHead = pElement;				// 8
	if (NULL == m_pTail)			// 16
		m_pTail = pElement;			// 16

	++m_uCount;						// 24

	return pElement;
}

AcRxGenLinkedListElement* AcRxGenLinkedList::addNewLastEntry(AcRxObject* pEntry)
{
	AcRxGenLinkedListElement* pElement = AcRxGenLinkedListElement::create(pEntry);

	pElement->setPrev(m_pTail);		// 8// 16
	if (m_pTail != NULL)
		m_pTail->setNext(pElement);	// 0
	m_pTail = pElement;
	if (NULL == m_pHead)			// 8
		m_pHead = pElement;

	++m_uCount;						// 24

	return pElement;
}

AcRxObject* AcRxGenLinkedList::getFirstEntry(void)
{
	AcRxObject* pEntry = NULL;
	if (m_pHead != NULL)			// 8
		pEntry = m_pHead->value();	// 16

	return pEntry;
}

AcRxObject* AcRxGenLinkedList::getLastEntry(void)
{
	AcRxObject* pEntry = NULL;
	if (m_pTail != NULL)			// 16
		pEntry = m_pTail->value();	// 16

	return pEntry;
}

AcRxGenLinkedListIterator* AcRxGenLinkedList::newIterator(bool bToLast /*= true*/)
{
	return new AcRxGenLinkedListIterator(this, bToLast);
}

void AcRxGenLinkedList::resetIterator(bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

AcRxGenLinkedListElement* AcRxGenLinkedList::addBesideIterator(AcRxGenLinkedListIterator* pIter, AcRxObject* pEntry, bool bBefore)
{
	AcRxGenLinkedListElement* pCurElm = pIter->getCurrentEntry();
	bool bToLast = pIter->goesToLast();

	if (pIter->done())	// 64
	{
		if (bToLast)
			return addNewLastEntry(pEntry);
		else
			return addNewFirstEntry(pEntry);
	}
	else
	{
		AcRxGenLinkedListElement* pRet = NULL;
		AcRxGenLinkedListElement* pNewElm = AcRxGenLinkedListElement::create(pEntry);

		if (bToLast != bBefore)
		{
			if (pCurElm == m_pTail)
				m_pTail = pNewElm;
			else
				pCurElm->next()->setPrev(pNewElm);

			pRet = pCurElm;
			pNewElm->setNext(pCurElm->next());
			pNewElm->setPrev(pCurElm);
			pCurElm->setNext(pNewElm);
		}
		else
		{
			if (pCurElm == m_pHead)
				m_pHead = pNewElm;
			else
				pCurElm->prev()->setNext(pNewElm);

			pNewElm->setNext(pCurElm);
			pRet = pCurElm->prev();
			pNewElm->setPrev(pCurElm->prev());
			pCurElm->setPrev(pNewElm);
		}
		++m_uCount;

		return pRet;
	}
}

AcRxGenLinkedListElement* AcRxGenLinkedList::addBeforeIterator(AcRxGenLinkedListIterator* pIter, AcRxObject* pEntry)
{
	return addBesideIterator(pIter, pEntry, true);
}

AcRxGenLinkedListElement* AcRxGenLinkedList::addAfterIterator(AcRxGenLinkedListIterator* pIter, AcRxObject* pEntry)
{
	return addBesideIterator(pIter, pEntry, false);
}

bool AcRxGenLinkedList::removeAtIterator(AcRxGenLinkedListIterator* pIter)
{
	bool bRemoved = false;
	if (pIter != NULL)
	{
		Adesk::Boolean bDone = pIter->done();	// 64
		if (!bDone)
		{
			AcRxGenLinkedListElement* pCurEntry = pIter->getCurrentEntry();
			pIter->next();	// 72

			if (m_pHead == pCurEntry)
				m_pHead = pCurEntry->next();
			if (m_pTail == pCurEntry)
				m_pTail = pCurEntry->prev();

			--m_uCount;
			AcRxGenLinkedListElement::destroy(pCurEntry);

			bRemoved = true;
		}
	}

	return bRemoved;
}


//------------------------------------------------------------------
//
//------------------------------------------------------------------
AcRxGenLinkedList* newAcRxGenLinkedList(void)
{
	return new AcRxGenLinkedList();
}
