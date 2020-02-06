#ifndef _DWG_FILE_INT_H_
#define _DWG_FILE_INT_H_

#include "dwgFileHandle.h"

class AcFs;

class ADESK_NO_VTABLE DwgFileInt : public AcDwgFileHandle
{
public:
	virtual ~DwgFileInt() {}								// 200//C8h
	
	virtual int getType(void) = 0;			// 返回值类型不确定	// 208//D0h
	virtual void* getAFilePtr(void) = 0;	// 返回值类型不确定	// 216//D8h
	virtual AcFs* getAcFsPtr(void) = 0;							// 224//E0h
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