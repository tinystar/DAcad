#ifndef _DB_HDLTBL_UPDATER_H_
#define _DB_HDLTBL_UPDATER_H_

#include "dbhandle.h"
#include "acadstrc.h"
#include "dbid.h"

class AcDbDwgFiler;
class FreeSpaceAllocator;
class HandleTableBlock;
class HandleOffsetBuffer;
class AcDbQueue;

class HandleTableUpdater
{
public:
	HandleTableUpdater(void);
	~HandleTableUpdater();

	void clear(void);
	void freeBufferAvail(void);
	Acad::ErrorStatus addBlock(const AcDbHandle&, const AcDbHandle&, const AC_BYTE*, Adesk::UInt32, Adesk::Int64);
	Acad::ErrorStatus setFrom(AcDbDwgFiler*, bool);
	void removeEntry(AcDbObjectId);
	void updateEntry(AcDbObjectId, Adesk::Int64);
	Acad::ErrorStatus greatestHandle(void);
	void updateToFiler(FreeSpaceAllocator*, long long &, long long &);
	Acad::ErrorStatus addBlockAtCur(const HandleTableBlock&);
	Acad::ErrorStatus deleteCurBlock(void);
	Adesk::UInt32 numBlocks(void);

	static HandleOffsetBuffer* getBuffer(void);
	static void addBuffer(HandleOffsetBuffer*);

protected:
	AcDbDwgFiler*		m_pDwgFiler;		// 0
	HandleTableBlock*	m_pHdlTblBlk;		// 8
	HandleTableBlock*	m_pUnkBlock16;		// 16
	HandleTableBlock*	m_pUnkBlock24;		// 24
	HandleTableBlock*	m_pUnkBlock32;		// 32
	AcDbQueue*			m_pQueue;			// 40
};

#endif // _DB_HDLTBL_UPDATER_H_