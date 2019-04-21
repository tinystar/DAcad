#include "dbHandleTable.h"
#include "dbSysInternals.h"
#include "dbdbi.h"
#include "acutmacro.h"

#define LEAFNODE_ID_MASK  0xFFFFFFFFFFFFFF00
#define STUBGROUP_ID_MASK 0xFFFFFFFFFFFFFFF8

#ifdef _WIN64
static const size_t NODE_SIZE = 0x800;			// 256 * 8
static const size_t LEAF_NODE_SIZE = 0x120;		// 32 + 8 * 32
#else
static const size_t NODE_SIZE = 0x400;			// 256 * 4
static const size_t LEAF_NODE_SIZE = 0xA0;		// 32 + 4 * 32
#endif

AcDbHandleTable::AcDbHandleTable(AcDbDatabase* pDb, bool)
	: m_pBelongDb(pDb)
	, m_pDbHeader(NULL)
	, m_pRootHigh(NULL)
	, m_pRootLow(NULL)
	, m_pFreePage(NULL)
	, m_pFullPage(NULL)
	, m_pLastLeafNode(NULL)
	, m_uLastLeafId(1)
	, m_uUnknown88(0)
{
	m_pLeafHeap = __acHeapCreate(LEAF_NODE_SIZE);
	m_pNodeHeap = __acHeapCreate(NODE_SIZE);
}

AcDbHandleTable::~AcDbHandleTable()
{
	__acHeapDestroy(m_pLeafHeap);
	__acHeapDestroy(m_pNodeHeap);
}

AcDbStub* AcDbHandleTable::findStubAtHandle(const AcDbHandle& h)
{
	Adesk::UInt32 low = h.low();
	Adesk::UInt64 uLeafNodeId = (Adesk::UInt64)h & LEAFNODE_ID_MASK;
	Leaf256StubsNode* pLeafNode = NULL;
	if (uLeafNodeId == m_uLastLeafId)
	{
		pLeafNode = m_pLastLeafNode;
	}
	else
	{
		StubSearchTreeNode* pNodeLow = NULL;
		Adesk::UInt32 high = h.high();
		if (high > 0)
		{
			if (NULL == m_pRootHigh)
				return NULL;

			StubSearchTreeNode* pChild = m_pRootHigh->_children[high >> 24];
			if (NULL == pChild)
				return NULL;

			pChild = pChild->_children[(high >> 16) & 0xF];
			if (NULL == pChild)
				return NULL;

			pChild = pChild->_children[(high >> 8) & 0xF];
			if (NULL == pChild)
				return NULL;

			pNodeLow = pChild->_children[high & 0xF];
		}
		else
		{
			pNodeLow = m_pRootLow;
		}

		if (NULL == pNodeLow)
			return NULL;

		pNodeLow = pNodeLow->_children[low >> 24];
		if (NULL == pNodeLow)
			return NULL;

		pNodeLow = pNodeLow->_children[(low >> 16) & 0xF];
		if (NULL == pNodeLow)
			return NULL;

		pLeafNode = pNodeLow->_leaves[(low >> 8) & 0xF];
		if (NULL == pLeafNode)
			return NULL;

		m_uLastLeafId = uLeafNodeId;
		m_pLastLeafNode = pLeafNode;
	}

	Adesk::UInt32 uLeaf8StubIdx = (low >> 3) & 0x1F;	// handle最低字节的高五位
	AcDbStub* p8Stubs = pLeafNode->_p8Stubs[uLeaf8StubIdx];
	if (NULL == p8Stubs)
		return NULL;

	Adesk::UInt8 uBitMask = 1 << (low & 0x7);
	if (pLeafNode->_bitFlag[uLeaf8StubIdx] & uBitMask)
		return &p8Stubs[low & 0x7];

	return NULL;
}

Acad::ErrorStatus AcDbHandleTable::getStubAtHandle(AcDbObjectId& retId, const AcDbHandle& h)
{
	Acad::ErrorStatus es = Acad::eOk;
	AcDbStub* pStub = findStubAtHandle(h);
	if (pStub)
	{
		retId = pStub;
	}
	else
	{
		es = addHandle(pStub, h);
		retId = pStub;
	}

	return es;
}

Acad::ErrorStatus AcDbHandleTable::addHandle(AcDbStub*& pStub, const AcDbHandle& h)
{
	Adesk::UInt32 low = h.low();
	Adesk::UInt64 uLeafNodeId = (Adesk::UInt64)h & LEAFNODE_ID_MASK;
	Leaf256StubsNode* pLeafNode = NULL;
	if (uLeafNodeId == m_uLastLeafId)
	{
		pLeafNode = m_pLastLeafNode;
	}
	else
	{
		if (h.isNull())
			return Acad::eNullHandle;

		StubSearchTreeNode** ppNodeLow = NULL;
		Adesk::UInt32 high = h.high();
		if (high > 0)
		{
			if (NULL == m_pRootHigh)
			{
				m_pRootHigh = (StubSearchTreeNode*)__acHeapMalloc(m_pNodeHeap);
				memset(m_pRootHigh, 0, NODE_SIZE);
			}

			StubSearchTreeNode* pChild = m_pRootHigh->_children[high >> 24];
			if (NULL == pChild)
			{
				pChild = (StubSearchTreeNode*)__acHeapMalloc(m_pNodeHeap);
				memset(pChild, 0, NODE_SIZE);
				m_pRootHigh->_children[high >> 24] = pChild;
			}

			StubSearchTreeNode* pParent = pChild;
			pChild = pParent->_children[(high >> 16) & 0xF];
			if (NULL == pChild)
			{
				pChild = (StubSearchTreeNode*)__acHeapMalloc(m_pNodeHeap);
				memset(pChild, 0, NODE_SIZE);
				pParent->_children[(high >> 16) & 0xF] = pChild;
			}

			pParent = pChild;
			pChild = pParent->_children[(high >> 8) & 0xF];
			if (NULL == pChild)
			{
				pChild = (StubSearchTreeNode*)__acHeapMalloc(m_pNodeHeap);
				memset(pChild, 0, NODE_SIZE);
				pParent->_children[(high >> 8) & 0xF] = pChild;
			}

			ppNodeLow = &(pChild->_children[high & 0xF]);
		}
		else
		{
			ppNodeLow = &m_pRootLow;
		}

		StubSearchTreeNode* pParentLow = *ppNodeLow;
		if (NULL == pParentLow)
		{
			pParentLow = (StubSearchTreeNode*)__acHeapMalloc(m_pNodeHeap);
			memset(pParentLow, 0, NODE_SIZE);
			*ppNodeLow = pParentLow;
		}

		StubSearchTreeNode* pChildLow = pParentLow->_children[low >> 24];
		if (NULL == pChildLow)
		{
			pChildLow = (StubSearchTreeNode*)__acHeapMalloc(m_pNodeHeap);
			memset(pChildLow, 0, NODE_SIZE);
			pParentLow->_children[low >> 24] = pChildLow;
		}

		pParentLow = pChildLow;
		pChildLow = pParentLow->_children[(low >> 16) & 0xF];
		if (NULL == pChildLow)
		{
			pChildLow = (StubSearchTreeNode*)__acHeapMalloc(m_pNodeHeap);
			memset(pChildLow, 0, NODE_SIZE);
			pParentLow->_children[(low >> 16) & 0xF] = pChildLow;
		}

		pLeafNode = pChildLow->_leaves[(low >> 8) & 0xF];
		if (NULL == pLeafNode)
		{
			pLeafNode = (Leaf256StubsNode*)__acHeapMalloc(m_pLeafHeap);
			pChildLow->_leaves[(low >> 8) & 0xF] = pLeafNode;
			for (int i = 0; i < 32; ++i)
			{
				pLeafNode->_bitFlag[i] = 0;
				pLeafNode->_p8Stubs[i] = NULL;
			}
		}

		m_uLastLeafId = uLeafNodeId;
		m_pLastLeafNode = pLeafNode;
	}

	Adesk::UInt32 uLeaf8StubIdx = (low >> 3) & 0x1F;	// handle最低字节的高五位
	AcDbStub* p8Stubs = pLeafNode->_p8Stubs[uLeaf8StubIdx];
	if (NULL == p8Stubs)
	{
		p8Stubs = get8Stubs(h);
		pLeafNode->_p8Stubs[uLeaf8StubIdx] = p8Stubs;
	}

	pStub = &p8Stubs[low & 0x7];
	Adesk::UInt8 uBitMask = 1 << (low & 0x7);
	if (pLeafNode->_bitFlag[uLeaf8StubIdx] & uBitMask)
		return Acad::eHandleExists;

	pLeafNode->_bitFlag[uLeaf8StubIdx] |= uBitMask;
	++m_uUnknown88;
	return Acad::eOk;
}

AcDbStub* AcDbHandleTable::get8Stubs(AcDbHandle h)
{
	Adesk::UInt64 hGroup = (Adesk::UInt64)h & STUBGROUP_ID_MASK;

	AcDbStubPage* pCurPage = m_pFreePage;
	AcDbStubPage* pLastPage = NULL;
	while (pCurPage != NULL)
	{
		if (hGroup < pCurPage->_startHandle)
			break;

		if (hGroup - pCurPage->_startHandle <= 0xFFFF)
		{
			pCurPage->_groupOffset[pCurPage->_usedCount] = (hGroup - pCurPage->_startHandle) & 0xFFF8;
			++pCurPage->_usedCount;
			if (STUB_GROUP_COUNT == pCurPage->_usedCount)
			{
				if (NULL == pLastPage)
					m_pFreePage = pCurPage->_next;
				else
					pLastPage->_next = pCurPage->_next;

				pCurPage->_next = m_pFullPage;
				m_pFullPage = pCurPage;
			}

			return pCurPage->_stubs + (pCurPage->_usedCount - 1) * STUB_COUNT_PER_GROUP;
		}

		pLastPage = pCurPage;
		pCurPage = pCurPage->_next;
	}

	AcDbStubPage* pNewPage = (AcDbStubPage*)__acHeapMalloc(m_pNodeHeap);
	memset(pNewPage, 0, NODE_SIZE);
	pNewPage->_groupOffset[0] = 0;
	pNewPage->_pHandleTable = this;
	pNewPage->_next = pCurPage;
	pNewPage->_usedCount = 1;
	pNewPage->_startHandle = hGroup;
	if (NULL == pLastPage)
		m_pFreePage = pNewPage;
	else
		pLastPage->_next = pNewPage;

	return pNewPage->_stubs;
}
