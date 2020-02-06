#ifndef _ACFS_FSTREAM_H_
#define _ACFS_FSTREAM_H_

#include "adesk.h"
#include "acfsdefs.h"

#pragma pack (push, 1)

class AcFs_fstream
{
public:
	AcFs_fstream();
	void Reset(void);
	void SetName(const ACHAR*);
	void GetName(ACHAR*);

public:
	Adesk::Int64	m_nUnk0;						// 0//0h
	Adesk::Int32	m_nUnk8;						// 8//8h
	Adesk::Int32	m_nUnk12;						// 12//Ch
	Adesk::Int32	m_nUnk16;						// 16//10h
	Adesk::Int32	m_nUnk20;						// 20//14h
	Adesk::Int32	m_nUnk24;						// 24//18h
	Adesk::Int32	m_nUnk28;						// 28//1Ch
	char			m_szName[MAX_STREAM_NAME_LEN];	// 32//20h
};

#pragma pack (pop)

#endif // _ACFS_FSTREAM_H_