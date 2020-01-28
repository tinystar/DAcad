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
	Adesk::Int32	m_nUnk16;			// 16
	Adesk::Int32	m_nUnk20;			// 20
	Adesk::Int32	m_nUnk24;			// 24
	Adesk::Int32	m_nUnk28;			// 28
	Adesk::Int32	m_nUnk32;			// 32
	Adesk::Int32	m_nUnk36;			// 36
	Adesk::Int32	m_nUnk40;			// 40
	Adesk::Int64	m_nUnk44;			// 44
	Adesk::Int64	m_nUnk52;			// 52
	Adesk::Int32	m_nUnk60;			// 60
	Adesk::Int32	m_nUnk64;			// 64
	Adesk::Int32	m_nUnk68;			// 68
	Adesk::Int32	m_nUnk72;			// 72
	Adesk::Int32	m_nUnk76;			// 76
	Adesk::Int32	m_nUnk80;			// 80
	Adesk::Int64	m_nUnk84;			// 84
	Adesk::Int32	m_nUnk92;			// 92
	Adesk::Int32	m_nUnk96;			// 96
	Adesk::Int32	m_nUnk100;			// 100
	Adesk::Int32	m_nUnk104;			// 104
};

#pragma pack (pop)

#endif // _ACFS_FHEADER_H_