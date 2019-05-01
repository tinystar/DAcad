#include "dbmain.h"
#include "dbdbi.h"
#include "acutmacro.h"


#define DEF_DIM_VAR(type, var, initval, getmethod, setmethod) \
	type AcDbDatabase::getmethod() const \
	{ \
		return mpImpDb->header()->getmethod(); \
	} \
	Acad::ErrorStatus AcDbDatabase::setmethod(type v) \
	{ \
		return mpImpDb->header()->setmethod(v); \
	}

#define DEF_DIM_VAR_CSTR(type, var, initval, getmethod, setmethod) \
	const type AcDbDatabase::getmethod() const \
	{ \
		return mpImpDb->header()->getmethod(); \
	} \
	Acad::ErrorStatus AcDbDatabase::setmethod(const type v) \
	{ \
		return mpImpDb->header()->setmethod(v); \
	}

#define DEF_DIM_VAR_CONST_REF(type, var, initval, getmethod, setmethod) \
	type AcDbDatabase::getmethod() const \
	{ \
		return mpImpDb->header()->getmethod(); \
	} \
	Acad::ErrorStatus AcDbDatabase::setmethod(const type& v) \
	{ \
		return mpImpDb->header()->setmethod(v); \
	}

#include "dbDimVarDef.h"

#undef DEF_DIM_VAR
#undef DEF_DIM_VAR_CSTR
#undef DEF_DIM_VAR_CONST_REF


//////////////////////////////////////////////////////////////////////////
ACRX_NO_CONS_DEFINE_MEMBERS(AcDbDatabase, AcRxObject)


AcDbDatabase::AcDbDatabase(bool buildDefaultDrawing /*= true*/, bool noDocument /*= false*/)
	: mpImpDb(NULL)
{
	mpImpDb = new AcDbImpDatabase(this, noDocument);
	if (buildDefaultDrawing)
	{
		mpImpDb->buildDefaultDwg(true);
		mpImpDb->createDefaultLayout(2, false);
		mpImpDb->fixupHeaderData();
	}

	mpImpDb->sendConstructedNotification();
}

AcDbDatabase::~AcDbDatabase()
{
	delete mpImpDb;
}

Acad::ErrorStatus AcDbDatabase::setDimblk(const ACHAR* v)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbDatabase::setDimblk1(const ACHAR* v)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbDatabase::setDimblk2(const ACHAR* v)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbDatabase::setDimldrblk(const ACHAR* v)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbDatabase::readDwgFile(const ACHAR* fileName, const int shmode /*= _SH_DENYWR*/, bool bAllowCPConversion /*= false*/, const wchar_t* wszPassword /*= NULL*/)
{
	return mpImpDb->readDwgFile(fileName, shmode, bAllowCPConversion, wszPassword);
}
