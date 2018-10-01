#include "adesk.h"
#include "rxtypecbs.h"

NewClassRegCallback pNewClassCallback = NULL;
DelClassRegCallback pDeleteClassCallback = NULL;


void AcRxTypeSysCallbacks::setCallback_newClassCallback(NewClassRegCallback pCallback)
{
	pNewClassCallback = pCallback;
}

void AcRxTypeSysCallbacks::setCallback_deleteClassCallback(DelClassRegCallback pCallback)
{
	pDeleteClassCallback = pCallback;
}
