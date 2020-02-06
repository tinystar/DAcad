#ifndef _DWG_SHARE_INFO_H_
#define _DWG_SHARE_INFO_H_

#include "acadstrc.h"

class AcDwgFileHandle;

class DwgShareInfo
{
public:
	DwgShareInfo(void);
	~DwgShareInfo();

	Acad::ErrorStatus read(const AcDwgFileHandle*);
	Acad::ErrorStatus write(const AcDwgFileHandle*);
	unsigned int getDwgShareMode(void);

private:
	unsigned int m_uUnk0;		// 0
	unsigned int m_uUnk4;		// 4
	unsigned int m_uUnk8;		// 8
	void*		 m_pUnk16;		// 16
};

#endif // _DWG_SHARE_INFO_H_