#ifndef _AC_DB_DBI_H_
#define _AC_DB_DBI_H_

#include "acdb.h"
#include "dbheader.h"
#include "AcHeapOpers.h"
#include <windows.h>

class AcDbObject;
class AcDbGlobals;
class AcDbFastDwgFiler;
class HandleTableUpdater;


class AcDbImpDatabase : public AcHeapOperators	// win size: 1050h
{
public:
	AcDbImpDatabase(AcDbDatabase* pDb, bool noDocument);
	virtual ~AcDbImpDatabase();

public:
	AcDbDatabase* apiDatabase() const { return m_pDb; }

public:
	Acad::ErrorStatus buildDefaultDwg(bool);
	Acad::ErrorStatus defaultTheTables(bool);

	Acad::ErrorStatus createDefaultLayout(int, bool);

	void fixupHeaderData(void);

	AcDbHeader* header() { return &m_header; }

	Acad::ErrorStatus addAcDbObject(AcDbObject* pObj);
	Acad::ErrorStatus addAcDbObject(AcDbObjectId& objId, AcDbObject* pObj);
	Acad::ErrorStatus addAcDbObjectAtAcDbHandle(AcDbObjectId& objId, AcDbObject* pObj, const AcDbHandle& handle, bool b);

	void sendConstructedNotification(void);

	Acad::ErrorStatus readDwgFile(const ACHAR* fileName, int shmode, bool bAllowCPConversion, const wchar_t* wszPassword);
	Acad::ErrorStatus readDwgFile(AcDwgFileHandle* pDwgFile, bool bAllowCPConversion, const wchar_t* wszPassword);

protected:
	void initializeADP(void);
	Acad::ErrorStatus readR13Drawing(AcDwgFileHandle*, bool);
	Acad::ErrorStatus readAuxHeader(AcDbFastDwgFiler*);

private:
	AcDbDatabase*		m_pDb;				// 40
	//AcArray<AcLargeObjectInfo>			// 48
	AcDbHeader			m_header;			// 120
	Adesk::Int32		m_nUnk2912;			// 2912
	Adesk::Int32		m_nUnk2916;			// 2916
	Adesk::Int32		m_nUnk2920;			// 2920
	Adesk::Int32		m_nUnk2924;			// 2924
	Adesk::UInt32		m_uUnk2932;			// 2932
	Adesk::UInt32		m_uUnk2936;			// 2936
	Adesk::Int32		m_nUnk2940;			// 2940
	Adesk::UInt16		m_uUnk2944;			// 2944
	Adesk::UInt16		m_uUnk2946;			// 2946
	Adesk::UInt16		m_uUnk2948;			// 2948
	Adesk::UInt16		m_uUnk2950;			// 2950
	Adesk::Int32		m_nUnk2952;			// 2952
	Adesk::UInt8		m_uUnk3032;			// 3032
	Adesk::UInt8		m_uUnk3033;			// 3033
	AcDbHandleTable*	m_pHandleTable;		// 3048
	HandleTableUpdater*	m_pHdlTblUpdater;	// 3064
	AcDbGlobals*		m_pGlobals;			// 3384
	Adesk::Int8			m_nUnk3426;			// 3426
	FILETIME			m_FileTime;			// 3576
	//AcString								// 3752
	//AcString			m_strFile;			// 3782
	bool				m_bUnk3995;			// 3995
};

#endif // _AC_DB_DBI_H_