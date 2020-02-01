#include "dwgFileIntAcFs.h"
#include "acfs.h"
#include "dbutil.h"
#include "dbOpenSaveGlobal.h"
#include "acfsglobal.h"
#include "rxcommon.h"
#include "acutmacro.h"

const ACHAR* pszRcvAcad = ACRX_T("RcvAcad");

#define STREAM_OF_SEC(pSection)		((AcFsStream**)((char*)this + pSection->uStreamOffset))


DwgFileIntAcFs::FileSection DwgFileIntAcFs::smDefaultStreams[] = {
	{ ACRX_T("AcDb:Header"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pHdrStream,		0x00000007, 0x00000001, 0x0000000000000800 },
	{ ACRX_T("AcDb:AuxHeader"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pAuxHdrStream,	0x00000007, 0x00000002, 0x0000000000000800 },
	{ ACRX_T("AcDb:Classes"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pClsStream,		0x00000007, 0x00000003, 0x0000000000000000 },
	{ ACRX_T("AcDb:Preview"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pPreviewStream,	0x00000000, 0x00000004, 0x0000000000000000 },
	{ ACRX_T("AcDb:Handles"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pHdlStream,		0x00000007, 0x00000005, 0x0000000000000000 },
	{ ACRX_T("AcDb:Template"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pTmplStream,		0x00000007, 0x00000006, 0x0000000000000040 },
	{ ACRX_T("AcDb:ObjFreeSpace"),		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pObjFreeStream,	0x00000007, 0x00000007, 0x0000000000000000 },
	{ ACRX_T("AcDb:AcDbObjects"),		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pObjsStream,		0x00000007, 0x00000008, 0x0000000000000000 },
	{ ACRX_T("AcDb:ShareInfo"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pShInfoStream,	0x00000001, 0x00000009, 0x0000000000001000 },
	{ ACRX_T("AcDb:FileDepList"),		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pFDepLstStream,	0x00000000, 0x0000000A, 0x0000000000000000 },
	{ ACRX_T("AcDb:SummaryInfo"),		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pSmyInfoStream,	0x00000000, 0x0000000B, 0x0000000000000000 },
	{ ACRX_T("AcDb:VBAProject"),		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pVBAPrjStream,	0x00000000, 0x0000000C, 0x0000000000000000 },
	{ ACRX_T("AcDb:RevHistory"),		(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pRevHstStream,	0x00000007, 0x0000000D, 0x0000000000001000 },
	{ ACRX_T("AcDb:AppInfo"),			(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pApInfoStream,	0x00000000, 0x0000000E, 0x0000000000000000 },
	{ ACRX_T("AcDb:AppInfoHistory"),	(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pApInfHstStream,	0x00000000, 0x0000000F, 0x0000000000000000 },
	{ ACRX_T("AcDb:AcDsPrototype_1b"),	(Adesk::ULongPtr)&((DwgFileIntAcFs*)0)->m_pAcDsStream,		0x00000001, 0x00000010, 0x0000000000000000 },
	{ NULL, 0x0, 0x00000000, 0x00000000, 0x0000000000000000 },
};


DwgFileIntAcFs::DwgFileIntAcFs()
	: m_pAcFs(NULL)
	, m_hFile(NULL)
	, m_hTmpFile(NULL)
	, m_pFileName(NULL)
	, m_pTmpFile(NULL)
	, m_uUnk212(4)
	, m_uUnk232(0)
	, m_uUnk236(0)
	, m_bUnk256(false)
	, m_bUnk258(false)
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

}

DwgFileIntAcFs::~DwgFileIntAcFs()
{

}

bool DwgFileIntAcFs::isWriteable(void)
{
	return m_bUnk258;
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
	return m_uUnk232 != 0;
}

bool DwgFileIntAcFs::hasSignature(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

bool DwgFileIntAcFs::needsRecovery(void)
{
	return m_bUnk256;
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

void* DwgFileIntAcFs::getAcFsPtr(void)
{
	return m_pAcFs;
}

void DwgFileIntAcFs::reopenToDenyWrites(Acad::ErrorStatus&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

unsigned int DwgFileIntAcFs::getDwgShareMode(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

Acad::ErrorStatus DwgFileIntAcFs::freeData(void**)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
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
	m_bUnk258 = (desiredAccess >> 30) & 1;

	setAcFsVer();
	setupAcFs(m_pAcFs);

	m_hFile = hFile;
	int nErrCode = m_pAcFs->OpenUsing(hFile, shareMode);		// 16
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

	for (FileSection* pSection = smDefaultStreams; ; ++pSection)
	{
		const ACHAR* pSecName = pSection->pSectionName;
		if (NULL == pSecName)
			break;

		if (m_pAcFs->OpenStream(pSecName, 0, STREAM_OF_SEC(pSection)))
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
	m_bUnk256 = szHeader[21] == -52;
	m_uUnk232 = ((Adesk::UInt32*)szHeader)[6];
	m_uUnk236 = ((Adesk::UInt32*)szHeader)[7];

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

Acad::ErrorStatus DwgFileIntAcFs::openForWrite(const ACHAR*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Adesk::Int64 DwgFileIntAcFs::getObjectSectionSize(void)
{
	//m_pObjsStream		// 48
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

AcDbDwgFiler* DwgFileIntAcFs::fastDwgFiler(void)
{
	return this;
}

Acad::ErrorStatus DwgFileIntAcFs::setFileTime(const FILETIME* pTime1, const FILETIME* pTime2, const FILETIME* pTime3)
{
	int nRet = m_pAcFs->SetFileTime(pTime1, pTime2, pTime3);		// 32
	return DbUtil::win32ToAcadError(nRet);
}

void DwgFileIntAcFs::setSignatureInfo(unsigned int info)
{
	m_uUnk236 = info;
}

void DwgFileIntAcFs::setPasswordInfo(unsigned int info)
{
	m_uUnk232 = info;
}

Acad::ErrorStatus DwgFileIntAcFs::seekFile(Adesk::Int64, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Adesk::Int64 DwgFileIntAcFs::getFilePointer(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

Acad::ErrorStatus DwgFileIntAcFs::readBinaryBytes(void*, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
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

Acad::ErrorStatus DwgFileIntAcFs::startSectionRead(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::endSectionRead(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
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
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::popObjectSectionRead(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
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
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::returnFromObjectToHandleSection(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::switchFromHandleToFreeSpaceSection(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::returnFromFreeSpaceToHandleSection(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
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
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

bool DwgFileIntAcFs::sectionExists(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return false;
}

Acad::ErrorStatus DwgFileIntAcFs::getHdrHdr(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
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

Acad::ErrorStatus DwgFileIntAcFs::tryPassword(const wchar_t*, CIPHER*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::flushBuffers(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileIntAcFs::getAcADPPackage(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
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
		m_uUnk212 = 6;
}

void DwgFileIntAcFs::setupAcFs(AcFs*& pAcFs)
{
	if (4 == m_uUnk212)
		pAcFs = AcFsNewClass();
	else
		pAcFs = AcFs6NewClass();

	pAcFs->SetPrivateHeaderSize(128);		// 208
	pAcFs->SetCompressionType(2);			// 128
	pAcFs->SetCompressionLevel(1);			// 144
	pAcFs->SetBlockSize(m_uUnk212 > 5 ? 63488 : 29696);		// 160
	pAcFs->SetAlignment(32);				// 176
	pAcFs->SetMaxCache(0x400000);			// 168
}

void DwgFileIntAcFs::freeDeadAcFs(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Acad::ErrorStatus DwgFileIntAcFs::verifyDwgShareMode(unsigned int desiredAccess, unsigned int shareMode)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

void DwgFileIntAcFs::closeAndDeleteTempRecoveryFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
