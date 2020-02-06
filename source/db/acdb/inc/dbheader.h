#ifndef _AC_DB_HEADER_H_
#define _AC_DB_HEADER_H_

#include "acadstrc.h"
#include "dbhandle.h"
#include "dbmain.h"


class AcDbHeader
{
public:
	AcDbHeader(AcDbDatabase* pDb);
	~AcDbHeader();

public:
	Acad::ErrorStatus dwgIn(AcDbDwgFiler*);
	Acad::ErrorStatus dwgOut(AcDbDwgFiler*, AcDbIdMapping*);

public:
	AcDbHandle handseed(void) const { return m_handleSeed; }
	Acad::ErrorStatus setHandseed(const AcDbHandle& hSeed);

public:
#define DEF_DIM_VAR(type, var, initval, getmethod, setmethod) \
	type getmethod() const; \
	Acad::ErrorStatus setmethod(type);

#define DEF_DIM_VAR_CSTR(type, var, initval, getmethod, setmethod) \
	const type getmethod() const; \
	Acad::ErrorStatus setmethod(const type);

#define DEF_DIM_VAR_CONST_REF(type, var, initval, getmethod, setmethod) \
	type getmethod() const; \
	Acad::ErrorStatus setmethod(const type&);

#include "dbDimVarDef.h"

#undef DEF_DIM_VAR
#undef DEF_DIM_VAR_CSTR
#undef DEF_DIM_VAR_CONST_REF

private:
	AcDbHandle			m_handleSeed;		// 700

private:
#define DEF_DIM_VAR(type, var, initval, getmethod, setmethod) \
	type m_##var;

#define DEF_DIM_VAR_CSTR DEF_DIM_VAR

#define DEF_DIM_VAR_CONST_REF DEF_DIM_VAR

#include "dbDimVarDef.h"

#undef DEF_DIM_VAR
#undef DEF_DIM_VAR_CSTR
#undef DEF_DIM_VAR_CONST_REF
};

#endif // _AC_DB_HEADER_H_