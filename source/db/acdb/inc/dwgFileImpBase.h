#ifndef _DWG_FILE_IMP_BASE_H_
#define _DWG_FILE_IMP_BASE_H_

#include "dwgFileInt.h"

class AcDbImpDatabase;

class DwgFileImpBase : public DwgFileInt
{
protected:
	DwgFileImpBase();

public:
	virtual bool isXRef(void);							// 8//8h
	virtual bool isXRefTemp(void);						// 16//10h
	virtual const ACHAR* getOriginalXRefName(void);		// 24//18h
	virtual AcDbDatabase* getDatabase(void);			// 40//28h
	virtual code_page_id getCodePage(void);				// 64//40h
	virtual AcDb::AcDbDwgVersion getDwgVersion(void);	// 72//48h
	virtual Acad::ErrorStatus getDwgVersion(AcDb::AcDbDwgVersion&, AcDb::MaintenanceReleaseVersion&);	// 80//50h
	virtual bool isValid(void);							// 112//70h
	virtual unsigned int getAccessMode(void);			// 136//88h
	virtual unsigned int getShareMode(void);			// 144//90h
	virtual bool hasPassword(void);						// 152//98h
	virtual bool hasSignature(void);					// 160//A0h
	virtual Acad::ErrorStatus testPassword(const SecurityParams&);		// 176//B0h
	virtual Acad::ErrorStatus getSignature(const SignatureInfo*&);		// 184//B8h
	virtual ~DwgFileImpBase();							// 200//C8h

	virtual void setIsXRef(void);							// 240//F0h
	virtual void setIsXRefTemp(const ACHAR*);				// 248//F8h
	virtual int getDwgShareMode(void);						// 256//100h

	virtual Acad::ErrorStatus freeData(void**) = 0;			// 264//108h
	virtual Acad::ErrorStatus initForReading(const ACHAR*,
											 HANDLE,
											 unsigned int, 
											 unsigned int, 
											 const AC_BYTE*, 
											 unsigned int, 
											 int(*)(int, int, int, int), 
											 bool&) = 0;	// 272//110h

	virtual Acad::ErrorStatus attachDb(AcDbImpDatabase*, bool);			// 280//118h
	virtual Acad::ErrorStatus detachDb(AcDbImpDatabase*, bool, bool);	// 288//120h

public:
	static Adesk::UInt32 numberOfOpenDwgFiles(void) { return smNumberOfOpenDwgFiles; }

	static DwgFileImpBase* openWithModes(const ACHAR*,
										unsigned int,
										unsigned int, 
										bool, 
										bool, 
										Acad::ErrorStatus&, 
										int(*)(int, int, int, int));

	static int readversion(const AC_BYTE*,
						   unsigned int, 
						   bool&, 
						   int&, 
						   int&, 
						   AcDb::AcDbDwgVersion&, 
						   AcDb::MaintenanceReleaseVersion&, 
						   AcDb::AcDbDwgVersion&, 
						   AcDb::MaintenanceReleaseVersion&, 
						   AcDb::AcDbDwgVersion&, 
						   AcDb::MaintenanceReleaseVersion&);

	static DwgFileImpBase* newDwgFileIntADP(void);
	static DwgFileImpBase* newDwgFileIntAcFs(void);
	static DwgFileImpBase* newDwgFileIntAFile(void);
	static DwgFileImpBase* newDwgFileIntAFileR12(void);

protected:
	static Adesk::UInt32 smNumberOfOpenDwgFiles;

protected:
	AcDbImpDatabase*				m_pImpDb;		// 8
	code_page_id					m_codePage;		// 16
	AcDb::AcDbDwgVersion			m_dwgVer1;		// 20
	AcDb::MaintenanceReleaseVersion	m_maintVer1;	// 24
	AcDb::AcDbDwgVersion			m_dwgVer2;		// 28
	AcDb::MaintenanceReleaseVersion	m_maintVer2;	// 32
	AcDb::AcDbDwgVersion			m_dwgVer3;		// 36
	AcDb::MaintenanceReleaseVersion	m_maintVer3;	// 40
	unsigned int					m_AccessMode;	// 44
	unsigned int					m_ShareMode;	// 48
	bool							m_bValid;		// 52
	bool							m_bUnknown54;	// 54
};

#endif // _DWG_FILE_IMP_BASE_H_