#ifndef _AC_RX_TEXT_STR_H_
#define _AC_RX_TEXT_STR_H_

#include "rxobject.h"
#include <malloc.h>


class AcRxTextString : public AcRxObject
{
public:
	ACRX_DECLARE_MEMBERS_EXPIMP(AcRxTextString, ACBASE_PORT);

	explicit AcRxTextString(bool bIgnoreCase);
	AcRxTextString(const ACHAR* pszStr, bool bIgnoreCase);
	AcRxTextString(const AcRxTextString& other, bool bIgnoreCase);
	virtual ~AcRxTextString();

public:

	virtual AcRxObject* clone(void);							// 24//18h

	virtual Adesk::Boolean isEqualTo(const AcRxObject*);		// 40//28h
	virtual AcRx::Ordering comparedTo(const AcRxObject*);		// 48//30h

	virtual const ACHAR* getAsciiPtr(void) const;				// 64//40h
	virtual operator const ACHAR* (void) const;					// 72//48h

	virtual void cpyFromAscii(const ACHAR*);					// 80//50h
	virtual AcRxTextString& operator= (const ACHAR*);			// 88//58h
	// Caller is responsible for free the return string.
	virtual ACHAR* cpyToAscii(void);							// 96//60h

	virtual ACHAR getChar(unsigned int);						// 104//68h
	virtual ACHAR operator[] (unsigned int);					// 112//70h

	virtual void copy(const AcRxTextString&);					// 120//78h
	virtual void cat(const AcRxTextString&);					// 128//80h
	virtual unsigned int len(void) const;						// 136//88h
	virtual int cmp(const AcRxTextString&) const;				// 144//90h

protected:
	void initStrData(void);
	void killStrData(void);

protected:
	unsigned int	m_uStrLen;			// 8
	unsigned int	m_uCapacity;		// 12
	ACHAR*			m_pBuffer;			// 16
	bool			m_bIgnoreCase;		// 24
};

inline void AcRxTextString::initStrData(void)
{
	m_uStrLen = 0;
	m_uCapacity = 0;
	m_pBuffer = NULL;
}

inline void AcRxTextString::killStrData(void)
{
	if (m_pBuffer != NULL)
		free(m_pBuffer);
	initStrData();
}

#endif // _AC_RX_TEXT_STR_H_