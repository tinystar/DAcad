#ifndef _DWG_FILE_INT_ACFS_H_
#define _DWG_FILE_INT_ACFS_H_

#include "dwgFileInti.h"
#include "dbfastfiler.h"

class AcFs;
class AcFsStream;

struct StreamDescriptor
{
	const ACHAR*	pStreamName;
	Adesk::ULongPtr	uStreamOffset;
	Adesk::UInt32	uUnknown16;
	Adesk::UInt32	uUnknown20;
	Adesk::UInt64	uUnknown24;
};


class DwgFileIntAcFs : public DwgFileIntImp, public AcDbFastDwgFiler
{
	friend class DwgFileImpBase;

protected:
	DwgFileIntAcFs();
	virtual ~DwgFileIntAcFs();

protected:
	virtual bool isWriteable(void);						// 0//0h
	virtual const ACHAR* getFileName(void);				// 32//20h
	virtual const ACHAR* getAcDwgx(void);				// 48//30h
	virtual Adesk::UInt64 getFileSize(void);			// 56//38h
	virtual Acad::ErrorStatus getError(void);			// 120//78h
	virtual Acad::ErrorStatus getFileTime(FILETIME*, FILETIME*, FILETIME*);		// 128//80h
	virtual bool hasPassword(void);						// 152//98h
	virtual bool hasSignature(void);					// 160//A0h
	virtual bool needsRecovery(void);					// 168//A8h
	virtual Acad::ErrorStatus testPassword(const SecurityParams&);		// 176//B0h
	virtual Acad::ErrorStatus getSignature(const SignatureInfo*&);		// 184//B8h
	virtual int getType(void);							// 208//D0h
	virtual void* getAFilePtr(void);					// 216//D8h
	virtual AcFs* getAcFsPtr(void);						// 224//E0h
	virtual void reopenToDenyWrites(Acad::ErrorStatus&);	// 232//E8h
	virtual unsigned int getDwgShareMode(void);				// 256//100h

	virtual Acad::ErrorStatus freeData(HANDLE*);			// 264//108h
	virtual Acad::ErrorStatus initForReading(const ACHAR*,
											 HANDLE,
											 unsigned int,
											 unsigned int,
											 const AC_BYTE*,
											 unsigned int,
											 int(*)(int, int, int, int),
											 bool&);

	virtual Acad::ErrorStatus downgradeAfterSave(void);		// 296//128h
	virtual Acad::ErrorStatus setCrashSave(void);			// 304//130h
	virtual Acad::ErrorStatus openForWrite(const ACHAR*);	// 312//138h
	virtual Adesk::Int64 getObjectSectionSize(void);		// 320//140h
	virtual AcDbFastDwgFiler* fastDwgFiler(void);			// 328//148h
	virtual Acad::ErrorStatus setFileTime(const FILETIME*, const FILETIME*, const FILETIME*);	// 336//150h
	virtual void setSignatureInfo(Adesk::Boolean);			// 344//158h
	virtual void setPasswordInfo(Adesk::Boolean);			// 352//160h
	virtual Acad::ErrorStatus seekFile(Adesk::Int64, int);	// 360//168h
	virtual Adesk::Int64 getFilePointer(void);				// 368//170h
	virtual Adesk::UInt64 readBinaryBytes(void*, Adesk::UInt64);	// 376//178h
	virtual Adesk::UInt64 writeBinaryBytes(void const*, Adesk::UInt64);	// 384//180h
	virtual Acad::ErrorStatus flushAndCheckForWriteErrors(void);		// 392//188h
	virtual Acad::ErrorStatus startSectionRead(int);		// 400//190h
	virtual Acad::ErrorStatus endSectionRead(void);			// 408//198h
	virtual Acad::ErrorStatus startSectionSave(int);		// 416//1A0h
	virtual Acad::ErrorStatus endSectionSave(void);			// 424//1A8h
	virtual Acad::ErrorStatus startContiguousSectionSave(int, int);		// 432//1B0h
	virtual Acad::ErrorStatus endContiguousSectionSave(int, const unsigned int*);	// 440//1B8h
	virtual void* getCurrentAcFsStream(void);				// 448//1C0h
	virtual Acad::ErrorStatus pushObjectSectionRead(void);	// 456//1C8h
	virtual Acad::ErrorStatus popObjectSectionRead(void);	// 464//1D0h
	virtual Acad::ErrorStatus readVBASection(void*, unsigned int, unsigned int&, unsigned int&);		// 472//1D8h
	virtual Acad::ErrorStatus writeVBASection(const void*, unsigned int, unsigned int, unsigned int);	// 480//1E0h
	virtual Acad::ErrorStatus switchFromHandleToObjectSection(void);	// 488//1E8h
	virtual Acad::ErrorStatus returnFromObjectToHandleSection(void);	// 496//1F0h
	virtual Acad::ErrorStatus switchFromHandleToFreeSpaceSection(void);	// 504//1F8h
	virtual Acad::ErrorStatus returnFromFreeSpaceToHandleSection(void);	// 512//200h
	virtual Acad::ErrorStatus startAcDsSectionRead(SaveObjectReadState*&);		// 520//208h
	virtual Acad::ErrorStatus endAcDsSectionRead(const SaveObjectReadState*);	// 528//210h
	virtual Acad::ErrorStatus prepareToWriteSections(void);				// 536//218h
	virtual Acad::ErrorStatus omitSection(int);				// 544//220h
	virtual bool sectionExists(int);						// 552//228h
	virtual Acad::ErrorStatus getHdrHdr(void);				// 560//230h
	virtual Acad::ErrorStatus saveHdrHdr(void);				// 568//238h
	virtual Acad::ErrorStatus saveUnknownSections(AcDbImpDatabase*);	// 576//240h
	virtual Acad::ErrorStatus readUnknownSections(AcDbImpDatabase*);	// 584//248h
	virtual Acad::ErrorStatus tryPassword(const wchar_t*, CIPHER*);		// 592//250h
	virtual Acad::ErrorStatus flushBuffers(void);			// 600//258h
	virtual Acad::ErrorStatus getAcADPPackage(void);		// 608//260h
	virtual int getCurrentSection(void);					// 616//268h

protected:
	void setAcFsVer(void);
	void setupAcFs(AcFs*&);
	void freeDeadAcFs(void);
	Acad::ErrorStatus verifyDwgShareMode(unsigned int, unsigned int);
	void closeAndDeleteTempRecoveryFile(void);

	Acad::ErrorStatus seekToSection(int, int);

	int createStream(const StreamDescriptor*, int);

protected:
	AcFs*			m_pAcFs;			// 160
	HANDLE			m_hFile;			// 168
	HANDLE			m_hTmpFile;			// 176
	ACHAR*			m_pFileName;		// 184
	ACHAR*			m_pTmpFile;			// 192
	int				m_nCurSection;		// 200
	int				m_nObjSecDepth;		// 204
	int				m_nSecReadState;	// 208
	Adesk::UInt32	m_uAcFsVer;			// 212
	Adesk::Boolean	m_bPassword;		// 232
	Adesk::Boolean	m_bSignature;		// 236
	bool			m_bNeedsRecovery;	// 256
	bool			m_bCrashSave;		// 257
	bool			m_bWriteable;		// 258
	AcFsStream*		m_pCurFsStream;		// 264

	AcFsStream*		m_pHdrStream;		// 272
	AcFsStream*		m_pAuxHdrStream;	// 280
	AcFsStream*		m_pClsStream;		// 288
	AcFsStream*		m_pPreviewStream;	// 296
	AcFsStream*		m_pHdlStream;		// 304
	AcFsStream*		m_pObjFreeStream;	// 312
	AcFsStream*		m_pTmplStream;		// 320
	AcFsStream*		m_pObjsStream;		// 328
	AcFsStream*		m_pShInfoStream;	// 336
	AcFsStream*		m_pFDepLstStream;	// 344
	AcFsStream*		m_pSmyInfoStream;	// 352
	AcFsStream*		m_pVBAPrjStream;	// 360
	AcFsStream*		m_pRevHstStream;	// 368
	AcFsStream*		m_pApInfoStream;	// 376
	AcFsStream*		m_pApInfHstStream;	// 384
	AcFsStream*		m_pAcDsStream;		// 392

protected:

	static StreamDescriptor smDefaultStreams[];
};

#endif // _DWG_FILE_INT_ACFS_H_