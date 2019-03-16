#include "dbheader.h"
#include "acutmacro.h"
#include "acutmem.h"


#define DEF_DIM_VAR(type, var, initval, getmethod, setmethod) \
	type AcDbHeader::getmethod() const \
	{ \
		return m_##var; \
	} \
	Acad::ErrorStatus AcDbHeader::setmethod(type v) \
	{ \
		m_##var = v; \
		return Acad::eOk; \
	}

#define DEF_DIM_VAR_CSTR(type, var, initval, getmethod, setmethod) \
	const type AcDbHeader::getmethod() const \
	{ \
		return m_##var; \
	} \
	Acad::ErrorStatus AcDbHeader::setmethod(const type v) \
	{ \
		acutUpdString(v, m_##var); \
		return Acad::eOk; \
	}

#define DEF_DIM_VAR_CONST_REF(type, var, initval, getmethod, setmethod) \
	type AcDbHeader::getmethod() const \
	{ \
		return m_##var; \
	} \
	Acad::ErrorStatus AcDbHeader::setmethod(const type& v) \
	{ \
		m_##var = v; \
		return Acad::eOk; \
	}

#include "dbDimVarDef.h"

#undef DEF_DIM_VAR
#undef DEF_DIM_VAR_CSTR
#undef DEF_DIM_VAR_CONST_REF


//////////////////////////////////////////////////////////////////////////
AcDbHeader::AcDbHeader(AcDbDatabase* pDb)
{
#define DEF_DIM_VAR(type, var, initval, getmethod, setmethod) \
	m_##var = initval;

#define DEF_DIM_VAR_CSTR(type, var, initval, getmethod, setmethod) \
	acutNewString(initval, m_##var);

#define DEF_DIM_VAR_CONST_REF DEF_DIM_VAR

#include "dbDimVarDef.h"

#undef DEF_DIM_VAR
#undef DEF_DIM_VAR_CSTR
#undef DEF_DIM_VAR_CONST_REF
}

AcDbHeader::~AcDbHeader()
{
#define DEF_DIM_VAR(type, var, initval, getmethod, setmethod)

#define DEF_DIM_VAR_CSTR(type, var, initval, getmethod, setmethod) \
	if(m_##var != NULL) \
	{ \
		acutDelString(m_##var); \
		m_##var = NULL; \
	}

#define DEF_DIM_VAR_CONST_REF DEF_DIM_VAR

#include "dbDimVarDef.h"

#undef DEF_DIM_VAR
#undef DEF_DIM_VAR_CSTR
#undef DEF_DIM_VAR_CONST_REF
}

Acad::ErrorStatus AcDbHeader::setHandseed(const AcDbHandle& hSeed)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}
