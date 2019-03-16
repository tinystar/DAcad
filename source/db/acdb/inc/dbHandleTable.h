#ifndef _AC_DB_HANDLE_TABLE_H_
#define _AC_DB_HANDLE_TABLE_H_

#include "dbhandle.h"
#include "dbid.h"
#include "acadstrc.h"
#include "AcHeapOpers.h"

class AcDbDatabase;
class AcDbStub;
class Bits4Through8;
class AcDbHandleTableIterator;
class AcDbHeader;

class AcDbHandleTable : public AcHeapOperators	// win size: 70h
{
public:
	AcDbHandleTable(AcDbDatabase* pDb, bool);
	~AcDbHandleTable();

public:
	AcDbHandle getMaxHandle(void);
	void getMaxHandle25Through32(Bits4Through8****, AcDbHandle&);
	bool isValid(const AcDbStub*);
	AcDbStub* findStubAtHandle(const AcDbHandle&);
	Acad::ErrorStatus getStubAtHandle(AcDbObjectId&, const AcDbHandle&);
	Acad::ErrorStatus addHandle(AcDbStub*&, const AcDbHandle&);
	void get8Stubs(AcDbHandle);
	void newIterator(AcDbHandleTableIterator*&, bool);

private:
	AcDbDatabase*		m_pBelongDb;		// 0
	AcDbHeader*			m_pDbHeader;		// 8
};

#endif // _AC_DB_HANDLE_TABLE_H_