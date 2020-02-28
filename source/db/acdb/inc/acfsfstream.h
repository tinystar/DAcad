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
	Adesk::UInt64	m_uFileSize;					// 0//0h
	Adesk::Int32	m_nNodeCount;					// 8//8h
	Adesk::UInt32	m_uBlockSize;					// 12//Ch
	Adesk::Int32	m_nComprLevel;					// 16//10h
	Adesk::Int32	m_nComprType;					// 20//14h
	Adesk::Int32	m_nStreamId;					// 24//18h
	Adesk::UInt32	m_uAppFlags;					// 28//1Ch
	char			m_szName[MAX_STREAM_NAME_LEN];	// 32//20h
};

#pragma pack (pop)

#endif // _ACFS_FSTREAM_H_