#ifndef _ACFS_MBHEADER_H_
#define _ACFS_MBHEADER_H_

#include "adesk.h"

class AcFs_mbheader
{
public:
	AcFs_mbheader*		pPrev;		// 0
	AcFs_mbheader*		pNext;		// 8
	Adesk::Int64		nOffset;	// 16
	Adesk::Int32		nBlkSize;	// 24
	Adesk::Int32		nBlkId;		// 28
	AcFs_mbheader*		pUnk32;		// 32
	AcFs_mbheader*		pUnk40;		// 40
	AcFs_mbheader*		pUnk48;		// 48
	AcFs_mbheader*		pUnk56;		// 56
};

#endif // _ACFS_MBHEADER_H_