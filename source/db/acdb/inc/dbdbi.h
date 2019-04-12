#ifndef _AC_DB_DBI_H_
#define _AC_DB_DBI_H_

#include "acdb.h"
#include "dbheader.h"
#include "AcHeapOpers.h"

class AcDbObject;


class AcDbImpDatabase : public AcHeapOperators	// win size: 1050h
{
public:
	AcDbImpDatabase(AcDbDatabase* pDb, bool noDocument);
	virtual ~AcDbImpDatabase();

public:
	Acad::ErrorStatus buildDefaultDwg(bool);
	Acad::ErrorStatus defaultTheTables(bool);

	Acad::ErrorStatus createDefaultLayout(int, bool);

	void fixupHeaderData(void);

public:
	AcDbHeader* header() { return &m_header; }

	Acad::ErrorStatus addAcDbObject(AcDbObject* pObj);
	Acad::ErrorStatus addAcDbObject(AcDbObjectId& objId, AcDbObject* pObj);
	Acad::ErrorStatus addAcDbObjectAtAcDbHandle(AcDbObjectId& objId, AcDbObject* pObj, const AcDbHandle& handle, bool b);

public:
	void sendConstructedNotification(void);

private:
	AcDbDatabase*		m_pDb;				// 40
	AcDbHeader			m_header;			// 120
	AcDbHandleTable*	m_pHandleTable;		// 3048
	bool				m_bUnk3995;			// 3995
};

#endif // _AC_DB_DBI_H_