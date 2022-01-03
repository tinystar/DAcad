#ifndef _DB_GLOBAL_FUNCS_H_
#define _DB_GLOBAL_FUNCS_H_

#include "acdb.h"

class AcDbHandleTable;
class AcDbDwgFiler;
class HandleTableUpdater;

void acdbDelString(ACHAR*&);

Acad::ErrorStatus dwgInHandleOffsets(AcDbHandleTable&, AcDbDwgFiler*, HandleTableUpdater*, bool&);

#endif // _DB_GLOBAL_FUNCS_H_