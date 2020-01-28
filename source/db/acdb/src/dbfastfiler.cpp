#include "dbfastfiler.h"
#include "dwgFileInti.h"
#include "acutmacro.h"


AcDbFastDwgFiler::AcDbFastDwgFiler()
	: m_pDwgFileInt(NULL)
	, m_filerStatus(Acad::eOk)
	, m_pAuditInfo(NULL)
{

}

AcDbFastDwgFiler::~AcDbFastDwgFiler()
{

}

Acad::ErrorStatus AcDbFastDwgFiler::filerStatus() const
{
	return m_filerStatus;
}

AcDb::FilerType AcDbFastDwgFiler::filerType() const
{
	return AcDb::kFileFiler;
}

void AcDbFastDwgFiler::setFilerStatus(Acad::ErrorStatus es)
{
	m_filerStatus = es;
}

void AcDbFastDwgFiler::resetFilerStatus()
{
	setFilerStatus(Acad::eOk);	// 80
}

Acad::ErrorStatus AcDbFastDwgFiler::dwgVersion(AcDb::AcDbDwgVersion& ver, AcDb::MaintenanceReleaseVersion& maintVer) const
{
	return m_pDwgFileInt->getDwgVersion(ver, maintVer);	// 80
}

Acad::ErrorStatus AcDbFastDwgFiler::readHardOwnershipId(AcDbHardOwnershipId*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeHardOwnershipId(const AcDbHardOwnershipId&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readSoftOwnershipId(AcDbSoftOwnershipId*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeSoftOwnershipId(const AcDbSoftOwnershipId&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readHardPointerId(AcDbHardPointerId*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeHardPointerId(const AcDbHardPointerId&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readSoftPointerId(AcDbSoftPointerId*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeSoftPointerId(const AcDbSoftPointerId&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readInt8(Adesk::Int8 *)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeInt8(Adesk::Int8)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readString(ACHAR **)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readString(AcString &)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeString(const ACHAR *)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeString(const AcString &)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readBChunk(ads_binary *)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeBChunk(const ads_binary&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readAcDbHandle(AcDbHandle*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeAcDbHandle(const AcDbHandle&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readInt64(Adesk::Int64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeInt64(Adesk::Int64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readInt32(Adesk::Int32*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeInt32(Adesk::Int32)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readInt16(Adesk::Int16*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeInt16(Adesk::Int16)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readUInt64(Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeUInt64(Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readUInt32(Adesk::UInt32*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeUInt32(Adesk::UInt32)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readUInt16(Adesk::UInt16*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeUInt16(Adesk::UInt16)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readUInt8(Adesk::UInt8*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeUInt8(Adesk::UInt8)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readBoolean(Adesk::Boolean*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeBoolean(Adesk::Boolean)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readBool(bool*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeBool(bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readDouble(double*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeDouble(double)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readPoint2d(AcGePoint2d*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writePoint2d(const AcGePoint2d&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readPoint3d(AcGePoint3d*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writePoint3d(const AcGePoint3d&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readVector2d(AcGeVector2d*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeVector2d(const AcGeVector2d&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readVector3d(AcGeVector3d*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeVector3d(const AcGeVector3d&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readScale3d(AcGeScale3d*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeScale3d(const AcGeScale3d&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::readBytes(void *, Adesk::UIntPtr)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::writeBytes(const void *, Adesk::UIntPtr)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}

Acad::ErrorStatus AcDbFastDwgFiler::seek(Adesk::Int64 nOffset, int nMethod)
{
	if(m_pDwgFileInt->seekFile(nOffset, nMethod) != Acad::eOk)	// 360
		return Acad::eFilerError;

	return Acad::eOk;
}

Adesk::Int64 AcDbFastDwgFiler::tell() const
{
	return m_pDwgFileInt->getFilePointer();		// 368
}

AcDbAuditInfo * AcDbFastDwgFiler::getAuditInfo() const
{
	return m_pAuditInfo;
}
