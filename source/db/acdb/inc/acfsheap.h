#ifndef _ACFS_HEAP_H_
#define _ACFS_HEAP_H_

class AcFsHeap
{
public:
	static AcFsHeap* createHeap(void);
	void destroyHeap(AcFsHeap*);

public:
	void* zalloc(size_t sz);
};

#endif // _ACFS_HEAP_H_