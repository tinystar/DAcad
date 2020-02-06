#ifndef _ACFS_ISTREAM_H_
#define _ACFS_ISTREAM_H_

#include "acfsstream.h"
#include "acfsfstream.h"

class AcFs_mheader;
class AcFs_fnode;
class AcFs_mnode;
class AcFsHeap;

class AcFsIStream : public AcFsStream
{
	friend class AcFsI;

public:
	virtual int WriteFile(const void*, Adesk::UInt64);		// 0//0h
	virtual int ReadFile(void*, Adesk::UInt64*);			// 8//8h
	virtual int SetFilePointer(Adesk::Int64, Adesk::UInt32);	// 16//10h
	virtual int SetEndOfFile(void);							// 24//18h
	virtual int ZeroData(Adesk::UInt64, Adesk::UInt64);		// 32//20h
	virtual int GetFilePointer(Adesk::UInt64*);				// 40//28h
	virtual int GetFileSize(Adesk::UInt64*);				// 48//30h
	virtual Adesk::UInt32 GetBlockSize(void);				// 56//38h
	virtual void SetCompressionLevel(int);					// 64//40h
	virtual int GetCompressionLevel(void);					// 72//48h
	virtual int GetStreamInfo(AcFs_STREAMINFO_t*);			// 80//50h
	virtual void EnableWriteCache(int);						// 88//58h
	virtual void SetMaxCache(int);							// 96//60h
	virtual int Flush(bool);								// 104//68h
	virtual const ACHAR* GetStreamName(void);				// 112//70h
	virtual int GetStreamID(void);							// 120//78h
	virtual int ScanPhysical(const void**, Adesk::UInt64*, Adesk::UInt64*);		// 128//80h
	virtual int GetPhysicalAddr(Adesk::UInt64*);			// 136//88h
	virtual void RegisterCallback(AcFsStreamCallBack);		// 144//90h
	virtual void DefineAppFlags(Adesk::UInt32, Adesk::UInt32);	// 152//98h
	virtual Adesk::UInt32 GetAppFlags(void);				// 160//A0h
	virtual void SetAccessType(Adesk::UInt32);				// 168//A8h
	virtual ~AcFsIStream();									// 184//B8h

protected:
	AcFsIStream(void);
	void Reset(void);
	void CopyFtoM(AcFsI*, AcFs_mheader*, AcFs_fstream*);
	void CopyFNtoMN(AcFs_fnode*, AcFs_mnode*);
	AcFs_mnode* MapLinear(Adesk::Int64, int*);
	int ReadNodes(char**, int);

	int GetReadCache(void);
	int ReadBlock(AcFs_mnode*);
	int WriteBlock(AcFs_mnode*);

	int LRUAddToFront(AcFs_mnode*);
	void LRURemove(AcFs_mnode*);
	void LRUMoveToFront(AcFs_mnode*);

	void DeleteMemory(void);

protected:
	AcFsIStream*		m_pNextStream;		// 8//8h
	AcFsIStream*		m_pPrevStream;		// 16//10h
	AcFsI*				m_pAcFsI;			// 24//18h
	AcFs_mheader*		m_pMHeader;			// 32//20h
	Adesk::UInt64		m_uFilePtr;			// 40//28h
	Adesk::UInt64		m_uFileSize;		// 48//30h
	AcFs_mnode**		m_pmnodeTable;		// 56//38h
	Adesk::Int32		m_mnodeTblSize;		// 64//40h
	int					m_nComprLevel;		// 68//44h
	int					m_nComprType;		// 72//48h
	int					m_nStreamId;		// 76//4Ch
	ACHAR				m_szStreamName[MAX_STREAM_NAME_LEN];	// 80//50h --> 336
	Adesk::UInt32		m_uBlockSize;		// 336//150h
	AC_BYTE*			m_pBufferCur;		// 344//158h
	AC_BYTE*			m_pBufferEnd;		// 352//160h
	AcFs_mnode*			m_pMNodeUnk360;		// 360//168h
	AcFs_mnode*			m_pMNodeUnk368;		// 368//170h
	Adesk::Boolean		m_bEnableWrCache;	// 376//178h
	Adesk::Boolean		m_bUnk380;			// 380//17Ch
	Adesk::UInt32		m_uAcessType;		// 384//180h
	Adesk::UInt32		m_uMaxCache;		// 388//184h
	Adesk::UInt32		m_uUnk392;			// 392//188h
	AcFsStreamCallBack	m_pCallback;		// 400//190h
	Adesk::UInt32		m_uAppFlags;		// 408//198h
	AcFsHeap*			m_pFsHeap;			// 416//1A0h
	bool				m_bUnk424;			// 424//1A8h
};

#endif // _ACFS_ISTREAM_H_