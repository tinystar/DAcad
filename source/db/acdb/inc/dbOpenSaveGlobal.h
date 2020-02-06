#ifndef _DB_OPEN_SAVE_GLOBAL_H_
#define _DB_OPEN_SAVE_GLOBAL_H_

#include "acdb.h"

class DwgFileIntImp;

ACHAR*& curOpenSaveFilename(void);
Acad::ErrorStatus getDWGTempFileName(const ACHAR*, const ACHAR*, ACHAR*);

void notifyAcDbDsDwgFilerBeingClosed(DwgFileIntImp*, bool);

#endif // _DB_OPEN_SAVE_GLOBAL_H_