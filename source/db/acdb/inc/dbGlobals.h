#ifndef _AC_DB_GLOBALS_H_
#define _AC_DB_GLOBALS_H_

class AcDbGlobals
{
	friend class AcDbImpDatabase;

public:
	AcDbGlobals();
	virtual ~AcDbGlobals();

protected:
	bool		m_bDbLoading;			// 1272
};

#endif // _AC_DB_GLOBALS_H_