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
	for (MemSegmentList::iterator iter = m_unk8.begin();
		iter != m_unk8.end();
		++iter)
	{
		MacMemorySegment* pSegment = *iter;
		if (pSegment != NULL)
		{
			free(pSegment->m_pUnk8 - 8);
			delete pSegment;
		}
	}

	unRegSameSizePool(this);
	
	m_unk32.clear();
	m_unk8.clear();
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
		pPool = *iter;
		if (pPool->size() == size)
			break;
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
	if (m_unk32.size() > 0)
		pSegment = m_unk32.front();
	else
		pSegment = allocSegment();

	void* pMem = NULL;
	if (pSegment != NULL)
	{
		pMem = pSegment->allocate();
		if (pSegment->isUsedUp())
		{
			m_unk32.remove(pSegment);
		}
	}

	return pMem;
}

void MacSameSizePool::release(void* pMem)
{
	if (pMem != NULL)
	{
		MacMemorySegment* pSegment = NULL;
		for (MemSegmentList::iterator iter = m_unk8.begin();
			iter != m_unk8.end();
			++iter)
		{
			pSegment = *iter;
			if (pSegment->contains(pMem))
				break;
		}

		if (NULL == pSegment)
			return;

		bool bUnk11 = false;
		if (pSegment->m_pUnk8 != NULL)
		{
			if (NULL == pSegment->m_pUnk32)
				bUnk11 = (pSegment->m_pUnk16 + pSegment->m_size) > pSegment->m_pUnk24;
		}

		*(Adesk::Int8**)pMem = pSegment->m_pUnk32;
		pSegment->m_pUnk32 = (MEMORY_POINTER)pMem;
		++pSegment->m_unk40;
		if (pSegment->m_pUnk8 && pSegment->m_unk40 != pSegment->m_unk44)
		{
			if (bUnk11)
				m_unk32.push_back(pSegment);
		}
		else if (m_unk32.size() != 1)
		{
			m_unk8.remove(pSegment);
			m_unk32.remove(pSegment);

			free(pSegment->m_pUnk8 - 8);
			delete pSegment;
		}
	}
}

MacMemorySegment* MacSameSizePool::allocSegment(void)
{
	MacMemorySegment* pSegment = new MacMemorySegment(m_size);
	m_unk8.push_front(pSegment);
	m_unk32.push_front(pSegment);
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
			delete *iter;
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
	for (MemSegmentList::iterator iter = m_unk8.begin();
		iter != m_unk8.end();
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
	: m_pUnk8(NULL)
	, m_pUnk16(NULL)
	, m_pUnk24(NULL)
	, m_pUnk32(NULL)
	, m_unk40(0)
	, m_unk44(0)
{
	if (size < 8)
		size = 8;
	m_size = size;
}

MacMemorySegment::~MacMemorySegment()
{

}

void* MacMemorySegment::allocate(void)
{
	if (NULL == m_pUnk8)
	{
		MEMORY_POINTER pMem = (MEMORY_POINTER)malloc(0x11007);
		Adesk::UInt16 uOff = (Adesk::UInt16)(pMem + 8) & 0x0FFF;
		*(Adesk::Int8**)(pMem - uOff + 0x1000) = pMem;
		MEMORY_POINTER pUnk = 0x1000 - uOff + pMem + 8;
		m_pUnk8 = pUnk;
		if (NULL == pUnk)
			return NULL;

		m_pUnk16 = pUnk;
		m_pUnk24 = pUnk + 0x10000;
		m_unk44 = 0x10000 / m_size;
	}

	void* pAlloc = m_pUnk32;
	if (pAlloc != NULL)
	{
		m_pUnk32 = (MEMORY_POINTER)*(Adesk::IntPtr*)pAlloc;
		--m_unk40;
	}
	else
	{
		if (m_pUnk16 + m_size <= m_pUnk24)
		{
			pAlloc = m_pUnk16;
			m_pUnk16 = m_pUnk16 + m_size;
		}
	}

	return pAlloc;
}
