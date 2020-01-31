#ifndef _ACFS_NODE_H_
#define _ACFS_NODE_H_

#include "adesk.h"

class AcFs_mbheader;

class AcFs_fnode
{
public:
	Adesk::Int32		m_blkId;	// 0
	Adesk::Int32		m_nUnk4;	// 4
};

// sizeof = 56
class AcFs_mnode
{
public:
	AcFs_mbheader*		m_pMbHeader;	// 0
	Adesk::Int32		m_nUnk8;		// 8
	Adesk::Int64		m_nStartPos;	// 48
};

#endif // _ACFS_NODE_H_