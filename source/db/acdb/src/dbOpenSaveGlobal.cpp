#include "dbOpenSaveGlobal.h"
#include "adesk.h"

ACHAR* pCurFilename = NULL;


ACHAR*& curOpenSaveFilename(void)
{
	return pCurFilename;
}
