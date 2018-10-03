#include "rxclass.h"
#include "rxdicti.h"
#include "rxnames.h"
#include "acutmacro.h"
#include "cstrop.h"
#include <malloc.h>


extern void initAcRxObject();

// Global variables definition
static AcRxDictionary* sysRegistry = NULL;
static AcRxDictionary* sysClasses = NULL;


//------------------------------------------------------------------
//
//------------------------------------------------------------------
AcRxDictionary* newAcRxDictionary(void)
{
	return new AcRxImpDictionary();
}

AcRxDictionary* newRxOpaqueDictionary(unsigned int a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4)
{
	return new AcRxImpDictionary();
}

AcRxDictionary* acrxSysClasses(void)
{
	return sysClasses;
}

void delSysClasses(void)
{
	AC_SAFE_DELETE(sysClasses);
}

const ACHAR* acStrdup(const ACHAR* pszStr)
{
	ACHAR* pDupStr = NULL;
	if (pszStr != NULL)
	{
		size_t len = ac_strlen(pszStr);
		pDupStr = (ACHAR*)malloc(sizeof(ACHAR) * (len + 1));
		if (NULL == ac_strcpy(pDupStr, pszStr))
		{
			free(pDupStr);
			pDupStr = NULL;
		}
	}

	return pDupStr;
}


//------------------------------------------------------------------
// API Functions
//------------------------------------------------------------------
void acrxInitTypeSystem()
{
	sysRegistry = newAcRxDictionary();
	sysClasses = newRxOpaqueDictionary(0xC8u, 1u, 1u, 0);
	sysRegistry->atPut(ACRX_CLASS_DICTIONARY, sysClasses);		// 80

	initAcRxObject();
}

void acrxTerminateTypeSystem()
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

AcRxDictionary* acrxSysRegistry()
{
	return sysRegistry;
}

void acrx_abort(const ACHAR * format, ...)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
