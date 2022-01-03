#ifndef _DB_QUEUE_H_
#define _DB_QUEUE_H_

class QueueChain
{
public:
	QueueChain(int, int);
	~QueueChain();

	void fractionUsed(void);
};

class AcDbQueue
{
public:
	AcDbQueue(int, int);
	~AcDbQueue();

	QueueChain* newQueueChain(void);
	void deleteQueueChain(QueueChain*);

	void enqueue(const void*);
	void dequeue(void);
	void enqueue(AcDbQueue&, double);
	void clear(void);
};

#endif // _DB_QUEUE_H_