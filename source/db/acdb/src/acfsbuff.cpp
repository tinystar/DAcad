#include "acfsbuff.h"
#include "acfsheap.h"


void AcFsTempBuff::SetSize(int nSize)
{
	if (m_uSize < nSize)
	{
		if (m_pBuffer)
			free(m_pBuffer);

		m_uSize = (nSize + 15) & 0xFFFFFFF0;
		m_pBuffer = (AC_BYTE*)m_pFsHeap->zalloc(m_uSize);
	}
}
