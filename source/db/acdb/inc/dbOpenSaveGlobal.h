#ifndef _DB_OPEN_SAVE_GLOBAL_H_
#define _DB_OPEN_SAVE_GLOBAL_H_

#include "acdb.h"

ACHAR*& curOpenSaveFilename(void);
Acad::ErrorStatus getDWGTempFileName(const ACHAR*, const ACHAR*, ACHAR*);

#endif // _DB_OPEN_SAVE_GLOBAL_H_