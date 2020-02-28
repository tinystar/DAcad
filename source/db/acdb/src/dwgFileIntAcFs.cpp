#include "dwgFileIntAcFs.h"
#include "acfs.h"
#include "dbutil.h"
#include "dbOpenSaveGlobal.h"
#include "acfsglobal.h"
#include "rxcommon.h"
#include "acutmacro.h"
#include "acfsdefs.h"
#include "dwgShareInfo.h"
#include "acfsstream.h"
#include "dbGlobalFuncs.h"

const ACHAR* pszRcvAcad = ACRX_T("RcvAcad");

#define STREAM_OF_SEC(pSection)		((AcFsStream**)((char*)this + (pSection)->uStreamOffset))


StreamDescriptor DwgFileIntAcFs::smDefaultStreams[] = {
	{ ACFS_STREAM_HEADER,			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pHdrStream, 0x00000007, 0x00000001, 0x0000000000000800 },
	{ ACFS_STREAM_AUXHEADER,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pAuxHdrStream, 0x00000007, 0x00000002, 0x0000000000000800 },
	{ ACFS_STREAM_CLASSES,			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pClsStream, 0x00000007, 0x00000003, 0x0000000000000000 },
	{ ACFS_STREAM_PREVIEW,			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pPreviewStream, 0x00000000, 0x00000004, 0x0000000000000000 },
	{ ACFS_STREAM_HANDLES,			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pHdlStream, 0x00000007, 0x00000005, 0x0000000000000000 },
	{ ACFS_STREAM_TEMPLATE,			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pTmplStream, 0x00000007, 0x00000006, 0x0000000000000040 },
	{ ACFS_STREAM_OBJFREESPACE,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pObjFreeStream, 0x00000007, 0x00000007, 0x0000000000000000 },
	{ ACFS_STREAM_ACDBOBJECTS,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pObjsStream, 0x00000007, 0x00000008, 0x0000000000000000 },
	{ ACFS_STREAM_SHAREINFO,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pShInfoStream, 0x00000001, 0x00000009, 0x0000000000001000 },
	{ ACFS_STREAM_FILEDEPLIST,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pFDepLstStream, 0x00000000, 0x0000000A, 0x0000000000000000 },
	{ ACFS_STREAM_SUMMARYINFO,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pSmyInfoStream, 0x00000000, 0x0000000B, 0x0000000000000000 },
	{ ACFS_STREAM_VBAPROJECT,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pVBAPrjStream, 0x00000000, 0x0000000C, 0x0000000000000000 },
	{ ACFS_STREAM_REVHISTORY,		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pRevHstStream, 0x00000007, 0x0000000D, 0x0000000000001000 },
	{ ACFS_STREAM_APPINFO,			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pApInfoStream, 0x00000000, 0x0000000E, 0x0000000000000000 },
	{ ACFS_STREAM_APPINFOHISTORY,	(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pApInfHstStream, 0x00000000, 0x0000000F, 0x0000000000000000 },
	{ ACFS_STREAM_ACDSPROTOTYPE_1B, (Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pAcDsStream, 0x00000001, 0x00000010, 0x0000000000000000 },
	{ NULL, 0x0, 0x00000000, 0x00000000, 0x0000000000000000 },
};


DwgFileIntAcFs::DwgFileIntAcFs()
	: m_pAcFs(NULL)
	, m_hFile(NULL)
	, m_hTmpFile(NULL)
	, m_pFileName(NULL)
	, m_pTmpFile(NULL)
	, m_nCurSection(-1)
	, m_nObjSecDepth(0)
	, m_nSecReadState(-1)
	, m_uAcFsVer(4)
	, m_bPassword(Adesk::kFalse)
	, m_bSignature(Adesk::kFalse)
	, m_bNeedsRecovery(false)
	, m_bCrashSave(false)
	, m_bWriteable(false)
	, m_pCurFsStream(NULL)
	, m_pHdrStream(NULL)
	, m_pAuxHdrStream(NULL)
	, m_pClsStream(NULL)
	, m_pPreviewStream(NULL)
	, m_pHdlStream(NULL)
	, m_pObjFreeStream(NULL)
	, m_pTmplStream(NULL)
	, m_pObjsStream(NULL)
	, m_pShInfoStream(NULL)
	, m_pFDepLstStream(NULL)
	, m_pSmyInfoStream(NULL)
	, m_pVBAPrjStream(NULL)
	, m_pRevHstStream(NULL)
	, m_pApInfoStream(NULL)
	, m_pApInfHstStream(NULL)
	, m_pAcDsStream(NULL)
{
	AcDbFastDwgFiler::m_pDwgFileInt = this;
}

DwgFileIntAcFs::~DwgFileIntAcFs()
{

}

bool DwgFileIntAcFs::isWriteable(void)
{
	return m_bWriteable;
}

const ACHAR* DwgFileIntAcFs::getFileName(void)
{
	return m_pFileName;
}

const ACHAR* DwgFileIntAcFs::getAcDwgx(void)
{
	return NULL;
}

Adesk::UInt64 DwgFileIntAcFs::getFileSize(void)
{
	Adesk::UInt64 uFileSize = 0;
	m_pAcFs->GetFileSize(&uFileSize);	// 120
	return uFileSize;
}

Acad::ErrorStatus DwgFileIntAcFs::getError(void)
{
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::getFileTime(FILETIME* pTime1, FILETIME* pTime2, FILETIME* pTime3)
{
	int nRet = m_pAcFs->GetFileTime(pTime1, pTime2, pTime3);	// 40
	if (nRet != 0)
		return DbUtil::win32ToAcadError(nRet);

	return Acad::eOk;
}

bool DwgFileIntAcFs::hasPassword(void)
{
	return m_bPassword;
}

bool DwgFileIntAcFs::hasSignature(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool DwgFileIntAcFs::needsRecovery(void)
{
	return m_bNeedsRecovery;
}

Acad::ErrorStatus DwgFileIntAcFs::testPassword(const SecurityParams&)
{
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::getSignature(const SignatureInfo*&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

int DwgFileIntAcFs::getType(void)
{
	return 3;
}

void* DwgFileIntAcFs::getAFilePtr(void)
{
	return NULL;
}

AcFs* DwgFileIntAcFs::getAcFsPtr(void)
{
	return m_pAcFs;
}

void DwgFileIntAcFs::reopenToDenyWrites(Acad::ErrorStatus&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

unsigned int DwgFileIntAcFs::getDwgShareMode(void)
{
	DwgShareInfo info;
	unsigned int mode = 255;
	if (Acad::eOk == info.read(this))
		mode = info.getDwgShareMode();
	return mode;
}

Acad::ErrorStatus DwgFileIntAcFs::freeData(HANDLE* pFileHandle)
{
	notifyAcDbDsDwgFilerBeingClosed(this, true);
	int ret = m_pAcFs->CloseFile();
	if (pFileHandle)
	{
		*pFileHandle = m_hFile;
		m_hFile = NULL;
	}
	else if (m_hFile)
	{
		if (!CloseHandle(m_hFile))
			ret = GetLastError();

		m_hFile = NULL;
	}

	if (m_bIsXrefTmp)
	{
		DeleteFile(m_pFileName);
	}
	else if (m_hTmpFile)
	{
		closeAndDeleteTempRecoveryFile();
	}

	freeDeadAcFs();
	acdbDelString(m_pFileName);

	if (ret)
		return DbUtil::win32ToAcadError(ret);
	else
		return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::initForReading(const ACHAR* fileName,
												 HANDLE hFile,
												 unsigned int desiredAccess,
												 unsigned int shareMode,
												 const AC_BYTE* szHeader,
												 unsigned int uBytes, 
												 int(* pUnkFunc)(int, int, int, int), 
												 bool& bRet)
{
	Acad::ErrorStatus es = Acad::eOk;

	m_AccessMode = desiredAccess;
	m_ShareMode = shareMode;
	m_bWriteable = (desiredAccess & GENERIC_WRITE) != 0;

	setAcFsVer();
	setupAcFs(m_pAcFs);

	m_hFile = hFile;
	int nErrCode = m_pAcFs->OpenUsing(hFile, desiredAccess);		// 16
	if (nErrCode != 0)
	{
		if (NULL == pUnkFunc)
		{
			freeDeadAcFs();
			return DbUtil::win32ToAcadError(nErrCode);
		}

		ACHAR szTmpFile[MAX_PATH] = { 0 };
		es = getDWGTempFileName(pszRcvAcad, NULL, szTmpFile);
		if (es != Acad::eOk)
		{
			freeDeadAcFs();
			return es;
		}

		HANDLE hTmpFile = CreateFile(szTmpFile, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
		if (INVALID_HANDLE_VALUE == hTmpFile)
		{
			DWORD dwErr = GetLastError();
			es = Acad::eFilerError;
			if (dwErr)
				es = DbUtil::win32ToAcadError(dwErr);
			
			freeDeadAcFs();
			return es;
		}

		int nRet = AcFsRecover(hFile, hTmpFile, 128, NULL, NULL);
		if (nRet != 0)
		{
			CloseHandle(hTmpFile);
			DeleteFile(szTmpFile);
			freeDeadAcFs();
			return DbUtil::win32ToAcadError(nRet);
		}

		freeDeadAcFs();
		setupAcFs(m_pAcFs);
		nRet = m_pAcFs->OpenUsing(hTmpFile, GENERIC_READ);		// 16
		if (nRet != 0)
		{
			CloseHandle(hTmpFile);
			DeleteFile(szTmpFile);
			freeDeadAcFs();
			return DbUtil::win32ToAcadError(nRet);
		}

		m_hTmpFile = hTmpFile;
		m_pTmpFile = acStrdup(szTmpFile);
	}

	es = verifyDwgShareMode(desiredAccess, shareMode);
	if (es != Acad::eOk && NULL == pUnkFunc)
	{
		m_pAcFs->CloseFile();		// 56
		if (m_hTmpFile != NULL)
			closeAndDeleteTempRecoveryFile();
		freeDeadAcFs();
		return es;
	}

	for (StreamDescriptor* pSection = smDefaultStreams; ; ++pSection)
	{
		const ACHAR* pStreamName = pSection->pStreamName;
		if (NULL == pStreamName)
			break;

		if (m_pAcFs->OpenStream(pStreamName, 0, STREAM_OF_SEC(pSection)))
		{
			if ((AC_BYTE)pSection->uUnknown20 & 4)
			{
				if (NULL == pUnkFunc && 8 == pSection->uUnknown16)
				{
					m_pAcFs->CloseFile();		// 56
					if (m_hTmpFile != NULL)
						closeAndDeleteTempRecoveryFile();
					freeDeadAcFs();
					return Acad::eFileMissingSections;
				}
			}
		}
	}

	if (pUnkFunc != NULL)
		bRet = true;

	m_pFileName = acStrdup(fileName);
	m_pCurFsStream = NULL;
	m_bNeedsRecovery = szHeader[21] == -52;
	Adesk::UInt32* pHeader32 = (Adesk::UInt32*)szHeader;
	m_bPassword = (Adesk::Boolean)(pHeader32[6]);
	m_bSignature = (Adesk::Boolean)(pHeader32[7]);

	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::downgradeAfterSave(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::setCrashSave(void)
{
	m_bCrashSave = true;
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::openForWrite(const ACHAR* pFileName)
{
	m_AccessMode = GENERIC_READ | GENERIC_WRITE;
	m_ShareMode = 0;
	setAcFsVer();
	setupAcFs(m_pAcFs);
	m_bWriteable = true;
	m_pFileName = acStrdup(pFileName);
	SetLastError(0);
	m_hFile = CreateFile(pFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	int errCode = 0;
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		errCode = ERROR_OPEN_FAILED;
		if (int err = GetLastError())
			errCode = err;

		m_hFile = NULL;
		freeDeadAcFs();
		acdbDelString(m_pFileName);
		m_pCurFsStream = NULL;
		return DbUtil::win32ToAcadError(errCode);
	}

	errCode = m_pAcFs->OpenUsing(m_hFile, GENERIC_READ | GENERIC_WRITE);
	if (errCode)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
		freeDeadAcFs();
		acdbDelString(m_pFileName);
		m_pCurFsStream = NULL;
		return DbUtil::win32ToAcadError(errCode);
	}

	for (StreamDescriptor* pSection = smDefaultStreams; pSection->pStreamName; ++pSection)
	{
		if (pSection->uUnknown20 & 2)
		{
			int unk = pSection->uUnknown24;
			if (m_uAcFsVer < 6)
				unk = 0;
			errCode = createStream(pSection, unk);
			if (errCode)
				break;
		}
	}

	m_pCurFsStream = NULL;
	return DbUtil::win32ToAcadError(errCode);
}

Adesk::Int64 DwgFileIntAcFs::getObjectSectionSize(void)
{
	Adesk::UInt64 uSize = 0;
	m_pObjsStream->GetFileSize(&uSize);		// 48
	return uSize;
}

AcDbFastDwgFiler* DwgFileIntAcFs::fastDwgFiler(void)
{
	return this;
}

Acad::ErrorStatus DwgFileIntAcFs::setFileTime(const FILETIME* pTime1, const FILETIME* pTime2, const FILETIME* pTime3)
{
	int nRet = m_pAcFs->SetFileTime(pTime1, pTime2, pTime3);		// 32
	return DbUtil::win32ToAcadError(nRet);
}

void DwgFileIntAcFs::setSignatureInfo(Adesk::Boolean info)
{
	m_bSignature = info;
}

void DwgFileIntAcFs::setPasswordInfo(Adesk::Boolean info)
{
	m_bPassword = info;
}

Acad::ErrorStatus DwgFileIntAcFs::seekFile(Adesk::Int64 offset, int method)
{
	int ret = m_pCurFsStream->SetFilePointer(offset, method);		// 16
	if (ret)
		return DbUtil::win32ToAcadError(ret);

	return Acad::eOk;
}

Adesk::Int64 DwgFileIntAcFs::getFilePointer(void)
{
	Adesk::UInt64 uPos;
	m_pCurFsStream->GetFilePointer(&uPos);
	return uPos;
}

Adesk::UInt64 DwgFileIntAcFs::readBinaryBytes(void* pBytes, Adesk::UInt64 uBytes)
{
	if (NULL == m_pCurFsStream)
		return 0;	// ACAD返回68，对应Acad::eNoInputFiler，应该是错误

	m_pCurFsStream->ReadFile(pBytes, &uBytes);		// 8
	return uBytes;
}

Adesk::UInt64 DwgFileIntAcFs::writeBinaryBytes(void const*, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

Acad::ErrorStatus DwgFileIntAcFs::flushAndCheckForWriteErrors(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::startSectionRead(int sectionId)
{
	m_nCurSection = sectionId;
	m_nSecReadState = 0;
	return seekToSection(sectionId, 0);
}

Acad::ErrorStatus DwgFileIntAcFs::endSectionRead(void)
{
	m_nCurSection = -1;
	m_nSecReadState = -1;
	return seekToSection(-1, 0);
}

Acad::ErrorStatus DwgFileIntAcFs::startSectionSave(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::endSectionSave(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::startContiguousSectionSave(int, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::endContiguousSectionSave(int, const unsigned int*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

void* DwgFileIntAcFs::getCurrentAcFsStream(void)
{
	return m_pCurFsStream;
}

Acad::ErrorStatus DwgFileIntAcFs::pushObjectSectionRead(void)
{
	if (0 == m_nObjSecDepth)
	{
		m_nSecReadState = 0;
		m_pCurFsStream = m_pObjsStream;
		m_nCurSection = kSecObjects;
	}
	++m_nObjSecDepth;
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::popObjectSectionRead(void)
{
	--m_nObjSecDepth;
	if (0 == m_nObjSecDepth)
	{
		m_nCurSection = -1;
		m_nSecReadState = -1;
		m_pCurFsStream = NULL;
	}
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::readVBASection(void*, unsigned int, unsigned int&, unsigned int&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::writeVBASection(const void*, unsigned int, unsigned int, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::switchFromHandleToObjectSection(void)
{
	m_nCurSection = kSecObjects;
	m_pCurFsStream = m_pObjsStream;
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::returnFromObjectToHandleSection(void)
{
	m_nCurSection = kSecHandles;
	m_pCurFsStream = m_pHdlStream;
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::switchFromHandleToFreeSpaceSection(void)
{
	m_nCurSection = KSecObjFreeSpace;
	m_pCurFsStream = m_pObjFreeStream;
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::returnFromFreeSpaceToHandleSection(void)
{
	m_nCurSection = kSecHandles;
	m_pCurFsStream = m_pHdlStream;
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::startAcDsSectionRead(SaveObjectReadState*&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::endAcDsSectionRead(const SaveObjectReadState*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::prepareToWriteSections(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::omitSection(int)
{
	return Acad::eOk;
}

bool DwgFileIntAcFs::sectionExists(int sectionId)
{
	bool bExist = false;
	if (sectionId > kSecNone && sectionId <= kSecMax)
	{
		AcFsStream** ppStream = STREAM_OF_SEC(&smDefaultStreams[sectionId - 1]);
		AcFsStream* pStream = *ppStream;
		if (NULL == pStream)
		{
			m_pAcFs->OpenStream(smDefaultStreams[sectionId - 1].pStreamName, 0, ppStream);		// 280
			pStream = *ppStream;
		}

		bExist = pStream != NULL;
	}

	return bExist;
}

Acad::ErrorStatus DwgFileIntAcFs::getHdrHdr(void)
{
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::saveHdrHdr(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::saveUnknownSections(AcDbImpDatabase*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::readUnknownSections(AcDbImpDatabase*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::tryPassword(const wchar_t* password, CIPHER* cipher)
{
	return DwgFileIntImp::tryPassword(m_pAcFs, password, cipher);
}

Acad::ErrorStatus DwgFileIntAcFs::flushBuffers(void)
{
	if (NULL == m_pAcFs)
		return Acad::eNullObjectPointer;

	int ret = m_pAcFs->Flush();
	if (ret)
		return DbUtil::win32ToAcadError(ret);

	return Acad::eOk;
}

Acad::ErrorStatus DwgFileIntAcFs::getAcADPPackage(void)
{
	return Acad::eOk;
}

int DwgFileIntAcFs::getCurrentSection(void)
{
	return m_nCurSection;
}

void DwgFileIntAcFs::setAcFsVer(void)
{
	AcDb::AcDbDwgVersion ver;
	AcDb::MaintenanceReleaseVersion maintVer;
	getDwgVersion(ver, maintVer);		// 80
	if (ver <= AcDb::kDHL_1021)
		m_uAcFsVer = 6;
}

void DwgFileIntAcFs::setupAcFs(AcFs*& pAcFs)
{
	if (4 == m_uAcFsVer)
		pAcFs = AcFsNewClass();
	else
		pAcFs = AcFs6NewClass();

	pAcFs->SetPrivateHeaderSize(128);		// 208
	pAcFs->SetCompressionType(2);			// 128
	pAcFs->SetCompressionLevel(1);			// 144
	pAcFs->SetBlockSize(m_uAcFsVer > 5 ? 63488 : 29696);		// 160
	pAcFs->SetAlignment(32);				// 176
	pAcFs->SetMaxCache(0x400000);			// 168
}

void DwgFileIntAcFs::freeDeadAcFs(void)
{
	// 逆向的代码感觉存在错误
	//AcDb::AcDbDwgVersion dwgVer;
	//AcDb::MaintenanceReleaseVersion maintVer;
	//getDwgVersion(dwgVer, maintVer);
	//if (dwgVer > AcDb::kDHL_1800)
	//	AcFs6DeleteClass(m_pAcFs);
	//else
	//	AcFsDeleteClass(m_pAcFs);
	if (6 == m_uAcFsVer)
		AcFs6DeleteClass(m_pAcFs);
	else
		AcFsDeleteClass(m_pAcFs);
	m_pAcFs = NULL;
}

Acad::ErrorStatus DwgFileIntAcFs::verifyDwgShareMode(unsigned int desiredAccess, unsigned int shareMode)
{
	unsigned int uShareMode = getDwgShareMode();	// 256
	if (!(desiredAccess & GENERIC_WRITE))
	{
		if (!(uShareMode & FILE_SHARE_READ))
		{
			return (desiredAccess & GENERIC_WRITE) ? Acad::eDwgShareWriteAccess : Acad::eDwgShareReadAccess;
		}
	}
	else if ((uShareMode & (FILE_SHARE_READ | FILE_SHARE_WRITE)) != (FILE_SHARE_READ | FILE_SHARE_WRITE))
	{
		return (desiredAccess & GENERIC_WRITE) ? Acad::eDwgShareWriteAccess : Acad::eDwgShareReadAccess;
	}

	if (shareMode & FILE_SHARE_WRITE)
		return Acad::eOk;
	else if (uShareMode & FILE_SHARE_DELETE)
		return Acad::eOk;
	else
		return Acad::eDwgShareDemandLoad;
}

void DwgFileIntAcFs::closeAndDeleteTempRecoveryFile(void)
{
	CloseHandle(m_hTmpFile);
	m_hTmpFile = NULL;
	if (m_pTmpFile)
	{
		DeleteFile(m_pTmpFile);
		acdbDelString(m_pTmpFile);
	}
}

Acad::ErrorStatus DwgFileIntAcFs::seekToSection(int sectionId, int createIfNotFound)
{
	if (-1 == sectionId)
	{
		m_pCurFsStream = NULL;
		return Acad::eOk;
	}

	Acad::ErrorStatus es = Acad::eInvalidInput;
	if (sectionId > kSecNone && sectionId <= kSecMax)
	{
		AcFsStream** ppStream = STREAM_OF_SEC(&smDefaultStreams[sectionId - 1]);
		AcFsStream* pStream = *ppStream;
		if (NULL == pStream && 1 == createIfNotFound)
		{
			long createmask = 0x10F7;
			if (!BitTest(&createmask, sectionId - 1))
			{
				createStream(&smDefaultStreams[sectionId - 1], smDefaultStreams[sectionId - 1].uUnknown24);
				ppStream = STREAM_OF_SEC(&smDefaultStreams[sectionId - 1]);
				pStream = *ppStream;
			}
		}

		if (pStream)
		{
			m_pCurFsStream = pStream;
			es = Acad::eOk;
			pStream->SetFilePointer(0, 0);		// 16
		}
	}

	return es;
}

int DwgFileIntAcFs::createStream(const StreamDescriptor* pDescriptor, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}
