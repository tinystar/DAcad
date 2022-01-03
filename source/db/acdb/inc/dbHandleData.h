#ifndef _DB_HANDLE_DATA_H_
#define _DB_HANDLE_DATA_H_

#include "dbhandle.h"
#include "acadstrc.h"

class AcHsBufferReader;
class AcDbDwgFiler;

class HandleDataDef
{
public:
	HandleDataDef(AC_BYTE*, Adesk::Int32);

	void resetBuffer(void);
	void setBufferBytes(const AC_BYTE*, Adesk::Int32);
	void getBufferBytes(AC_BYTE*&);
	void add(const Adesk::UInt64&, Adesk::Int64);
	void toBytes(int, const Adesk::UInt64&, AC_BYTE*);
	void start(void);
	bool next(Adesk::UInt64&, Adesk::Int64&);
	void read(AcHsBufferReader&);
	void copyToOldType(const Adesk::UInt64&, AC_BYTE*);
	void compactify(int, int, const AC_BYTE*, AC_BYTE*);
	void field7(int, const AC_BYTE*, int);

public:
	Adesk::Int32 getUnk0() const { return m_nUnk0; }
	Adesk::Int32 getUnk8() const { return m_nUnk8; }

protected:
	Adesk::Int32		m_nUnk0;	// 0
	Adesk::Int32		m_nUnk4;	// 4
	Adesk::Int32		m_nUnk8;	// 8
	Adesk::UInt64		m_uUnk16;	// 16
	void*				m_pUnk24;	// 24
	AC_BYTE*			m_pUnk32;	// 32
};

class HandleDataBase : public HandleDataDef
{
public:
	HandleDataBase(AC_BYTE*, Adesk::Int32);

	Acad::ErrorStatus dwgIn(AcDbDwgFiler*);
	Acad::ErrorStatus dwgOut(AcDbDwgFiler*);
};

class HandleOffsetBuffer : public HandleDataBase
{
public:
	HandleOffsetBuffer();
};

#endif // _DB_HANDLE_DATA_H_