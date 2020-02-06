#ifndef _DB_FAST_FILER_H_
#define _DB_FAST_FILER_H_

#include "dbfiler.h"

class DwgFileIntImp;

class AcDbFastDwgFiler : public AcDbDwgFiler
{
protected:
	AcDbFastDwgFiler();
	virtual ~AcDbFastDwgFiler();

public:
	virtual  Acad::ErrorStatus filerStatus() const;
	virtual  AcDb::FilerType   filerType() const;
	virtual void               setFilerStatus(Acad::ErrorStatus);
	virtual void               resetFilerStatus();

	virtual  Acad::ErrorStatus  dwgVersion(AcDb::AcDbDwgVersion &,
										   AcDb::MaintenanceReleaseVersion &) const;

	virtual Acad::ErrorStatus   readHardOwnershipId(AcDbHardOwnershipId*);
	virtual Acad::ErrorStatus   writeHardOwnershipId(const AcDbHardOwnershipId&);

	virtual Acad::ErrorStatus   readSoftOwnershipId(AcDbSoftOwnershipId*);
	virtual Acad::ErrorStatus   writeSoftOwnershipId(const AcDbSoftOwnershipId&);

	virtual Acad::ErrorStatus   readHardPointerId(AcDbHardPointerId*);
	virtual Acad::ErrorStatus   writeHardPointerId(const AcDbHardPointerId&);

	virtual Acad::ErrorStatus   readSoftPointerId(AcDbSoftPointerId*);
	virtual Acad::ErrorStatus   writeSoftPointerId(const AcDbSoftPointerId&);

	virtual Acad::ErrorStatus   readInt8(Adesk::Int8 *);
	virtual Acad::ErrorStatus   writeInt8(Adesk::Int8);

	virtual Acad::ErrorStatus   readString(ACHAR **);
	virtual Acad::ErrorStatus   writeString(const ACHAR *);

	virtual Acad::ErrorStatus   readString(AcString &);
	virtual Acad::ErrorStatus   writeString(const AcString &);

	virtual Acad::ErrorStatus   readBChunk(ads_binary *);
	virtual Acad::ErrorStatus   writeBChunk(const ads_binary&);

	virtual Acad::ErrorStatus   readAcDbHandle(AcDbHandle*);
	virtual Acad::ErrorStatus   writeAcDbHandle(const AcDbHandle&);

	virtual Acad::ErrorStatus   readInt64(Adesk::Int64*);
	virtual Acad::ErrorStatus   writeInt64(Adesk::Int64);

	virtual Acad::ErrorStatus   readInt32(Adesk::Int32*);
	virtual Acad::ErrorStatus   writeInt32(Adesk::Int32);

	virtual Acad::ErrorStatus   readInt16(Adesk::Int16*);
	virtual Acad::ErrorStatus   writeInt16(Adesk::Int16);

	virtual Acad::ErrorStatus   readUInt64(Adesk::UInt64*);
	virtual Acad::ErrorStatus   writeUInt64(Adesk::UInt64);

	virtual Acad::ErrorStatus   readUInt32(Adesk::UInt32*);
	virtual Acad::ErrorStatus   writeUInt32(Adesk::UInt32);

	virtual Acad::ErrorStatus   readUInt16(Adesk::UInt16*);
	virtual Acad::ErrorStatus   writeUInt16(Adesk::UInt16);

	virtual Acad::ErrorStatus   readUInt8(Adesk::UInt8*);
	virtual Acad::ErrorStatus   writeUInt8(Adesk::UInt8);

	virtual Acad::ErrorStatus   readBoolean(Adesk::Boolean*);
	virtual Acad::ErrorStatus   writeBoolean(Adesk::Boolean);

	virtual Acad::ErrorStatus   readBool(bool*);
	virtual Acad::ErrorStatus   writeBool(bool);

	virtual Acad::ErrorStatus   readDouble(double*);
	virtual Acad::ErrorStatus   writeDouble(double);

	virtual Acad::ErrorStatus   readPoint2d(AcGePoint2d*);
	virtual Acad::ErrorStatus   writePoint2d(const AcGePoint2d&);

	virtual Acad::ErrorStatus   readPoint3d(AcGePoint3d*);
	virtual Acad::ErrorStatus   writePoint3d(const AcGePoint3d&);

	virtual Acad::ErrorStatus   readVector2d(AcGeVector2d*);
	virtual Acad::ErrorStatus   writeVector2d(const AcGeVector2d&);

	virtual Acad::ErrorStatus   readVector3d(AcGeVector3d*);
	virtual Acad::ErrorStatus   writeVector3d(const AcGeVector3d&);

	virtual Acad::ErrorStatus   readScale3d(AcGeScale3d*);
	virtual Acad::ErrorStatus   writeScale3d(const AcGeScale3d&);

	virtual Acad::ErrorStatus   readBytes(void *, Adesk::UIntPtr);
	virtual Acad::ErrorStatus   writeBytes(const void *, Adesk::UIntPtr);

	virtual Acad::ErrorStatus   seek(Adesk::Int64 nOffset, int nMethod);
	virtual Adesk::Int64        tell() const;

	virtual AcDbAuditInfo *     getAuditInfo() const;

public:
	DwgFileIntImp* getDwgFileInt() const { return m_pDwgFileInt; }

protected:
	Acad::ErrorStatus readwithCRC(void* pBytes, Adesk::UInt64 uBytes);

protected:
	DwgFileIntImp*			m_pDwgFileInt;		// 16
	Acad::ErrorStatus		m_filerStatus;		// 24
	AcDbAuditInfo*			m_pAuditInfo;		// 32
};

#endif // _DB_FAST_FILER_H_