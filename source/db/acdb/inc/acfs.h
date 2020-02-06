#ifndef _ACFS_H_
#define _ACFS_H_

#include "acdb.h"
#include "acfsdefs.h"
#include <windows.h>

class AcFsStream;
class AcFsCallBack;
class AcStream;

class AcFs
{
public:
	virtual int CreateFile(const char*, unsigned int, unsigned int, unsigned int, unsigned int) = 0;	// 0//0h
	virtual int CreateFile(const wchar_t*, unsigned int, unsigned int, unsigned int, unsigned int) = 0;	// 8//8h
	virtual int OpenUsing(HANDLE, unsigned int) = 0;		// 16//10h
	virtual Adesk::Boolean isOpen(void) = 0;			// 24//18h
	virtual int SetFileTime(const FILETIME*, const FILETIME*, const FILETIME*) = 0;		// 32//20h
	virtual int GetFileTime(FILETIME*, FILETIME*, FILETIME*) = 0;		// 40//28h
	virtual int GetFileInformationByHandle(BY_HANDLE_FILE_INFORMATION*) = 0;			// 48//30h
	virtual int CloseFile(void) = 0;					// 56//38h
	virtual int Flush(void) = 0;						// 64//40h
	virtual int WriteFile(const void*, Adesk::UInt64) = 0;		// 72//48h
	virtual int ReadFile(void*, Adesk::UInt64*) = 0;			// 80//50h
	virtual int SetFilePointer(Adesk::Int64, unsigned int) = 0;			// 88//58h
	virtual int SetEndOfFile(void) = 0;							// 96//60h
	virtual int ZeroData(Adesk::UInt64, Adesk::UInt64) = 0;		// 104//68h
	virtual int GetFilePointer(Adesk::UInt64*) = 0;				// 112//70h
	virtual int GetFileSize(Adesk::UInt64*) = 0;				// 120//78h
	virtual void SetCompressionType(int) = 0;					// 128//80h
	virtual int GetCompressionType(void) = 0;					// 136//88h
	virtual void SetCompressionLevel(int) = 0;					// 144//90h
	virtual int GetCompressionLevel(void) = 0;					// 152//98h
	virtual void SetBlockSize(Adesk::UInt64) = 0;				// 160//A0h
	virtual void SetMaxCache(int) = 0;							// 168//A8h
	virtual void SetAlignment(int) = 0;							// 176//B0h
	virtual int GetAlignment(void) = 0;							// 184//B8h
	virtual void EnableWriteCache(int) = 0;						// 192//C0h
	virtual Adesk::UInt64 GetBlockSize(void) = 0;				// 200//C8h
	virtual void SetPrivateHeaderSize(int) = 0;					// 208//D0h
	virtual int GetPrivateHeaderSize(void) = 0;					// 216//D8h
	virtual int GetAcFsInfo(AcFs_INFO_t*) = 0;					// 224//E0h
	virtual int GetAcFs6Info(AcFs6_INFO_t*) = 0;				// 232//E8h
	virtual int GetStreamInfo(int, AcFs_STREAMINFO_t*) = 0;		// 240//F0h
	virtual void ResyncSystemFilePointer(void) = 0;				// 248//F8h
	virtual int WritePrivateHeader(const void*, int, int) = 0;	// 256//100h
	virtual int ReadPrivateHeader(void*, int, int) = 0;			// 264//108h
	virtual int OpenStream(const char*, unsigned int, AcFsStream**) = 0;		// 272//110h
	virtual int OpenStream(const wchar_t*, unsigned int, AcFsStream**) = 0;		// 280//118h
	virtual int CreateStream(const char*, unsigned int*, unsigned int, unsigned int, 
							 AcFsStream**, int, Adesk::UInt64, unsigned int) = 0;	// 288//120h
	virtual int CreateStream(const wchar_t*, unsigned int*, unsigned int, unsigned int, 
							 AcFsStream**, int, Adesk::UInt64, unsigned int) = 0;	// 296//128h
	virtual int DeleteStream(const char*, unsigned int) = 0;	// 304//130h
	virtual int DeleteStream(const wchar_t*, unsigned int) = 0;	// 312//138h
	virtual void RegisterCallback(AcFsCallBack*) = 0;			// 320//140h
	virtual void DefineAppFlags(unsigned int, unsigned int) = 0;	// 328//148h
	virtual int GetAppFlags(void) = 0;							// 336//150h
	virtual void SetKey(const unsigned int*) = 0;				// 344//158h
	virtual Adesk::Boolean IsRealAcFsFile(void) = 0;			// 352//160h
	virtual void SetV6StreamCallbackFix(bool) = 0;				// 360//168h
	virtual int GetLastWriteError(void) = 0;					// 368//170h
	virtual int GetNumberOfWriteErrors(void) = 0;				// 376//178h
	virtual int OpenUsing(AcStream*, unsigned int) = 0;			// 384//180h
	virtual void SetThreading(int) = 0;							// 392//188h
	virtual ~AcFs() = 0 {}										// 408//198h
};

#endif // _ACFS_H_