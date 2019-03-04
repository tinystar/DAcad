#include "rxdicthelper.h"
#include "rxobject.h"
#include <stdlib.h>
#include <memory.h>
#include "acutmacro.h"
#include <math.h>
#include "rxtextstr.h"


//------------------------------------------------------------------
// DictArrayIterator
//------------------------------------------------------------------
DictArrayIterator::DictArrayIterator(DictArray* pDicArray)
	: m_pDictArray(NULL)
	, m_uCurId(0)
{
	m_pDictArray = pDicArray;

	while (true)
	{
		if (m_pDictArray->valueAt(++m_uCurId) || done())
			break;
	}
}

DictArrayIterator::~DictArrayIterator()
{

}

Adesk::Boolean DictArrayIterator::done() const
{
	return m_uCurId > m_pDictArray->getCapacity();
}

Adesk::Boolean DictArrayIterator::next()
{
	if (!done())	// 64
	{
		do
		{
			m_uCurId++;
		} while (m_pDictArray->valueAt(m_uCurId) != NULL && !done());
	}

	return !done();	// 64
}

AcRxObject* DictArrayIterator::object() const
{
	if (done())	// 64
		return NULL;

	return m_pDictArray->valueAt(m_uCurId);
}

const ACHAR * DictArrayIterator::key() const
{
	AcRxTextString* pKeyStr = NULL;
	if (!done() && (pKeyStr = AcRxTextString::cast(m_pDictArray->keyAt(m_uCurId))) != NULL)
		return pKeyStr->getAsciiPtr();		// 64
	
	return NULL;
}

Adesk::UInt32 DictArrayIterator::id() const
{
	if (done())			// 64
		return 0;

	return m_uCurId;	// 16
}


//------------------------------------------------------------------
// DictArray
//------------------------------------------------------------------
DictArray::DictArray(unsigned int length, bool bDelVal, bool bDelKey)
	: m_uSize(0)
	, m_uCapacity(0)
	, m_bDelValue(bDelVal)
	, m_bDelKey(bDelKey)
{
	setLength(length);
}

DictArray::~DictArray()
{
	setLength(0);
}

void DictArray::setLength(unsigned int uLen)
{
	if (uLen > 0)
	{
		if (m_uSize != uLen)
		{
			if (m_uSize > 0)
			{
				if (m_uCapacity > uLen)
				{
					for (unsigned int idx = uLen; idx < m_uCapacity; ++idx)
					{
						RxObjectPair& item = m_pPairsArray[idx];

						if (item.value)
						{
							if (m_bDelValue)
								delete item.value;
							else
								item.value = NULL;
						}
						if (item.key)
						{
							if (m_bDelKey)
								delete item.key;
							else
								item.key = NULL;
						}
					}
					setTop();
				}

				m_pPairsArray = (RxObjectPair*)realloc(m_pPairsArray, sizeof(RxObjectPair) * uLen);
				if (NULL == m_pPairsArray)
					acrx_abort(ACRX_T("Out of memory in DictArray.\n"));

				if (m_uSize < uLen)
					memset(m_pPairsArray + m_uSize, 0, (uLen - m_uSize) * sizeof(RxObjectPair));
			}
			else
			{
				m_pPairsArray = (RxObjectPair*)calloc(uLen, sizeof(RxObjectPair));
				if (NULL == m_pPairsArray)
					acrx_abort(ACRX_T("Out of memory in DictArray.\n"));
			}
			m_uSize = uLen;
		}
	}
	else
	{
		if (m_uCapacity > 0)
		{
			if (m_bDelValue || m_bDelKey)
			{
				for (unsigned int idx = 0; idx < m_uCapacity; ++idx)
				{
					RxObjectPair& item = m_pPairsArray[idx];
					if (m_bDelValue)
					{
						if (item.value)
							delete item.value;
					}
					if (m_bDelKey)
					{
						if (item.value && item.key)
							delete item.key;
					}
				}
			}
		}
		if (m_uSize > 0)
			free(m_pPairsArray);
		m_uSize = 0;
		m_uCapacity = 0;
	}
}

AcRxObject* DictArray::keyAt(Adesk::UInt32 id)
{
	if (m_uCapacity >= id)
		return m_pPairsArray[id - 1].key;
	else
		return NULL;
}

AcRxObject* DictArray::valueAt(Adesk::UInt32 id)
{
	if (m_uCapacity >= id)
		return m_pPairsArray[id - 1].value;
	else
		return NULL;
}

AcRxObject* DictArray::atPut(Adesk::UInt32 id, AcRxObject* value)
{
	if (0 == id || m_uCapacity < id)
		acrx_abort(ACRX_T("Invalid Dictionary Array Index: %d"), id);

	AcRxObject* pOld = m_pPairsArray[id - 1].value;
	m_pPairsArray[id - 1].value = value;
	if (m_bDelValue)
	{
		AC_SAFE_DELETE(pOld);
	}

	return pOld;
}

AcRxObject* DictArray::atPut(Adesk::UInt32 id, AcRxObject* key, AcRxObject* value)
{
	if (m_uSize < id)
		extendArray(id);
	
	if (m_uCapacity < id)
		m_uCapacity = id;

	RxObjectPair oldPair = m_pPairsArray[id - 1];
	m_pPairsArray[id - 1].key = key;
	m_pPairsArray[id - 1].value = value;

	if (m_bDelKey && oldPair.key != NULL)
		delete oldPair.key;

	if (oldPair.value != NULL && m_bDelValue)
	{
		AC_SAFE_DELETE(oldPair.value);
	}

	return oldPair.value;
}

AcRxObject* DictArray::atPutKey(Adesk::UInt32 id, AcRxObject* key)
{
	if (0 == id || m_uCapacity < id)
		acrx_abort(ACRX_T("Invalid Dictionary Array Index: %d"), id);

	AcRxObject* pOld = m_pPairsArray[id - 1].key;
	m_pPairsArray[id - 1].key = key;
	return pOld;
}

void DictArray::extendArray(unsigned int uLen)
{
	unsigned int uLenEx = (unsigned int)floor((double)uLen * 1.1);
	if (uLenEx > m_uSize)
	{
		unsigned int uNewLen = m_uSize + 5;
		if ((uLenEx - m_uSize) >= 5)
			uNewLen = uLenEx;
		setLength(uNewLen);
	}
}

Adesk::UInt32 DictArray::add(AcRxObject* key, AcRxObject* value)
{
	m_uCapacity += 1;
	atPut(m_uCapacity, key, value);
	return m_uCapacity;
}

bool DictArray::has(Adesk::UInt32 id)
{
	bool bRet = false;
	if (m_uSize >= id)
		bRet = m_pPairsArray[id - 1].value != NULL;
	return bRet;
}

AcRxObject* DictArray::remove(Adesk::UInt32 id)
{
	AcRxObject* pKey = m_pPairsArray[id - 1].key;
	AcRxObject* pVal = m_pPairsArray[id - 1].value;
	m_pPairsArray[id - 1].key = m_pPairsArray[id - 1].value = NULL;
	if (m_uCapacity == id)
		setTop();
	if (m_bDelKey && pKey)
		delete pKey;

	AcRxObject* pRemoveVal = NULL;
	if (NULL == pVal || !m_bDelValue)
		pRemoveVal = pVal;
	if (pVal != NULL && m_bDelValue)
	{
		delete pVal;
		pRemoveVal = NULL;
	}

	return pRemoveVal;
}

unsigned int DictArray::setTop(void)
{
	if (m_uSize > 0)
	{
		for (unsigned int i = m_uSize - 1; i >= 0; --i)
		{
			if (m_pPairsArray[i].value != NULL)
			{
				m_uCapacity = i + 1;
				return m_uCapacity;
			}
		}
	}

	m_uCapacity = 0;
	return m_uCapacity; // ????
}

Adesk::UInt32 DictArray::nbrOfEntries(void)
{
	Adesk::UInt32 uCount = 0;
	if (m_uCapacity > 0)
	{
		unsigned int uIdx = 0;
		do
		{
			uCount += m_pPairsArray[uIdx].value != NULL;
			++uIdx;
		} while (uIdx < m_uCapacity);
	}

	return uCount;
}

DictArrayIterator* DictArray::newIterator(void)
{
	return new DictArrayIterator(this);
}

DictArrayIterator DictArray::resetIterator(void)
{
	return DictArrayIterator(this);
}


//------------------------------------------------------------------
// DSLElement
//------------------------------------------------------------------
DSLElement::DSLElement(unsigned int level)
	: m_id(0)
	, m_pKey(NULL)
	, m_pNexts(NULL)
{
	m_pNexts = (DSLElement**)malloc(level * sizeof(DSLElement*));
	if (level > 0)
	{
		for (unsigned int i = 0; i < level; ++i)
			m_pNexts[i] = NULL;
	}
}

DSLElement::~DSLElement()
{
	AC_SAFE_DELETE(m_pKey);
	free(m_pNexts);
}

DSLElement* DSLElement::getNext(unsigned int level)
{
	return m_pNexts[level];
}

Adesk::UInt32 DSLElement::id(void)
{
	return m_id;
}

void DSLElement::setID(Adesk::UInt32 id)
{
	m_id = id;
}

void DSLElement::setNext(unsigned int level, DSLElement* pNext)
{
	m_pNexts[level] = pNext;
}

void DSLElement::setKey(AcRxObject* pKey)
{
	m_pKey = pKey;
}

AcRxObject* DSLElement::key(void)
{
	return m_pKey;
}


//------------------------------------------------------------------
// SkipListFinder
//------------------------------------------------------------------
SkipListFinder::SkipListFinder(const DictSkipList* pList)
	: m_pDSList(pList)
	, m_pElement(NULL)
	, m_nKeyLevel(0)
{
	memset(m_ElmPtrLists, 0, sizeof(m_ElmPtrLists));
}

bool SkipListFinder::find(const AcRxObject* pKey, bool bFindOver)
{
	DSLElement* pNode = m_pDSList->m_pHead;
	int nLevel = m_pDSList->m_nLevel;
	m_nKeyLevel = -1;
	AcRx::Ordering order = AcRx::kNotOrderable;
	DSLElement* pLast = NULL;

	do 
	{
		if (nLevel < 0)
			return (order == AcRx::kEqual) && bFindOver;
		
		DSLElement* pCurr = pNode;
		do 
		{
			pNode = pCurr;
			pCurr = pNode->getNext(nLevel);
			m_pElement = pCurr;

			if (NULL == pCurr)
			{
				m_ElmPtrLists[nLevel--] = pNode;
				break;
			}

			if (pLast != pCurr)
			{
				order = pKey->comparedTo(pCurr->key());		// 48
				pLast = pCurr;
			}

			if (AcRx::kLessThan == order)
			{
				m_ElmPtrLists[nLevel--] = pNode;
				break;
			}
			else if (AcRx::kEqual == order)
			{
				if (-1 == m_nKeyLevel)
					m_nKeyLevel = nLevel;

				if (!bFindOver)
					return true;

				m_ElmPtrLists[nLevel--] = pNode;
				break;
			}
		} while (order > AcRx::kEqual);
	} while (true);

	return false;
}

DSLElement* SkipListFinder::element(void) const
{
	return m_pElement;
}

DSLElement* SkipListFinder::pointerList(unsigned int level) const
{
	return m_ElmPtrLists[level];
}

int SkipListFinder::keyLevel(void) const
{
	return m_nKeyLevel;
}

void SkipListFinder::setPointerList(unsigned int level, DSLElement* pElement)
{
	m_ElmPtrLists[level] = pElement;
}


//------------------------------------------------------------------
// DictSkipList
//------------------------------------------------------------------
DictSkipList::DictSkipList()
	: m_nLevel(0)
	, m_nNumEntries(0)
	, m_nUnknown8(0)
	, m_nUnknown12(0)
	, m_pHead(NULL)
{
	m_pHead = new DSLElement(1);
}

DictSkipList::~DictSkipList()
{
	if (m_pHead != NULL)
	{
		DSLElement* pElement = m_pHead;
		do
		{
			m_pHead = pElement->getNext(0);
			delete pElement;
			pElement = m_pHead;
		} while (m_pHead != NULL);
	}
}

Adesk::UInt32 DictSkipList::at(const AcRxObject* pKey)
{
	unsigned int ret = 0;
	SkipListFinder finder(this);
	if (finder.find(pKey, false))
		ret = finder.element()->id();

	return ret;
}

SkipListFinder DictSkipList::resetFinder(void) const
{
	return SkipListFinder(this);
}

Adesk::UInt32 DictSkipList::atPut(AcRxObject* pKey, Adesk::UInt32 id)
{
	Adesk::UInt32 result = 0;

	int level = 0;
	SkipListFinder finder(this);
	if (finder.find(pKey, false))
	{
		result = finder.element()->id();
		finder.element()->setID(id);
	}
	else
	{
		++m_nUnknown12;
		int v6 = m_nUnknown8;
		m_nUnknown8 = (m_nUnknown8 + 1) & 0x7FFFFFFF;

		while (v6 & 1)
		{
			++level;
			v6 >>= 1;
		}
		
		DSLElement* pElement = new DSLElement(level + 1);
		++m_nNumEntries;
		DSLElement* pPutNode = NULL;
		int putLevel;
		if (level > m_nLevel)
		{
			pPutNode = m_pHead;
			int nIdx = 0;
			do
			{
				pElement->setNext(nIdx, m_pHead->getNext(nIdx));
				nIdx = nIdx + 1;
			} while (nIdx <= m_nLevel);

			putLevel = m_nLevel;
			m_pHead = pElement;
			m_nLevel = level;
		}
		else
		{
			pPutNode = pElement;
			putLevel = level;
		}

		pPutNode->setKey(pKey);
		pPutNode->setID(id);

		int idx = 0;
		do
		{
			DSLElement* pNode = finder.pointerList(idx);
			if (pNode == pPutNode)
			{
				pNode = m_pHead;
				finder.setPointerList(idx, pNode);
			}

			pPutNode->setNext(idx, pNode->getNext(idx));
			pNode->setNext(idx, pPutNode);
			idx = idx + 1;
		} while (idx <= putLevel);
	}

	return result;
}

bool DictSkipList::has(const AcRxObject* pKey)
{
	SkipListFinder finder(this);
	return finder.find(pKey, false);
}

Adesk::UInt32 DictSkipList::remove(const AcRxObject* pKey)
{
	SkipListFinder finder(this);
	Adesk::UInt32 id = 0;
	if (finder.find(pKey, true))
	{
		int idx = 0;
		int level = finder.keyLevel();
		DSLElement* pNode = finder.element();
		do
		{
			finder.pointerList(idx)->setNext(idx, pNode->getNext(idx));
			idx = idx + 1;
		} while (idx <= level);

		--m_nNumEntries;
		id = pNode->id();
		if (pNode)
			delete pNode;
	}

	return id;
}

Adesk::UInt32 DictSkipList::nbrOfEntries(void)
{
	return m_nNumEntries;
}

DictSkipListIterator* DictSkipList::newIterator(void)
{
	return new DictSkipListIterator(this);
}

DictSkipListIterator DictSkipList::resetIterator(void)
{
	return DictSkipListIterator(this);
}


//------------------------------------------------------------------
// DictSkipListIterator
//------------------------------------------------------------------
DictSkipListIterator::DictSkipListIterator(const DictSkipList* pSkipList)
	: m_pDicSkipList(pSkipList)
{
	m_pCurElement = m_pDicSkipList->m_pHead->getNext(0);
}

DictSkipListIterator::~DictSkipListIterator()
{

}

Adesk::Boolean DictSkipListIterator::done() const
{
	return m_pCurElement == NULL;
}

Adesk::Boolean DictSkipListIterator::next()
{
	if (m_pCurElement != NULL)
		m_pCurElement = m_pCurElement->getNext(0);

	return m_pCurElement != NULL;
}

AcRxObject* DictSkipListIterator::object() const
{
	return NULL;
}

const ACHAR* DictSkipListIterator::key() const
{
	if (NULL == m_pCurElement)
		return NULL;

	AcRxObject* pKey = m_pCurElement->key();
	AcRxTextString* pKeyStr = AcRxTextString::cast(pKey);
	if (NULL == pKeyStr)
		return NULL;

	return pKeyStr->getAsciiPtr();	// 64
}

Adesk::UInt32 DictSkipListIterator::id() const
{
	if (NULL == m_pCurElement)
		return 0;

	return m_pCurElement->id();
}
