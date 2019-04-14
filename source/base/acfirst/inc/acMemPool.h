#ifndef _AC_MEM_POOL_H_
#define _AC_MEM_POOL_H_

#include <windows.h>
#include <list>
#include "adesk.h"

class MacMemorySegment		// size: 0x30
{
public:
	MacMemorySegment(size_t size);
	~MacMemorySegment();

public:
	void* allocate(void);
	void release(void* pMem);

	bool contains(const void* pBlock) const
	{
		return (m_pSegBegin <= pBlock && pBlock < m_pSegEnd);
	}

	bool isUsedUp() const
	{
		return m_pSegBegin != NULL && NULL == m_pFreeBlock && (m_pUnuseBlock + m_nBlockSize > m_pSegEnd);
	}

	bool isAllReleased() const
	{
		return NULL == m_pSegBegin || m_nFreeCount == m_nBlockCount;
	}

private:
	struct Block
	{
		Block* _next;
	};

	size_t		m_nBlockSize;		// 0
	char*		m_pSegBegin;		// 8
	char*		m_pUnuseBlock;		// 16
	char*		m_pSegEnd;			// 24
	Block*		m_pFreeBlock;		// 32
	size_t		m_nFreeCount;		// 40
	size_t		m_nBlockCount;		// 44
};

class MacSameSizePool
{
private:
	MacSameSizePool(size_t size);

public:
	~MacSameSizePool();

public:
	static MacSameSizePool* newPool(size_t size);
	static void terminate(void);

	static MacSameSizePool* getDefaultPool(size_t size, bool bNewPool);
	static MacSameSizePool* newDefaultPool(size_t size);

	static void unRegSameSizePool(MacSameSizePool* pPool);
	static void regSameSizePool(MacSameSizePool* pPool);

	static MacSameSizePool* getPoolByBlock(const void* pBlock);

public:
	size_t size() const { return m_size; }

public:
	void* allocate(void);
	MacMemorySegment* allocSegment(void);

	void release(void* pMem);

	MacMemorySegment* getSegmentByBlock(const void* pBlock);

	bool contains(const void* pBlock);

public:
	void initialize(void);

private:
	typedef std::list<MacSameSizePool*> SameSizePoolList;

	static SameSizePoolList s_AllSameSizePool;
	static SameSizePoolList s_DefaultSameSizePool;

private:
	typedef std::list<MacMemorySegment*> MemSegmentList;

	size_t			m_size;			// 0
	MemSegmentList	m_segList;		// 8
	MemSegmentList	m_freeList;		// 32
};

class HeapManager
{
public:
	HeapManager();
	~HeapManager();

	static void lock() { EnterCriticalSection(&criticalSection); }
	static void unlock() { LeaveCriticalSection(&criticalSection); }

	static bool freeDisabled() { return sm_bFreeDisabled; }
	static bool useAcHeap() { return sm_bUseAcHeap; }

private:
	static CRITICAL_SECTION criticalSection;
	static bool sm_bInited;
	static bool sm_bFreeDisabled;
	static bool sm_bUseAcHeap;
};

#endif // _AC_MEM_POOL_H_