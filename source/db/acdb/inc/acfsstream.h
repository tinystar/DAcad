#ifndef _ACFS_STREAM_H_
#define _ACFS_STREAM_H_

#include "adesk.h"

class AcFs_STREAMINFO_t;

typedef void (*AcFsStreamCallBack)(void);

class AcFsStream
{
public:
	virtual int WriteFile(const void*, Adesk::UInt64) = 0;		// 0//0h
	virtual int ReadFile(void*, Adesk::UInt64*) = 0;			// 8//8h
	virtual int SetFilePointer(Adesk::Int64, Adesk::UInt32) = 0;	// 16//10h
	virtual int SetEndOfFile(void) = 0;							// 24//18h
	virtual int ZeroData(Adesk::UInt64, Adesk::UInt64) = 0;		// 32//20h
	virtual int GetFilePointer(Adesk::UInt64*) = 0;				// 40//28h
	virtual int GetFileSize(Adesk::UInt64*) = 0;				// 48//30h
	virtual Adesk::UInt32 GetBlockSize(void) = 0;				// 56//38h
	virtual void SetCompressionLevel(int) = 0;					// 64//40h
	virtual int GetCompressionLevel(void) = 0;					// 72//48h
	virtual int GetStreamInfo(AcFs_STREAMINFO_t*) = 0;			// 80//50h
	virtual void EnableWriteCache(int) = 0;						// 88//58h
	virtual void SetMaxCache(int) = 0;							// 96//60h
	virtual int Flush(bool) = 0;								// 104//68h
	virtual const ACHAR* GetStreamName(void) = 0;				// 112//70h
	virtual int GetStreamID(void) = 0;							// 120//78h
	virtual int ScanPhysical(const void**, Adesk::UInt64*, Adesk::UInt64*) = 0;		// 128//80h
	virtual int GetPhysicalAddr(Adesk::UInt64*) = 0;			// 136//88h
	virtual void RegisterCallback(AcFsStreamCallBack*) = 0;		// 144//90h
	virtual void DefineAppFlags(Adesk::UInt32, Adesk::UInt32) = 0;	// 152//98h
	virtual Adesk::UInt32 GetAppFlags(void) = 0;				// 160//A0h
	virtual void SetAccessType(Adesk::UInt32) = 0;				// 168//A8h
	virtual ~AcFsStream() = 0 {}								// 184//B8h
};

#endif // _ACFS_STREAM_H_