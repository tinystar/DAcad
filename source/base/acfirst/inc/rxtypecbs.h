#ifndef _AC_RX_TYPECBS_H_
#define _AC_RX_TYPECBS_H_

#include "AdAChar.h"

class AcRxClass;

typedef void (*NewClassRegCallback)(AcRxClass* pClass, const ACHAR* className);
typedef bool (*DelClassRegCallback)(AcRxClass* pClass);

extern NewClassRegCallback pNewClassCallback;
extern DelClassRegCallback pDeleteClassCallback;


class AcRxTypeSysCallbacks
{
public:
	static void setCallback_newClassCallback(NewClassRegCallback pCallback);
	static void setCallback_deleteClassCallback(DelClassRegCallback pCallback);
};

#endif // _AC_RX_TYPECBS_H_