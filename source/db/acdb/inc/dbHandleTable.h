#ifndef _AC_DB_HANDLE_TABLE_H_
#define _AC_DB_HANDLE_TABLE_H_

#include "dbhandle.h"
#include "dbid.h"
#include "acadstrc.h"
#include "AcHeapOpers.h"
#include "acFixedHeap.h"
#include "dbstub.h"

class AcDbDatabase;
class Bits4Through8;
class AcDbHandleTableIterator;
class AcDbHeader;

#define STUB_COUNT_PER_GROUP	8
#define STUB_GROUP_COUNT		15
#define STUB_COUNT_PER_PAGE		(STUB_COUNT_PER_GROUP * STUB_GROUP_COUNT)

struct AcDbStubPage
{
	AcDbStub			_stubs[STUB_COUNT_PER_PAGE];			// 0
	Adesk::UInt16		_groupOffset[STUB_GROUP_COUNT + 1];		// 1920
	AcDbHandleTable*	_pHandleTable;							// 1952
	AcDbStubPage*		_next;									// 1960
	Adesk::UInt32		_usedCount;								// 1968
	Adesk::UInt64		_startHandle;							// 1972
};

class AcDbHandleTable : public AcHeapOperators	// win size: 70h
{
public:
	AcDbHandleTable(AcDbDatabase* pDb, bool);
	~AcDbHandleTable();

public:
	AcDbHandle getMaxHandle(void);
	void getMaxHandle25Through32(Bits4Through8****, AcDbHandle&);
	bool isValid(const AcDbStub*);
	AcDbStub* findStubAtHandle(const AcDbHandle& h);
	Acad::ErrorStatus getStubAtHandle(AcDbObjectId& retId, const AcDbHandle& h);
	Acad::ErrorStatus addHandle(AcDbStub*& pStub, const AcDbHandle& h);
	AcDbStub* get8Stubs(AcDbHandle h);
	void newIterator(AcDbHandleTableIterator*&, bool);

protected:
	struct Leaf256StubsNode
	{
		Adesk::UInt8	_bitFlag[32];
		AcDbStub*		_p8Stubs[32];
	};

	// 多维数组表示
	struct StubSearchTreeNode
	{
		union
		{
			StubSearchTreeNode* _children[256];
			Leaf256StubsNode*	_leaves[256];
		};
	};

private:
	AcDbDatabase*		m_pBelongDb;		// 0
	AcDbHeader*			m_pDbHeader;		// 8

	StubSearchTreeNode*	m_pRootHigh;		// 24
	StubSearchTreeNode*	m_pRootLow;			// 32
	AcDbStubPage*		m_pFreePage;		// 40
	AcDbStubPage*		m_pFullPage;		// 48
	AcFixedSizeHeap*	m_pNodeHeap;		// 56
	AcFixedSizeHeap*	m_pLeafHeap;		// 64
	Leaf256StubsNode*	m_pLastLeafNode;	// 72
	Adesk::UInt64		m_uLastLeafId;		// 80
	Adesk::UInt32		m_uUnknown88;		// 88
};

#endif // _AC_DB_HANDLE_TABLE_H_