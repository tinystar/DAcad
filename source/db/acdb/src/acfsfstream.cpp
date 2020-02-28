#include "acfsfstream.h"
#include <windows.h>


AcFs_fstream::AcFs_fstream()
{
	Reset();
}

void AcFs_fstream::Reset(void)
{
	m_uFileSize = 0;
	m_nNodeCount = 0;
	m_uBlockSize = 29696;
	m_nComprLevel = 1;
	m_nComprType = 2;
	m_nStreamId = 0;
	m_uAppFlags = 0;
	m_szName[0] = 0;
}

void AcFs_fstream::SetName(const ACHAR* pName)
{
	WideCharToMultiByte(CP_ACP, 0, pName, -1, m_szName, MAX_STREAM_NAME_LEN, NULL, NULL);
}

void AcFs_fstream::GetName(ACHAR* pName)
{
	MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, m_szName, -1, pName, MAX_STREAM_NAME_LEN);
}
