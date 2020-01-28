#include "acfsfheader.h"
#include <string.h>
#include "acutmacro.h"

static const char* signiture = "AcFssFcAJMB";


AcFs_fheader::AcFs_fheader()
{
	Reset();
}

AcFs_fheader::~AcFs_fheader()
{

}

void AcFs_fheader::Reset(void)
{
	strcpy_s(m_szSign, "AcFssFcAJMB");
	m_nUnk16 = 108;
	m_nUnk20 = 4;
	m_nUnk24 = 0;
	m_nUnk28 = 0;
	m_nUnk32 = 0;
	m_nUnk36 = 0;
	m_nUnk40 = 0;
	m_nUnk44 = 0;
	m_nUnk52 = 0;
	m_nUnk60 = 0;
	m_nUnk64 = 0;
	m_nUnk68 = 32;
	m_nUnk72 = 128;
	m_nUnk76 = 64;
	m_nUnk80 = 0;
	m_nUnk84 = 0;
	m_nUnk92 = 0;
	m_nUnk96 = 0;
	m_nUnk100 = 0;
	m_nUnk104 = 0;
}

Adesk::Boolean AcFs_fheader::VerifyHeader(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kTrue;
}

Adesk::Boolean AcFs_fheader::recoverVerifyHeader(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kTrue;
}

void AcFs_fheader::CopyMB(AcFs_mheader*)
{

}
