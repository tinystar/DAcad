#ifndef _DWG_FILE_INT_H_
#define _DWG_FILE_INT_H_

#include "acdb.h"
#include "codepgid.h"
#include <windows.h>
#include "dbsecurity.h"

class AcDwgFileHandle;

class DwgFileInt
{
public:
	virtual bool isWriteable(void) = 0;						// 0//0h
	virtual bool isXRef(void) = 0;							// 8//8h
	virtual bool isXRefTemp(void) = 0;						// 16//10h
	virtual const ACHAR* getOriginalXRefName(void) = 0;		// 24//18h
	virtual const ACHAR* getFileName(void) = 0;				// 32//20h
	virtual AcDbDatabase* getDatabase(void) = 0;			// 40//28h
	virtual const ACHAR* getAcDwgx(void) = 0;				// 48//30h
	virtual Adesk::UInt64 getFileSize(void) = 0;			// 56//38h
	virtual code_page_id getCodePage(void) = 0;				// 64//40h
	virtual AcDb::AcDbDwgVersion getDwgVersion(void) = 0;	// 72//48h
	virtual Acad::ErrorStatus getDwgVersion(AcDb::AcDbDwgVersion&, AcDb::MaintenanceReleaseVersion&) = 0;	// 80//50h
	virtual const ACHAR* getVersionGuid(void) = 0;			// 88//58h
	virtual const ACHAR* getFingerPrintGuid(void) = 0;		// 96//60h
	virtual const ACHAR* getComment(void) = 0;				// 104//68h
	virtual bool isValid(void) = 0;							// 112//70h
	virtual Acad::ErrorStatus getError(void) = 0;			// 120//78h
	virtual Acad::ErrorStatus getFileTime(FILETIME*, FILETIME*, FILETIME*) = 0;		// 128//80h
	virtual unsigned int getAccessMode(void) = 0;			// 136//88h
	virtual unsigned int getShareMode(void) = 0;			// 144//90h
	virtual bool hasPassword(void) = 0;						// 152//98h
	virtual bool hasSignature(void) = 0;					// 160//A0h
	virtual bool needsRecovery(void) = 0;					// 168//A8h
	virtual Acad::ErrorStatus testPassword(const SecurityParams&) = 0;		// 176//B0h
	virtual Acad::ErrorStatus getSignature(const SignatureInfo*&) = 0;		// 184//B8h
	virtual ~DwgFileInt() {}								// 200//C8h
	
	virtual int getType(void) = 0;			// 返回值类型不确定	// 208//D0h
	virtual void* getAFilePtr(void) = 0;	// 返回值类型不确定	// 216//D8h
	virtual void* getAcFsPtr(void) = 0;		// 返回值类型不确定	// 224//E0h
	virtual void reopenToDenyWrites(Acad::ErrorStatus&) = 0;	// 232//E8h
	virtual void setIsXRef(void) = 0;							// 240//F0h
	virtual void setIsXRefTemp(const ACHAR*) = 0;				// 248//F8h
	virtual unsigned int getDwgShareMode(void) = 0;				// 256//100h

public:
	static Adesk::UInt32 numberOfOpenDwgFiles(void);
	static Adesk::Boolean openForWrite(const ACHAR*, AcDb::AcDbDwgVersion, AcDbDatabase*, Acad::ErrorStatus&);
	static Adesk::Boolean open(const ACHAR*, bool, Acad::ErrorStatus&);
	static Adesk::Boolean openDemandLoadXref(const ACHAR*, Acad::ErrorStatus&);
	static Acad::ErrorStatus copyCloseAndDelete(AcDwgFileHandle*&, const ACHAR*);
};

#endif // _DWG_FILE_INT_H_