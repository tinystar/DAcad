#include "dwgFileImpBase.h"
#include "dbutil.h"
#include "acutmacro.h"
#include "dbdbi.h"
#include "dwgFileIntAcFs.h"
#include "rxcommon.h"

enum FileIntType
{
	eNone = 0,
	eUnknown,
	eAFileR12,
	eAFile,
	eAcFs,
	eADP
};

enum FileFormatType
{
	eValidFileFormat = 0,
	eObsoleteFileFormat,
	eUnsupportedFileFormat
};

struct AcadId
{
	const ACHAR*  pszDwgVer;
	const ACHAR*  pszAcadVer;
	Adesk::UInt32 dwgVersion;
	Adesk::UInt32 fileFormatType;
} acadid[] = {
	{ ACRX_T("MC0.0"),  ACRX_T("Pre-2.5"), AcDb::kDHL_MC0_0,  eObsoleteFileFormat },
	{ ACRX_T("AC1.2"),  ACRX_T("Pre-2.5"), AcDb::kDHL_AC1_2,  eObsoleteFileFormat },
	{ ACRX_T("AC1.40"), ACRX_T("Pre-2.5"), AcDb::kDHL_AC1_40, eObsoleteFileFormat },
	{ ACRX_T("AC1.50"), ACRX_T("Pre-2.5"), AcDb::kDHL_AC1_50, eValidFileFormat },
	{ ACRX_T("AC2.10"), ACRX_T("Pre-2.5"), AcDb::kDHL_AC2_10, eValidFileFormat },
	{ ACRX_T("AC2.20"), ACRX_T("Pre-2.5"), AcDb::kDHL_AC2_20, eUnsupportedFileFormat },
	{ ACRX_T("AC2.21"), ACRX_T("Pre-2.5"), AcDb::kDHL_AC2_21, eValidFileFormat },
	{ ACRX_T("AC2.22"), ACRX_T("Pre-2.5"), AcDb::kDHL_AC2_22, eValidFileFormat },
	{ ACRX_T("AC1001"), ACRX_T("Pre-2.5"), AcDb::kDHL_1001,   eValidFileFormat },
	{ ACRX_T("AC1002"), ACRX_T("2.5"),	   AcDb::kDHL_1002,   eValidFileFormat },
	{ ACRX_T("AC1003"), ACRX_T("2.6"),     AcDb::kDHL_1003,   eValidFileFormat },
	{ ACRX_T("AC1004"), ACRX_T("9"),       AcDb::kDHL_1004,   eValidFileFormat },
	{ ACRX_T("AC1005"), ACRX_T("10"),      AcDb::kDHL_1005,   eUnsupportedFileFormat },
	{ ACRX_T("AC1006"), ACRX_T("10"),      AcDb::kDHL_1006,   eValidFileFormat },
	{ ACRX_T("AC1007"), ACRX_T("11"),      AcDb::kDHL_1007,   eValidFileFormat },
	{ ACRX_T("AC1008"), ACRX_T("11"),      AcDb::kDHL_1008,   eUnsupportedFileFormat },
	{ ACRX_T("AC1009"), ACRX_T("12"),      AcDb::kDHL_1009,   eValidFileFormat },
	{ ACRX_T("AC1010"), ACRX_T("13"),      AcDb::kDHL_1010,   eUnsupportedFileFormat },
	{ ACRX_T("AC1011"), ACRX_T("13"),      AcDb::kDHL_1011,   eUnsupportedFileFormat },
	{ ACRX_T("AC1012"), ACRX_T("13"),      AcDb::kDHL_1012,   eValidFileFormat },
	{ ACRX_T("AC1013"), ACRX_T("14"),      AcDb::kDHL_1013,   eValidFileFormat },
	{ ACRX_T("AC1014"), ACRX_T("14"),      AcDb::kDHL_1014,   eValidFileFormat },
	{ ACRX_T("AC1500"), ACRX_T("2000"),    AcDb::kDHL_1500,   eValidFileFormat },
	{ ACRX_T("AC1015"), ACRX_T("2000"),    AcDb::kDHL_1015,   eValidFileFormat },
	{ ACRX_T("AC402a"), ACRX_T("2004"),    AcDb::kDHL_1800a,  eUnsupportedFileFormat },
	{ ACRX_T("AC402b"), ACRX_T("2004"),    AcDb::kDHL_1800,   eValidFileFormat },
	{ ACRX_T("AC1018"), ACRX_T("2004"),    AcDb::kDHL_1800,   eValidFileFormat },
	{ ACRX_T("AC701a"), ACRX_T("2007"),    AcDb::kDHL_2100a,  eValidFileFormat },
	{ ACRX_T("AC1021"), ACRX_T("2007"),    AcDb::kDHL_1021,   eValidFileFormat },
	{ ACRX_T("AC802@"), ACRX_T("2010"),    AcDb::kDHL_2400a,  eUnsupportedFileFormat },
	{ ACRX_T("AC1024"), ACRX_T("2010"),    AcDb::kDHL_1024,   eValidFileFormat },
	{ ACRX_T("AC901@"), ACRX_T("2013"),    AcDb::kDHL_2700a,  eUnsupportedFileFormat },
	{ ACRX_T("AC1027"), ACRX_T("2013"),    AcDb::kDHL_1027,   eValidFileFormat }
};


Adesk::UInt32 DwgFileImpBase::smNumberOfOpenDwgFiles = 0;

DwgFileImpBase::DwgFileImpBase()
	: m_pImpDb(NULL)
	, m_codePage((code_page_id)-1)
	, m_dwgVer1(AcDb::kDHL_Unknown)
	, m_maintVer1(AcDb::kMReleaseUnknown)
	, m_dwgVer2(AcDb::kDHL_Unknown)
	, m_maintVer2(AcDb::kMReleaseUnknown)
	, m_dwgVer3(AcDb::kDHL_Unknown)
	, m_maintVer3(AcDb::kMReleaseUnknown)
	, m_AccessMode(0)
	, m_ShareMode(0)
	, m_bValid(false)
	, m_bIsXrefTmp(false)
	, m_bUnknown54(false)
	, m_pOrgXRefName(NULL)
	, m_bIsXref(false)
	, m_bUnk65(false)
	, m_pszFile(NULL)
{
	++smNumberOfOpenDwgFiles;
}

DwgFileImpBase::~DwgFileImpBase()
{
	--smNumberOfOpenDwgFiles;
}

bool DwgFileImpBase::isXRef(void)
{
	return m_bIsXref;
}

bool DwgFileImpBase::isXRefTemp(void)
{
	return m_bIsXrefTmp;
}

const ACHAR* DwgFileImpBase::getOriginalXRefName(void)
{
	return m_pOrgXRefName;
}

AcDbDatabase* DwgFileImpBase::getDatabase(void)
{
	return m_pImpDb->apiDatabase();
}

code_page_id DwgFileImpBase::getCodePage(void)
{
	return m_codePage;
}

AcDb::AcDbDwgVersion DwgFileImpBase::getDwgVersion(void)
{
	return m_dwgVer1;	// 20
}

Acad::ErrorStatus DwgFileImpBase::getDwgVersion(AcDb::AcDbDwgVersion& ver, AcDb::MaintenanceReleaseVersion& maintVer)
{
	ver = m_dwgVer1;		// 20
	maintVer = m_maintVer1;	// 24
	return Acad::eOk;
}

bool DwgFileImpBase::isValid(void)
{
	return m_bValid;
}

unsigned int DwgFileImpBase::getAccessMode(void)
{
	return m_AccessMode;
}

unsigned int DwgFileImpBase::getShareMode(void)
{
	return m_ShareMode;
}

bool DwgFileImpBase::hasPassword(void)
{
	return false;
}

bool DwgFileImpBase::hasSignature(void)
{
	return false;
}

Acad::ErrorStatus DwgFileImpBase::testPassword(const SecurityParams&)
{
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus DwgFileImpBase::getSignature(const SignatureInfo*& pInfo)
{
	pInfo = NULL;
	return Acad::eNotImplementedYet;
}

void DwgFileImpBase::setIsXRef(void)
{
	m_bIsXref = true;
}

void DwgFileImpBase::setIsXRefTemp(const ACHAR* pFileName)
{
	if (pFileName)
	{
		m_bIsXrefTmp = true;
		m_pOrgXRefName = acStrdup(pFileName);
	}
}

unsigned int DwgFileImpBase::getDwgShareMode(void)
{
	return 255;
}

Acad::ErrorStatus DwgFileImpBase::attachDb(AcDbImpDatabase* pDb, bool)
{
	m_pImpDb = pDb;
	return Acad::eOk;
}

Acad::ErrorStatus DwgFileImpBase::detachDb(AcDbImpDatabase*, bool, bool)
{
	m_pImpDb = NULL;
	return Acad::eOk;
}

DwgFileImpBase* DwgFileImpBase::openWithModes(const ACHAR* fileName,
											 unsigned int desiredAccess, 
											 unsigned int shareMode, 
											 bool b1, 
											 bool bForceOpen, 
											 Acad::ErrorStatus& esRet, 
											 int(* pUnkFunc)(int, int, int, int))
{
	SetLastError(0);
	HANDLE hFile = CreateFile(fileName, desiredAccess, shareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		DWORD dwErr = GetLastError();
		esRet = Acad::eFilerError;
		if (dwErr)
			esRet = DbUtil::win32ToAcadError(dwErr);
		return NULL;
	}

	DWORD dwType = GetFileType(hFile);
	if (FILE_TYPE_CHAR == dwType || FILE_TYPE_PIPE == dwType)
	{
		CloseHandle(hFile);
		esRet = Acad::eFileSystemErr;
		return NULL;
	}

	int nRetIntType = eNone;
	bool bVerExist = false;
	int nFormatType = eUnsupportedFileFormat;
	int nCodePage = -1;
	AcDb::AcDbDwgVersion ver1 = AcDb::kDHL_Unknown;
	AcDb::AcDbDwgVersion ver2 = AcDb::kDHL_Unknown;
	AcDb::AcDbDwgVersion ver3 = AcDb::kDHL_Unknown;
	AcDb::MaintenanceReleaseVersion maintVer1 = AcDb::kMReleaseUnknown;
	AcDb::MaintenanceReleaseVersion maintVer2 = AcDb::kMReleaseUnknown;
	AcDb::MaintenanceReleaseVersion maintVer3 = AcDb::kMReleaseUnknown;

	DWORD dwBytesRead = 0;
	AC_BYTE szHeader[64] = { 0 };
	if (ReadFile(hFile, szHeader, 64, &dwBytesRead, NULL) && 64 == dwBytesRead)
		nRetIntType = readversion(szHeader, 64, bVerExist, nFormatType, nCodePage, ver1, maintVer1, ver2, maintVer2, ver3, maintVer3);

	bool bInvalid = true;
	if (eNone == nRetIntType)
	{
		esRet = Acad::eBadDwgHeader;
	}
	else if (eUnknown == nRetIntType)
	{
		esRet = Acad::eNotImplementedYet;
		nRetIntType = eAcFs;
	}
	else
	{
		if (bVerExist)
		{
			if (eUnsupportedFileFormat == nFormatType)
				esRet = Acad::eUnsupportedFileFormat;
			else if (eObsoleteFileFormat == nFormatType)
				esRet = Acad::eObsoleteFileFormat;
			else
				bInvalid = false;
		}
		else
		{
			esRet = Acad::eBadDwgHeader;
		}
	}

	if (bForceOpen)
		bInvalid = false;

	if (bInvalid)
	{
		CloseHandle(hFile);
		return NULL;
	}

	DwgFileImpBase* pDwgFileBase = NULL;
	switch (nRetIntType)
	{
	case eAFileR12:
		pDwgFileBase = newDwgFileIntAFileR12();
		break;
	case eAcFs:
		pDwgFileBase = newDwgFileIntAcFs();
		break;
	case eADP:
		pDwgFileBase = newDwgFileIntADP();
		break;
	default:
		pDwgFileBase = newDwgFileIntAFile();
		break;
	}

	if (NULL == pDwgFileBase)
	{
		CloseHandle(hFile);
		return NULL;
	}

	pDwgFileBase->m_dwgVer2 = ver2;
	pDwgFileBase->m_maintVer2 = maintVer2;
	pDwgFileBase->m_dwgVer3 = ver3;
	pDwgFileBase->m_maintVer3 = maintVer3;
	pDwgFileBase->m_codePage = (code_page_id)nCodePage;

	pDwgFileBase->m_bValid = false;
	if (bVerExist && eValidFileFormat == nFormatType)
		pDwgFileBase->m_bValid = true;
	if (b1)
		pDwgFileBase->m_bUnknown54 = true;

	pDwgFileBase->m_dwgVer1 = ver1;
	pDwgFileBase->m_maintVer1 = maintVer1;

	bool bUnkRet = false;
	Acad::ErrorStatus es = pDwgFileBase->initForReading(fileName,
														hFile, 
														desiredAccess, 
														shareMode, 
														szHeader, 
														64, 
														pUnkFunc, 
														bUnkRet);				// 272

	esRet = es;
	if (es != Acad::eOk && (NULL == pUnkFunc || !bUnkRet))
	{
		CloseHandle(hFile);
		delete pDwgFileBase;			// 200
		return NULL;
	}

	return pDwgFileBase;
}

int DwgFileImpBase::readversion(const AC_BYTE* szHeader,	// rdi
								unsigned int uBytes,		// rsi
								bool& bExistVer,			// rdx
								int& nFileFormatType,		// rcx
								int& nCodePage,				// r8
								AcDb::AcDbDwgVersion& ver1,		// r9
								AcDb::MaintenanceReleaseVersion& maintver1,		// arg0
								AcDb::AcDbDwgVersion& ver2,		// arg1
								AcDb::MaintenanceReleaseVersion& maintver2,		// arg2
								AcDb::AcDbDwgVersion& ver3,		// arg3
								AcDb::MaintenanceReleaseVersion& maintver3)		// arg4
{
	nFileFormatType = eUnsupportedFileFormat;
	bExistVer = false;

	AC_BYTE byt12 = szHeader[12];
	if (0 == byt12)
		byt12 = (szHeader[13] < 1) + 1;

	int idx = AC_COUNT_OF(acadid) - 1;
	for (; idx >= 0; --idx)
	{
		const ACHAR* pDwgVer = acadid[idx].pszDwgVer;
		const AC_BYTE* pHeader = szHeader;
		bool bEqual = true;
		do
		{
			ACHAR ch = *pHeader;
			if (ch != *pDwgVer)
			{
				bEqual = false;
				break;
			}
			else if (0 == ch)
			{
				break;
			}

			++pHeader;
			++pDwgVer;
		} while (true);

		if (bEqual)
			break;
	}

	maintver1 = AcDb::kMRelease0;

	if (idx < 0)
	{
		int nRet = eNone;
		if (szHeader[0] != 'A'
			|| szHeader[1] != 'C'
			|| szHeader[2] != '1'
			|| szHeader[3] != '0'
			|| szHeader[4] < '0'
			|| szHeader[4] > '9'
			|| szHeader[5] < '0'
			|| szHeader[5] > '9'
			|| (szHeader[5] - '0') + 10 * (szHeader[4] - '0') <= 27)
		{
			ver1 = AcDb::kDHL_1014;
			nRet = eAFile;
		}
		else
		{
			ver1 = AcDb::kDHL_Unknown;
			nRet = eUnknown;
		}
		return nRet;
	}

	ver1 = (AcDb::AcDbDwgVersion)acadid[idx].dwgVersion;
	AC_BYTE byt23 = 0;
	AC_BYTE byt24 = 0;
	if (ver1 >= AcDb::kDHL_1010)
	{
		maintver1 = (AcDb::MaintenanceReleaseVersion)szHeader[11];
		byt23 = szHeader[17];
		byt24 = szHeader[18];
		nCodePage = szHeader[19];
	}

	bExistVer = true;

	Adesk::UInt32 nFormatType = acadid[idx].fileFormatType;
	if (eObsoleteFileFormat == nFormatType || byt12 <= 3 && byt12 != 2)
	{
		switch (ver1)
		{
		case AcDb::kDHL_1013:
			if (maintver1 >= AcDb::kMRelease4)
				nFileFormatType = nFormatType;
			break;
		case AcDb::kDHL_1012:
		case AcDb::kDHL_1014:
			if (byt23 != AcDb::kDHL_1500 || byt24 >= AcDb::kMReleaseFirstValid1500)
				nFileFormatType = nFormatType;
			break;
		case AcDb::kDHL_1500:
			if (maintver1 >= AcDb::kMReleaseFirstValid1500)
				nFileFormatType = nFormatType;
			break;
		default:
			nFileFormatType = nFormatType;
			break;
		}
	}

	ver2 = AcDb::kDHL_Unknown;
	if (byt23)
		ver2 = (AcDb::AcDbDwgVersion)byt23;

	maintver2 = AcDb::kMReleaseUnknown;
	if (byt24)
		maintver2 = (AcDb::MaintenanceReleaseVersion)byt24;

	if (ver1 <= AcDb::kDHL_1800a)
	{
		ver3 = ver2;
		maintver3 = maintver2;
	}
	else if (szHeader[22])
	{
		ver3 = (AcDb::AcDbDwgVersion)szHeader[22];
		maintver3 = (AcDb::MaintenanceReleaseVersion)szHeader[23];
	}
	else
	{
		ver3 = AcDb::kDHL_1800;
		maintver3 = AcDb::kMRelease1;
	}

	if (ver1 < AcDb::kDHL_1010)
		return eAFileR12;

	return ver1 > AcDb::kDHL_1800a ? eAcFs : eAFile;
}

DwgFileIntImp* DwgFileImpBase::newDwgFileIntADP(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

DwgFileIntImp* DwgFileImpBase::newDwgFileIntAcFs(void)
{
	return new DwgFileIntAcFs();
}

DwgFileIntImp* DwgFileImpBase::newDwgFileIntAFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

DwgFileIntImp* DwgFileImpBase::newDwgFileIntAFileR12(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return NULL;
}

void DwgFileImpBase::subCloseFile(void)
{
	if (m_pszFile != NULL)
	{
		DeleteFile(m_pszFile);
		free((void*)m_pszFile);
		m_pszFile = NULL;
	}
}
