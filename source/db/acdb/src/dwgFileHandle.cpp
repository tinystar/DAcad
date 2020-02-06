#include "dwgFileHandle.h"
#include "acutmacro.h"
#include "dwgFileImpBase.h"


Acad::ErrorStatus AcDwgFileHandle::close(AcDwgFileHandle*& pDwgFileHandle)
{
	if (NULL == pDwgFileHandle)
		return Acad::eOk;

	DwgFileImpBase* pDwgFileImp = (DwgFileImpBase*)pDwgFileHandle;
	pDwgFileImp->subCloseFile();
	Acad::ErrorStatus es = pDwgFileImp->freeData(NULL);		// 264
	delete pDwgFileImp;
	pDwgFileHandle = NULL;
	if (es != Acad::eOk)
		return Acad::eFilerError;

	return Acad::eOk;
}

Acad::ErrorStatus AcDwgFileHandle::open(const ACHAR* pFileName, AcDwgFileHandle*& pDwgFileHandle, int mode)
{
	Acad::ErrorStatus es;
	pDwgFileHandle = open(pFileName, mode, &es, NULL);
	if (es != Acad::eOk)
		pDwgFileHandle = NULL;
	return es;
}

AcDwgFileHandle* AcDwgFileHandle::open(const ACHAR* pFileName, int mode, Acad::ErrorStatus* pes, int(*pUnkFunc)(int, int, int, int))
{
	DwgFileImpBase* pDwgFileHandle = NULL;
	if (mode & 0x10)
	{
		if (pes)
			*pes = Acad::eInvalidInput;
	}
	else
	{
		unsigned int desiredAccess = (mode | 0xFFFFFFFE) << 30;
		unsigned int shareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
		if (!(mode & 8))
			shareMode = 2 * ((mode & 3) == 0) + 1;
		Acad::ErrorStatus es;
		pDwgFileHandle = DwgFileImpBase::openWithModes(pFileName, desiredAccess, shareMode, (mode & 0x20) >> 5, (mode & 4) >> 2, es, pUnkFunc);
		if (pDwgFileHandle && (mode & 0x1000))
			pDwgFileHandle->setIsXRef();		// 240
		if (pes)
			*pes = es;
	}

	return pDwgFileHandle;
}
