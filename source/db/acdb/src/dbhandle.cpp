#include "dbhandle.h"


void AcDbHandle::setValueBytes(Adesk::UInt8 uSize, const Adesk::UInt8* pBytes)
{
	mLow = 0;
	mHigh = 0;
	while (uSize > 0)
	{
		*((Adesk::UInt8*)this + uSize - 1) = *pBytes++;
		--uSize;
	}
}
