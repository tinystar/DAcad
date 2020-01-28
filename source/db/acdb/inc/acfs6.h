#ifndef _ACFS6_H_
#define _ACFS6_H_

#include "acfs.h"

class AcFs6 : public AcFs
{
public:
	AcFs6();

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
	virtual ~AcFs6();										// 408//198h
};

#endif // _ACFS6_H_