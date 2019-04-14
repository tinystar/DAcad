#include "acMemPool.h"


//------------------------------------------------------------------
// MacSameSizePool
//------------------------------------------------------------------
MacSameSizePool::SameSizePoolList MacSameSizePool::s_AllSameSizePool;
MacSameSizePool::SameSizePoolList MacSameSizePool::s_DefaultSameSizePool;

MacSameSizePool::MacSameSizePool(size_t size)
	: m_size(size)
{
	regSameSizePool(this);
}

MacSameSizePool::~MacSameSizePool()
{
	for (MemSegmentList::iterator iter = m_segList.begin();
		iter != m_segList.end();
		++iter)
	{
		MacMemorySegment* pSegment = *iter;
		if (pSegment != NULL)
			delete pSegment;
	}

	unRegSameSizePool(this);
	
	m_freeList.clear();
	m_segList.clear();
}

MacSameSizePool* MacSameSizePool::newPool(size_t size)
{
	return new MacSameSizePool(size);
}

void MacSameSizePool::terminate(void)
{
	for (SameSizePoolList::iterator iter = s_DefaultSameSizePool.begin();
		iter != s_DefaultSameSizePool.end();
		++iter)
	{
		MacSameSizePool* pPool = *iter;
		if (pPool != NULL)
			delete pPool;
	}

	s_DefaultSameSizePool.clear();
}

MacSameSizePool* MacSameSizePool::getDefaultPool(size_t size, bool bNewPool)
{
	MacSameSizePool* pPool = NULL;
	for (SameSizePoolList::iterator iter = s_DefaultSameSizePool.begin();
		iter != s_DefaultSameSizePool.end();
		++iter)
	{
		if ((*iter)->size() == size)
		{
			pPool = *iter;
			break;
		}
	}

	if (NULL == pPool && bNewPool)
		pPool = newDefaultPool(size);

	return pPool;
}

MacSameSizePool* MacSameSizePool::newDefaultPool(size_t size)
{
	MacSameSizePool* pPool = newPool(size);
	if (pPool != NULL)
		s_DefaultSameSizePool.push_back(pPool);

	return pPool;
}

void* MacSameSizePool::allocate(void)
{
	MacMemorySegment* pSegment = NULL;
	if (m_freeList.size() > 0)
		pSegment = m_freeList.front();
	else
		pSegment = allocSegment();

	void* pMem = NULL;
	if (pSegment != NULL)
	{
		pMem = pSegment->allocate();
		if (pSegment->isUsedUp())
		{
			m_freeList.remove(pSegment);
		}
	}

	return pMem;
}

void MacSameSizePool::release(void* pMem)
{
	if (pMem != NULL)
	{
		MacMemorySegment* pSegment = NULL;
		for (MemSegmentList::iterator iter = m_segList.begin();
			iter != m_segList.end();
			++iter)
		{
			if ((*iter)->contains(pMem))
			{
				pSegment = *iter;
				break;
			}
		}

		if (NULL == pSegment)
			return;

		bool bUseup = pSegment->isUsedUp();

		pSegment->release(pMem);

		if (!pSegment->isAllReleased())
		{
			if (bUseup)
				m_freeList.push_back(pSegment);
		}
		else if (m_freeList.size() != 1)
		{
			m_segList.remove(pSegment);
			m_freeList.remove(pSegment);

			delete pSegment;
		}
	}
}

MacMemorySegment* MacSameSizePool::allocSegment(void)
{
	MacMemorySegment* pSegment = new MacMemorySegment(m_size);
	m_segList.push_front(pSegment);
	m_freeList.push_front(pSegment);
	return pSegment;
}

void MacSameSizePool::regSameSizePool(MacSameSizePool* pPool)
{
	s_AllSameSizePool.push_back(pPool);
}

void MacSameSizePool::unRegSameSizePool(MacSameSizePool* pPool)
{
	for (SameSizePoolList::iterator iter = s_AllSameSizePool.begin();
		iter != s_AllSameSizePool.end();
		++iter)
	{
		if (*iter == pPool)
		{
			s_AllSameSizePool.erase(iter);
			return;
		}
	}
}

MacSameSizePool* MacSameSizePool::getPoolByBlock(const void* pBlock)
{
	for (SameSizePoolList::iterator iter = s_AllSameSizePool.begin();
		iter != s_AllSameSizePool.end();
		++iter)
	{
		if ((*iter)->contains(pBlock))
		{
			return *iter;
		}
	}

	return NULL;
}

MacMemorySegment* MacSameSizePool::getSegmentByBlock(const void* pBlock)
{
	for (MemSegmentList::iterator iter = m_segList.begin();
		iter != m_segList.end();
		++iter)
	{
		if ((*iter)->contains(pBlock))
			return *iter;
	}

	return NULL;
}

bool MacSameSizePool::contains(const void* pBlock)
{
	return getSegmentByBlock(pBlock) != NULL;
}


//------------------------------------------------------------------
// MacMemorySegment
//------------------------------------------------------------------
MacMemorySegment::MacMemorySegment(size_t size)
	: m_pSegBegin(NULL)
	, m_pUnuseBlock(NULL)
	, m_pSegEnd(NULL)
	, m_pFreeBlock(NULL)
	, m_nFreeCount(0)
	, m_nBlockCount(0)
{
	if (size < 8)
		size = 8;
	m_nBlockSize = size;
}

MacMemorySegment::~MacMemorySegment()
{
	if (m_pSegBegin != NULL)
		free(*(void**)(m_pSegBegin - 8));
}

void* MacMemorySegment::allocate(void)
{
	if (NULL == m_pSegBegin)
	{
		char* pMem = (char*)malloc(0x11007);
		Adesk::UInt16 uOff = (Adesk::UInt16)(pMem + 8) & 0x0FFF;
		*(void**)(pMem - uOff + 0x1000) = pMem;
		m_pSegBegin = pMem + 8 - uOff + 0x1000;
		if (NULL == m_pSegBegin)
			return NULL;

		m_pUnuseBlock = m_pSegBegin;
		m_pSegEnd = m_pSegBegin + 0x10000;
		m_nBlockCount = 0x10000 / m_nBlockSize;
	}

	void* pAlloc = m_pFreeBlock;
	if (pAlloc != NULL)
	{
		m_pFreeBlock = ((Block*)pAlloc)->_next;
		--m_nFreeCount;
	}
	else
	{
		if (m_pUnuseBlock + m_nBlockSize <= m_pSegEnd)
		{
			pAlloc = m_pUnuseBlock;
			m_pUnuseBlock += m_nBlockSize;
		}
	}

	return pAlloc;
}

void MacMemorySegment::release(void* pMem)
{
	if (NULL == pMem)
		return;

	Block* pMemBlock = (Block*)pMem;
	pMemBlock->_next = m_pFreeBlock;
	m_pFreeBlock = pMemBlock;
	++m_nFreeCount;
}


//------------------------------------------------------------------
// HeapManager
//------------------------------------------------------------------
CRITICAL_SECTION HeapManager::criticalSection;
bool HeapManager::sm_bInited = false;
bool HeapManager::sm_bFreeDisabled = false;
bool HeapManager::sm_bUseAcHeap = false;


HeapManager::HeapManager()
{
	InitializeCriticalSection(&criticalSection);
	EnterCriticalSection(&criticalSection);

	sm_bInited = true;
	sm_bFreeDisabled = false;

	// 	size_t var1Size, var2Size;
	// 	_tgetenv_s(&var1Size, NULL, 0, _T("disableAcadHeap"));
	// 	_tgetenv_s(&var2Size, NULL, 0, _T("CRTLeakReport"));
	// 	if (0 == var1Size && 0 == var2Size)
	sm_bUseAcHeap = true;

	LeaveCriticalSection(&criticalSection);
}

HeapManager::~HeapManager()
{
	EnterCriticalSection(&criticalSection);
	MacSameSizePool::terminate();
	sm_bInited = false;
	LeaveCriticalSection(&criticalSection);
}

HeapManager heapMgr;
