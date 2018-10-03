#ifndef _AC_RX_COMMON_H_
#define _AC_RX_COMMON_H_

class AcRxDictionary;


AcRxDictionary* newAcRxDictionary(void);
AcRxDictionary* newRxOpaqueDictionary(unsigned int a1, unsigned __int8 a2, unsigned __int8 a3, unsigned __int8 a4);

AcRxDictionary* acrxSysClasses(void);
void delSysClasses(void);

const ACHAR* acStrdup(const ACHAR* pszStr);

#endif // _AC_RX_COMMON_H_