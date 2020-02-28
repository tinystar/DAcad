#include "acfsfheader.h"
#include <string.h>
#include "acutmacro.h"
#include "acfsdefs.h"
#include "lzo/lzoconf.h"

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
	strcpy_s(m_szSign, signiture);
	m_nHeaderSize = ACFS_HEADER_SIZE;
	m_nAcFsVer = 4;
	m_nUnk24 = 0;
	m_nUnk28 = 0;
	m_nUnk32 = 0;
	m_nUnk36 = 0;
	m_nUnk40 = 0;
	m_nUnk44 = 0;
	m_nMBEndAddr = 0;
	m_nNegBlks = 0;
	m_nPosBlks = 0;
	m_nAlignment = 32;
	m_nAlignFHdrSize = 128;
	m_nDoubleAlign = 64;
	m_nBlksInfoBlkId = 0;
	m_nBlksHdrOffset = 0;
	m_nStreamsBlkId = 0;
	m_nPosBlksMaxId = 0;
	m_nNegBlksMaxId = 0;
	m_uCrc32 = 0;
}

Adesk::Boolean AcFs_fheader::VerifyHeader(void)
{
	if (strcmp(m_szSign, signiture) != 0 ||
		m_nHeaderSize != ACFS_HEADER_SIZE ||
		m_nAcFsVer != 4)
		return Adesk::kFalse;

	Adesk::UInt32 crc32 = m_uCrc32;
	m_uCrc32 = 0;
	unsigned int uCalCrc = lzo_crc32(0, (const lzo_bytep)this, ACFS_HEADER_SIZE);
	m_uCrc32 = crc32;
	if (crc32 != uCalCrc)
		return Adesk::kFalse;

	return Adesk::kTrue;
}

Adesk::Boolean AcFs_fheader::recoverVerifyHeader(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kTrue;
}

void AcFs_fheader::CopyMB(AcFs_mheader*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
