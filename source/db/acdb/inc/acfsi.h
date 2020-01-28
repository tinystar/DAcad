#ifndef _ACFSI_H_
#define _ACFSI_H_

#include "acfs.h"
#include "acfsistream.h"

class AcFs_mheader;
class AcFsHeap;

class AcFsI : public AcFs
{
	friend AcFs* AcFsNewClass();

protected:
	AcFsI();

public:
	virtual int CreateFile(const char*, unsigned int, unsigned int, unsigned int, unsigned int);	// 0//0h
	virtual int CreateFile(const wchar_t*, unsigned int, unsigned int, unsigned int, unsigned int);	// 8//8h
	virtual int OpenUsing(HANDLE, unsigned int);		// 16//10h
	virtual Adesk::Boolean isOpen(void);			// 24//18h
	virtual int SetFileTime(const FILETIME*, const FILETIME*, const FILETIME*);		// 32//20h
	virtual int GetFileTime(FILETIME*, FILETIME*, FILETIME*);		// 40//28h
	virtual int GetFileInformationByHandle(BY_HANDLE_FILE_INFORMATION*);			// 48//30h
	virtual int CloseFile(void);					// 56//38h
	virtual int Flush(void);						// 64//40h
	virtual int WriteFile(const void*, Adesk::UInt64);		// 72//48h
	virtual int ReadFile(void*, Adesk::UInt64*);			// 80//50h
	virtual int SetFilePointer(Adesk::Int64, unsigned int);			// 88//58h
	virtual int SetEndOfFile(void);							// 96//60h
	virtual int ZeroData(Adesk::UInt64, Adesk::UInt64);		// 104//68h
	virtual int GetFilePointer(Adesk::UInt64*);				// 112//70h
	virtual int GetFileSize(Adesk::UInt64*);				// 120//78h
	virtual void SetCompressionType(int);					// 128//80h
	virtual int GetCompressionType(void);					// 136//88h
	virtual void SetCompressionLevel(int);					// 144//90h
	virtual int GetCompressionLevel(void);					// 152//98h
	virtual void SetBlockSize(Adesk::UInt64);				// 160//A0h
	virtual void SetMaxCache(int);							// 168//A8h
	virtual void SetAlignment(int);							// 176//B0h
	virtual int GetAlignment(void);							// 184//B8h
	virtual void EnableWriteCache(int);						// 192//C0h
	virtual Adesk::UInt64 GetBlockSize(void);				// 200//C8h
	virtual void SetPrivateHeaderSize(int);					// 208//D0h
	virtual int GetPrivateHeaderSize(void);					// 216//D8h
	virtual int GetAcFsInfo(AcFs_INFO_t*);					// 224//E0h
	virtual int GetAcFs6Info(AcFs6_INFO_t*);				// 232//E8h
	virtual int GetStreamInfo(int, AcFs_STREAMINFO_t*);		// 240//F0h
	virtual void ResyncSystemFilePointer(void);				// 248//F8h
	virtual int WritePrivateHeader(const void*, int, int);	// 256//100h
	virtual int ReadPrivateHeader(void*, int, int);			// 264//108h
	virtual int OpenStream(const char*, unsigned int, AcFsStream**);		// 272//110h
	virtual int OpenStream(const wchar_t*, unsigned int, AcFsStream**);		// 280//118h
	virtual int CreateStream(const char*, unsigned int*, unsigned int, unsigned int,
							 AcFsStream**, int, Adesk::UInt64, unsigned int);	// 288//120h
	virtual int CreateStream(const wchar_t*, unsigned int*, unsigned int, unsigned int,
							 AcFsStream**, int, Adesk::UInt64, unsigned int);	// 296//128h
	virtual int DeleteStream(const char*, unsigned int);	// 304//130h
	virtual int DeleteStream(const wchar_t*, unsigned int);	// 312//138h
	virtual void RegisterCallback(AcFsCallBack*);			// 320//140h
	virtual void DefineAppFlags(unsigned int, unsigned int);	// 328//148h
	virtual int GetAppFlags(void);							// 336//150h
	virtual void SetKey(const unsigned int*);				// 344//158h
	virtual Adesk::Boolean IsRealAcFsFile(void);			// 352//160h
	virtual void SetV6StreamCallbackFix(bool);				// 360//168h
	virtual int GetLastWriteError(void);					// 368//170h
	virtual int GetNumberOfWriteErrors(void);				// 376//178h
	virtual int OpenUsing(AcStream*, unsigned int);			// 384//180h
	virtual void SetThreading(int);							// 392//188h
	virtual ~AcFsI();										// 408//198h

public:
	Adesk::UInt32 getAccessMode() const { return m_uAccessMode; }

protected:
	int iCreateFile(const void*, unsigned int, unsigned int, unsigned int, 
					unsigned int, bool, HANDLE, Adesk::Int64);
	int iCreateStream(const wchar_t*, unsigned int*, AcFsIStream**, int, int, unsigned int);

	void lock() { EnterCriticalSection(&m_cs); }
	void unlock() { LeaveCriticalSection(&m_cs); }

	int processLogicalHeaders(void);
	int ReadHeader(void);

protected:
	Adesk::Int32		m_nUnk8;			// 8//8h
	AcFsIStream			m_stream1;			// 16//10h
	int					m_nUnk88;			// 88//58h
	Adesk::UInt32		m_uUnk352;			// 352//160h
	int					m_nUnk404;			// 404//194h
	AcFs_mheader*		m_pMHeader;			// 448//1C0h
	int					m_nUnk456;			// 456//1C8h
	Adesk::UInt32		m_uUnk560;			// 560//230h
	int					m_nComprType;		// 564//234h
	Adesk::Int32		m_nUnk568;			// 568//238h
	Adesk::Int32		m_nUnk572;			// 572//23Ch
	Adesk::UInt32		m_uAccessMode;		// 576//240h
	Adesk::UInt32		m_uPrivtHdrSize;	// 580//244h
	CRITICAL_SECTION	m_cs;				// 592//250h
	Adesk::Int32		m_nUnk664;			// 664//298h
	Adesk::Int32		m_nUnk668;			// 668//29Ch
	AcFsHeap*			m_pFsHeap;			// 672//2A0h
};

#endif // _ACFSI_H_