#ifndef _DB_HDLTBL_BLK_H_
#define _DB_HDLTBL_BLK_H_

#include "dbhandle.h"
#include "acadstrc.h"

class AcDbDwgFiler;
class HandleOffsetBuffer;

class HandleTableBlock
{
	friend class HandleTableUpdater;

public:
	HandleTableBlock(void);
	HandleTableBlock(const HandleTableBlock&);
	HandleTableBlock(const AcDbHandle&, const AcDbHandle&, const AC_BYTE*, Adesk::UInt32, Adesk::Int64);
	~HandleTableBlock();

	HandleTableBlock& operator= (const HandleTableBlock&);

	void reset(bool);
	AcDbHandle nextHandle(void);
	Acad::ErrorStatus add(const AcDbHandle&, Adesk::Int64);
	Acad::ErrorStatus writeToFiler(AcDbDwgFiler*);
	Acad::ErrorStatus readFromFiler(AcDbDwgFiler*);

public:
	bool getUnk8() const { return m_bUnk8; }

protected:
	Adesk::Int64			m_nUnkOffset;		// 0
	bool					m_bUnk8;			// 8
	HandleOffsetBuffer*		m_pHdlOffsetBuf;	// 16
	HandleTableBlock*		m_pNextBlock;		// 56
};

#endif // _DB_HDLTBL_BLK_H_