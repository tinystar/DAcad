#include "rxseti.h"
#include "rxllist.h"
#include "acutmacro.h"


//------------------------------------------------------------------
// AcRxImpSetIterator
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxImpSetIterator, AcRxIterator)


AcRxImpSetIterator::AcRxImpSetIterator(const AcRxImpSet* pImpSet)
{
	m_pllistIter = new AcRxGenLinkedListIterator(pImpSet->m_pLinkedList, true);
}

AcRxImpSetIterator::~AcRxImpSetIterator()
{
	AC_SAFE_DELETE(m_pllistIter);
}

Adesk::Boolean AcRxImpSetIterator::isEqualTo(const AcRxObject* other) const
{
	if (other->isKindOf(AcRxImpSetIterator::desc()))
	{
		AcRxImpSetIterator* pOther = (AcRxImpSetIterator*)other;
		return m_pllistIter->isEqualTo(pOther->m_pllistIter);
	}

	return Adesk::kFalse;
}

Adesk::Boolean AcRxImpSetIterator::done() const
{
	return m_pllistIter->done();
}

Adesk::Boolean AcRxImpSetIterator::next()
{
	return m_pllistIter->next();
}

AcRxObject* AcRxImpSetIterator::object() const
{
	if (m_pllistIter->done())	// 64
		return NULL;
	else
		return m_pllistIter->object();	// 80
}

AcRxGenLinkedListIterator* AcRxImpSetIterator::getCurrentElement(void)
{
	return m_pllistIter;
}


//------------------------------------------------------------------
// AcRxImpSet
//------------------------------------------------------------------
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxSet, AcRxObject)
ACRX_NO_CONS_DEFINE_MEMBERS(AcRxImpSet, AcRxSet)


AcRxImpSet::AcRxImpSet()
	: m_pLinkedList(newAcRxGenLinkedList())
{
}

AcRxImpSet::~AcRxImpSet()
{
	removeAllElements();
	AC_SAFE_DELETE(m_pLinkedList);
}

void AcRxImpSet::add(AcRxObject* pElement)
{
	if (pElement != NULL)
	{
		if (!has(pElement))		// 88
			m_pLinkedList->addNewLastEntry(pElement);
	}
}

void AcRxImpSet::remove(const AcRxObject* pElement)
{
	return removeGuts(pElement, false);
}

void AcRxImpSet::removeAllElements(void)
{
	AcRxImpSetIterator iter = resetIterator();		// 128
	while (!iter.done())
		removeAtIterator(&iter);		// 144
}

bool AcRxImpSet::has(const AcRxObject* pElement) const
{
	bool bHas = false;
	if (pElement != NULL)
	{
		AcRxImpSetIterator iter = resetIterator();		// 128
		while (!iter.done())
		{
			Adesk::Boolean bEqual = pElement->isEqualTo(iter.object());	// 40
			if (bEqual)
			{
				bHas = true;
				break;
			}

			iter.next();
		}
	}

	return bHas;
}

unsigned int AcRxImpSet::nbrOfElements(void)
{
	return m_pLinkedList->nbrOfElements();
}

void AcRxImpSet::intersectWith(const AcRxSet* pOther)
{
	if (pOther != NULL)
	{
		AcRxImpSetIterator iter = resetIterator();		// 128
		while (!iter.done())
		{
			if (!pOther->has(iter.object()))		// 88
				removeAtIterator(&iter);			// 144

			iter.next();
		}
	}
	else
	{
		removeAllElements();	// 80
	}
}

void AcRxImpSet::unionWith(const AcRxSet* pOther)
{
	if (pOther != NULL)
	{
		AcRxIterator* pOtherIt = const_cast<AcRxSet*>(pOther)->newIterator();		// 120
		while (!pOtherIt->done())		// 64
		{
			add(pOtherIt->object());	// 64 // 80
			pOtherIt->next();			// 72
		}

		delete pOtherIt;
	}
}

AcRxImpSetIterator* AcRxImpSet::newIterator(void)
{
	return new AcRxImpSetIterator(this);
}

AcRxImpSetIterator AcRxImpSet::resetIterator(void) const
{
	return AcRxImpSetIterator(this);
}

void AcRxImpSet::addUnique(AcRxObject* pElement)
{
	if (pElement != NULL)
		m_pLinkedList->addNewLastEntry(pElement);
}

void AcRxImpSet::removeAtIterator(AcRxImpSetIterator* pIter)
{
	if (pIter != NULL)
		m_pLinkedList->removeAtIterator(pIter->getCurrentElement());
}

AcRxImpSetIterator* AcRxImpSet::findGuts(const AcRxObject* pElement, bool bByPtr)
{
	if (pElement != NULL)
	{
		AcRxImpSetIterator* pIter = newIterator();		// 120
		for (; !pIter->done(); pIter->next())			// 64 // 72
		{
			if (bByPtr)
			{
				if (pIter->object() == pElement)		// 80
					return pIter;
			}
			else
			{
				if (pElement->isEqualTo(pIter->object()))		// 40 // 80
					return pIter;
			}
		}

		delete pIter;
	}

	return NULL;
}

AcRxImpSetIterator* AcRxImpSet::find(const AcRxObject* pElement)
{
	return findGuts(pElement, false);
}

AcRxImpSetIterator* AcRxImpSet::findByPointer(const AcRxObject* pElement)
{
	return findGuts(pElement, true);
}

void AcRxImpSet::removeGuts(const AcRxObject* pElement, bool bByPtr)
{
	AcRxImpSetIterator* pIter = findGuts(pElement, bByPtr);
	if (pIter != NULL)
	{
		removeAtIterator(pIter);	// 144
		delete pIter;
	}
}

void AcRxImpSet::removeByPointer(const AcRxObject* pElement)
{
	return removeGuts(pElement, true);
}


//------------------------------------------------------------------
//
//------------------------------------------------------------------
AcRxSet* newAcRxSet(void)
{
	return new AcRxImpSet();
}
