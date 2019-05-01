#include "dbapserv.h"

static AcDbHostApplicationServices* s_pServices = NULL;

Acad::ErrorStatus acdbSetHostApplicationServices(AcDbHostApplicationServices* pServices)
{
	s_pServices = pServices;
	return Acad::eOk;
}

AcDbHostApplicationServices* acdbHostApplicationServices()
{
	return s_pServices;
}
