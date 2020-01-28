#ifndef _DWG_FILE_INTI_H_
#define _DWG_FILE_INTI_H_

#include "dwgFileImpBase.h"

class AcDbDwgFiler;
class CIPHER;

class DwgFileIntImp : public DwgFileImpBase
{
public:
	enum SaveObjectReadState
	{

	};

	DwgFileIntImp(void);
	virtual ~DwgFileIntImp();

	virtual const ACHAR* getVersionGuid(void);					// 88//58h
	virtual const ACHAR* getFingerPrintGuid(void);				// 96//60h
	virtual const ACHAR* getComment(void);						// 104//68h
	
	virtual Acad::ErrorStatus downgradeAfterSave(void) = 0;		// 296//128h
	virtual Acad::ErrorStatus setCrashSave(void) = 0;			// 304//130h
	virtual Acad::ErrorStatus openForWrite(const ACHAR*) = 0;	// 312//138h
	virtual Adesk::Int64 getObjectSectionSize(void) = 0;		// 320//140h
	virtual AcDbDwgFiler* fastDwgFiler(void) = 0;				// 328//148h
	virtual Acad::ErrorStatus setFileTime(const FILETIME*, const FILETIME*, const FILETIME*) = 0;	// 336//150h
	virtual void setSignatureInfo(unsigned int) = 0;			// 344//158h
	virtual void setPasswordInfo(unsigned int) = 0;				// 352//160h
	virtual Acad::ErrorStatus seekFile(Adesk::Int64, int) = 0;	// 360//168h
	virtual Adesk::Int64 getFilePointer(void) = 0;				// 368//170h
	virtual Acad::ErrorStatus readBinaryBytes(void*, Adesk::UInt64) = 0;	// 376//178h
	virtual Adesk::UInt64 writeBinaryBytes(void const*, Adesk::UInt64) = 0;	// 384//180h
	virtual Acad::ErrorStatus flushAndCheckForWriteErrors(void) = 0;		// 392//188h
	virtual Acad::ErrorStatus startSectionRead(int) = 0;		// 400//190h
	virtual Acad::ErrorStatus endSectionRead(void) = 0;			// 408//198h
	virtual Acad::ErrorStatus startSectionSave(int) = 0;		// 416//1A0h
	virtual Acad::ErrorStatus endSectionSave(void) = 0;			// 424//1A8h
	virtual Acad::ErrorStatus startContiguousSectionSave(int, int) = 0;		// 432//1B0h
	virtual Acad::ErrorStatus endContiguousSectionSave(int, const unsigned int*) = 0;	// 440//1B8h
	virtual void* getCurrentAcFsStream(void) = 0;				// 448//1C0h
	virtual Acad::ErrorStatus pushObjectSectionRead(void) = 0;	// 456//1C8h
	virtual Acad::ErrorStatus popObjectSectionRead(void) = 0;	// 464//1D0h
	virtual Acad::ErrorStatus readVBASection(void*, unsigned int, unsigned int&, unsigned int&) = 0;		// 472//1D8h
	virtual Acad::ErrorStatus writeVBASection(const void*, unsigned int, unsigned int, unsigned int) = 0;	// 480//1E0h
	virtual Acad::ErrorStatus switchFromHandleToObjectSection(void) = 0;	// 488//1E8h
	virtual Acad::ErrorStatus returnFromObjectToHandleSection(void) = 0;	// 496//1F0h
	virtual Acad::ErrorStatus switchFromHandleToFreeSpaceSection(void) = 0;	// 504//1F8h
	virtual Acad::ErrorStatus returnFromFreeSpaceToHandleSection(void) = 0;	// 512//200h
	virtual Acad::ErrorStatus startAcDsSectionRead(SaveObjectReadState*&) = 0;		// 520//208h
	virtual Acad::ErrorStatus endAcDsSectionRead(const SaveObjectReadState*) = 0;	// 528//210h
	virtual Acad::ErrorStatus prepareToWriteSections(void) = 0;				// 536//218h
	virtual Acad::ErrorStatus omitSection(int) = 0;				// 544//220h
	virtual bool sectionExists(int) = 0;						// 552//228h
	virtual Acad::ErrorStatus getHdrHdr(void) = 0;				// 560//230h
	virtual Acad::ErrorStatus saveHdrHdr(void) = 0;				// 568//238h
	virtual Acad::ErrorStatus saveUnknownSections(AcDbImpDatabase*);	// 576//240h
	virtual Acad::ErrorStatus readUnknownSections(AcDbImpDatabase*);	// 584//248h
	virtual Acad::ErrorStatus tryPassword(const wchar_t*, CIPHER*);		// 592//250h
	virtual Acad::ErrorStatus flushBuffers(void) = 0;			// 600//258h
	virtual Acad::ErrorStatus getAcADPPackage(void) = 0;		// 608//260h
};

#endif // _DWG_FILE_INTI_H_