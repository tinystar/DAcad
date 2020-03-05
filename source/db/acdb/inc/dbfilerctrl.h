#ifndef _DB_FILER_CTRL_H_
#define _DB_FILER_CTRL_H_

#include "AcHeapOpers.h"
#include "codepgid.h"

class AcDbFilerController : public AcHeapOperators
{
public:
	AcDbFilerController();

	void doCRCAccumulation(const void*, Adesk::UInt64);
	void setCodePageId(code_page_id);
	void startCollectingLocaleInfo(void);
	void stopCollectingLocaleInfo(void);
	bool gotLocaleInfo(void);
	Adesk::UInt8 getMIFCodePageIndex(void);
	void setMIFCodePageIndex(Adesk::UInt8);
	bool collectingLocaleInfo(void);
	bool applyingLocaleInfo(void);
	void startApplyingLocaleInfo(Adesk::UInt8);
	void stopApplyingLocaleInfo(void);

public:
	bool doCRCCheck() const { return m_bDoCRCCheck; }

protected:
	bool				m_bDoCRCCheck;		// 4		// ???
	Adesk::Int8			m_nLInfoState;		// 14
	Adesk::UInt8		m_uMIFCPIdx;		// 15
	code_page_id		m_CodePageId;		// 32
};

#endif // _DB_FILER_CTRL_H_