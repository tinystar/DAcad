#ifndef _ACFS_FHEADER_H_
#define _ACFS_FHEADER_H_

#include "adesk.h"

class AcFs_mheader;

#pragma pack (push, 1)

class AcFs_fheader
{
public:
	AcFs_fheader();
	~AcFs_fheader();

	void Reset(void);
	Adesk::Boolean VerifyHeader(void);
	Adesk::Boolean recoverVerifyHeader(void);
	void CopyMB(AcFs_mheader*);

public:
	char			m_szSign[16];		// 0
	Adesk::Int32	m_nHeaderSize;		// 16
	Adesk::Int32	m_nAcFsVer;			// 20		// ???
	Adesk::Int32	m_nUnk24;			// 24
	Adesk::Int32	m_nUnk28;			// 28
	Adesk::Int32	m_nUnk32;			// 32
	Adesk::Int32	m_nUnk36;			// 36
	Adesk::Int32	m_nUnk40;			// 40
	Adesk::Int64	m_nUnk44;			// 44
	Adesk::Int64	m_nMBEndAddr;		// 52
	Adesk::Int32	m_nNegBlks;			// 60
	Adesk::Int32	m_nPosBlks;			// 64
	Adesk::Int32	m_nAlignment;		// 68
	Adesk::Int32	m_nAlignFHdrSize;	// 72		// ???
	Adesk::Int32	m_nDoubleAlign;		// 76
	Adesk::Int32	m_nBlksInfoBlkId;	// 80		// ???
	Adesk::Int64	m_nBlksHdrOffset;	// 84
	Adesk::Int32	m_nStreamsBlkId;	// 92
	Adesk::Int32	m_nPosBlksMaxId;	// 96
	Adesk::Int32	m_nNegBlksMaxId;	// 100
	Adesk::UInt32	m_uCrc32;			// 104
};

#pragma pack (pop)

#endif // _ACFS_FHEADER_H_