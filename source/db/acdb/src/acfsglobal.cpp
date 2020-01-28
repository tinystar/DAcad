#include "acfsglobal.h"
#include "acutmacro.h"
#include "acfsi.h"
#include "acfs6.h"
#include "acfsglobalmem.h"


int AcFsRecover(HANDLE, HANDLE, int, int(*)(void *, int, int), void*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

AcFs* AcFsNewClass()
{
	AcFsHeap* pHeap = GetAcFsHeap();

	AcFsI* pAcFs = new AcFsI();
	if (pAcFs != NULL)
		pAcFs->m_pFsHeap = pHeap;

	return pAcFs;
}

AcFs* AcFs6NewClass()
{
	return new AcFs6();
}

int iFileSeek(HANDLE hFile, Adesk::Int64 nPos)
{
	Adesk::Int32 nHigh = nPos >> 32;
	Adesk::Int32 nLow = (Adesk::Int32)nPos;
	Adesk::UInt32 uRet = SetFilePointer(hFile, nLow, &nHigh, FILE_BEGIN);
	if (INVALID_SET_FILE_POINTER == uRet)
		return GetLastError();

	return 0;
}

AcFsHeap* GetAcFsHeap(void)
{
	return globalMemory.GetHeap();
}
