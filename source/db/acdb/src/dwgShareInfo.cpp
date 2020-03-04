#include "dwgShareInfo.h"
#include "acfsstream.h"
#include "dwgFileInt.h"
#include "acfs.h"
#include "acfsdefs.h"
#include "acutmacro.h"

static Acad::ErrorStatus getStream(const AcDwgFileHandle* pFileHandle, AcFsStream*& pStream, bool bCreateIfNotFound)
{
	pStream = NULL;
	if (NULL == pFileHandle)
		return Acad::eNullHandle;

	DwgFileInt* pFileInt = (DwgFileInt*)pFileHandle;
	AcFs* pAcFs = pFileInt->getAcFsPtr();		// 224
	if (NULL == pAcFs)
		return Acad::eNullObjectPointer;

	int ret = pAcFs->OpenStream(ACFS_STREAM_SHAREINFO, 0, &pStream);		// 280
	if (ERROR_NOT_FOUND == ret && bCreateIfNotFound)
	{
		AcFsStream* pNewStream = NULL;
		ret = pAcFs->CreateStream(ACFS_STREAM_SHAREINFO, NULL, GENERIC_WRITE, 1, &pNewStream, 0, 0, 0);
		if (ERROR_SUCCESS == ret)
		{
			pStream = pNewStream;
			return Acad::eOk;
		}
	}

	if (ERROR_NOT_FOUND == ret)
		return Acad::eFileNotFound;
	else if (ERROR_SUCCESS == ret)
		return Acad::eOk;
	else
		return Acad::eFilerError;
}


DwgShareInfo::DwgShareInfo(void)
	: m_uUnk0(255)
	, m_uUnk4(1)
	, m_uUnk8(0)
	, m_pUnk16(NULL)
{

}

DwgShareInfo::~DwgShareInfo()
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Acad::ErrorStatus DwgShareInfo::read(const AcDwgFileHandle* pFileHandle)
{
	AcFsStream* pStream = NULL;
	Acad::ErrorStatus es = getStream(pFileHandle, pStream, false);
	if (es != Acad::eOk)
		return es;

	if (pStream != NULL)
	{
		if (pStream->SetFilePointer(0, 0) != ERROR_SUCCESS)
			return Acad::eInvalidInput;

		Adesk::UInt64 uRdBytes = 4;
		pStream->ReadFile(&m_uUnk4, &uRdBytes);

		unsigned int unk;
		pStream->ReadFile(&unk, &uRdBytes);

		pStream->ReadFile(&m_uUnk0, &uRdBytes);

		AC_ASSERT_NOT_IMPLEMENTED();
	}

	return es;
}

Acad::ErrorStatus DwgShareInfo::write(const AcDwgFileHandle*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

unsigned int DwgShareInfo::getDwgShareMode(void)
{
	return m_uUnk0;
}
