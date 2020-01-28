#ifndef _ACFS_GLOBAL_MEMORY_H_
#define _ACFS_GLOBAL_MEMORY_H_

#include <windows.h>

class AcFsHeap;

class AcFsGlobalMemory
{
public:
	AcFsGlobalMemory();
	~AcFsGlobalMemory();

	AcFsHeap* GetHeap(void);
	void IncCount(void);
	void* GetFileThread(void);
	void resetThreads(void);
	void DecCount(void);
	void* GetHeapThread(void);

protected:
	void lock() { EnterCriticalSection(&m_cs); }
	void unlock() { LeaveCriticalSection(&m_cs); }

private:
	AcFsHeap*			m_pFsHeap;		// 0
	CRITICAL_SECTION	m_cs;			// 24
};

extern AcFsGlobalMemory globalMemory;

#endif // _ACFS_GLOBAL_MEMORY_H_