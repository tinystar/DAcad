#ifndef _ACFS_NODE_H_
#define _ACFS_NODE_H_

#include "adesk.h"

class AcFs_mbheader;

#pragma pack (push, 1)

class AcFs_fnode
{
public:
	Adesk::Int32		m_blkId;		// 0
	Adesk::UInt32		m_uComprSize;	// 4
};

#pragma pack (pop)

// sizeof = 56
class AcFs_mnode
{
public:
	AcFs_mbheader*		m_pMbHeader;	// 0
	Adesk::UInt32		m_uComprSize;	// 8
	AC_BYTE*			m_pBuffer;		// 16
	AcFs_mnode*			m_pPrevNode;	// 24
	AcFs_mnode*			m_pNextNode;	// 32
	Adesk::Int32		m_nUnk40;		// 40
	Adesk::Int64		m_nStartPos;	// 48
};

#endif // _ACFS_NODE_H_