#include "rxtextstr.h"
#include "acutmacro.h"
#include "rxcommon.h"
#include "cstrop.h"


ACRX_NO_CONS_DEFINE_MEMBERS(AcRxTextString, AcRxObject)

AcRxTextString::AcRxTextString(const ACHAR* pszStr, bool bIgnoreCase)
	: m_pBuffer(NULL)
	, m_uStrLen(0)
	, m_uCapacity(0)
	, m_bIgnoreCase(bIgnoreCase)
{
	cpyFromAscii(pszStr);
}

AcRxTextString::AcRxTextString(const AcRxTextString& other, bool bIgnoreCase)
	: m_pBuffer(NULL)
	, m_uStrLen(0)
	, m_uCapacity(0)
	, m_bIgnoreCase(bIgnoreCase)
{
	copy(other);
}

AcRxTextString::AcRxTextString(bool bIgnoreCase)
	: m_pBuffer(NULL)
	, m_uStrLen(0)
	, m_uCapacity(0)
	, m_bIgnoreCase(bIgnoreCase)
{

}

AcRxTextString::~AcRxTextString()
{
	killStrData();
}

AcRxObject* AcRxTextString::clone(void)
{
	AcRxTextString* pClone = new AcRxTextString(*this, false);
	return pClone;
}

Adesk::Boolean AcRxTextString::isEqualTo(const AcRxObject* pOther)
{
	Adesk::Boolean bRet = Adesk::kFalse;
	AcRxTextString* pOtherStr = AcRxTextString::cast(pOther);
	if (pOtherStr != NULL)
	{
		bRet = ((m_uStrLen == pOtherStr->m_uStrLen) == 0);
		if (m_uStrLen > 0 && pOtherStr->m_uStrLen > 0)
			bRet = (0 == acStricmp_local(m_pBuffer, pOtherStr->m_pBuffer));
	}

	return bRet;
}

AcRx::Ordering AcRxTextString::comparedTo(const AcRxObject* pOther)
{
	AcRxTextString* pOtherStr = AcRxTextString::cast(pOther);
	if (NULL == pOtherStr)
		return AcRx::kNotOrderable;

	return (AcRx::Ordering)cmp(*pOtherStr);
}

const ACHAR* AcRxTextString::getAsciiPtr(void) const
{
	return m_pBuffer;
}

AcRxTextString::operator const ACHAR*(void) const
{
	return m_pBuffer;
}

void AcRxTextString::cpyFromAscii(const ACHAR* pszStr)
{
	m_uStrLen = 0;
	if (pszStr != NULL)
	{
		m_uStrLen = (unsigned int)ac_strlen(pszStr);
		if (m_uStrLen > 0)
		{
			if (m_uStrLen > m_uCapacity)
			{
				AC_SAFE_FREE(m_pBuffer);

				m_pBuffer = (ACHAR*)malloc(sizeof(ACHAR) * (m_uStrLen + 1));
				m_uCapacity = m_uStrLen;
			}
			ac_strcpy(m_pBuffer, pszStr);
		}
	}
}

AcRxTextString& AcRxTextString::operator=(const ACHAR* pszStr)
{
	cpyFromAscii(pszStr);			// 80
	return *this;
}

ACHAR* AcRxTextString::cpyToAscii(void)
{
	ACHAR* pBuf = NULL;

	unsigned int uLen = len();		// 136
	if (uLen > 0)
	{
		pBuf = (ACHAR*)malloc(sizeof(ACHAR) * (uLen + 1));
		ac_strcpy(pBuf, m_pBuffer);
	}

	return pBuf;
}

ACHAR AcRxTextString::getChar(unsigned int idx)
{
	ACHAR ch = 0;
	if (idx < len())				// 136
		ch = m_pBuffer[idx];

	return ch;
}

ACHAR AcRxTextString::operator[](unsigned int idx)
{
	return getChar(idx);			// 104
}

void AcRxTextString::copy(const AcRxTextString& other)
{
	if (len() > 0)				// 136
	{
		m_uStrLen = 0;			// 8
		*m_pBuffer = 0;			// 16
	}
	return cat(other);			// 128
}

void AcRxTextString::cat(const AcRxTextString& other)
{
	const ACHAR* pOtherStr = other.getAsciiPtr();			// 64
	if (pOtherStr != NULL)
	{
		unsigned int uTotalLen = len();						// 136
		uTotalLen += other.len();							// 136
		if (uTotalLen > m_uCapacity || m_pBuffer == NULL)
		{
			ACHAR* pNewBuf = (ACHAR*)malloc(sizeof(ACHAR) * (uTotalLen + 1));
			if (len() > 0)									// 136
				ac_strcpy(pNewBuf, m_pBuffer);
			else
				*pNewBuf = 0;

			if (m_pBuffer != NULL)
				free(m_pBuffer);
			m_pBuffer = pNewBuf;
			m_uCapacity = uTotalLen;
		}

		ac_strcat(m_pBuffer, pOtherStr);
		m_uStrLen = (unsigned int)ac_strlen(m_pBuffer);
	}
}

unsigned int AcRxTextString::len(void) const
{
	return m_uStrLen;
}

int AcRxTextString::cmp(const AcRxTextString& other) const
{
	int nRet = 0;
	const ACHAR* pOtherStr = other.getAsciiPtr();		// 64
	if (pOtherStr != NULL)
	{
		nRet = -1;
		if (m_uStrLen > 0)
		{
			nRet = acStricmp_local(m_pBuffer, pOtherStr);
		}
	}
	else
	{
		nRet = m_uStrLen != 0;
	}

	return nRet;
}
