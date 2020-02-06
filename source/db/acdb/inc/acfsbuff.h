#ifndef _ACFS_BUFF_H_
#define _ACFS_BUFF_H_

#include "adesk.h"
#include <malloc.h>

class AcFsHeap;

class GuardBuff
{
public:
	GuardBuff()
		: m_uSize(0)
		, m_pBuffer(NULL)
		, m_pFsHeap(NULL)
	{
	}

	~GuardBuff()
	{
		Reset();
	}

	void Reset()
	{
		if (m_pBuffer)
			free(m_pBuffer);
		m_pBuffer = NULL;
		m_uSize = 0;
	}

	Adesk::UInt32	m_uSize;		// 0
	AC_BYTE*		m_pBuffer;		// 8
	AcFsHeap*		m_pFsHeap;		// 16
};

class AcFsTempBuff : public GuardBuff
{
public:
	void SetSize(int);
};

#endif // _ACFS_BUFF_H_