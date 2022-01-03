#include "dbHandleData.h"
#include "acutmacro.h"
#include "dbfiler.h"


HandleDataDef::HandleDataDef(AC_BYTE* pBytes, Adesk::Int32 unk)
	: m_nUnk0(2)
	, m_nUnk4(unk)
	, m_nUnk8(-1)
	, m_uUnk16(0)
	, m_pUnk24(NULL)
	, m_pUnk32(pBytes)
{
}

void HandleDataDef::resetBuffer(void)
{
	m_nUnk0 = 2;
	m_nUnk8 = -1;
	m_uUnk16 = 0;
	m_pUnk24 = NULL;
}

void HandleDataDef::setBufferBytes(const AC_BYTE*, Adesk::Int32)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void HandleDataDef::getBufferBytes(AC_BYTE*&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void HandleDataDef::add(const Adesk::UInt64&, Adesk::Int64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void HandleDataDef::toBytes(int, const Adesk::UInt64&, AC_BYTE*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void HandleDataDef::start(void)
{
	m_nUnk8 = 2;
	m_pUnk24 = NULL;
	m_uUnk16 = 0;
}

bool HandleDataDef::next(Adesk::UInt64& handle, Adesk::Int64& unk)
{
	if (m_nUnk8 < m_nUnk0)
	{
		Adesk::UInt32* pHdlData = (Adesk::UInt32*)&handle;
		char* pCurData = (char*)(m_pUnk32 + m_nUnk8);
		*(pHdlData + 1) = 0;
		Adesk::UInt32 val = *pCurData & 0x7F;
		*pHdlData = val;

		Adesk::UInt32 idx = 1;
		if (*pCurData < 0)
		{
			Adesk::UInt32 shift = 7;
			char byte = 0;
			do 
			{
				byte = *(pCurData + idx);
				val |= (byte & 0x7F) << shift;
				*pHdlData = val;
				if (shift == 28)
				{
					val = (byte >> 4) & 7;
					*(pHdlData + 1) = val;
					pHdlData = pHdlData + 1;
					shift = -4;
				}
				shift += 7;
				idx++;
			} while (byte < 0);
			pCurData += idx;
		}
		else
		{
			pCurData++;
		}

		handle += m_uUnk16;
		m_uUnk16 += idx;
		unk = 0;
		v14 = *v8;
		v15 = v14 & ((((*v8 & 0x80u) >> 1) ^ 0x40) + 127);
		v16 = (*v8 & 0x80) == 0;
		*a3 = v15;
	}
}

void HandleDataDef::read(AcHsBufferReader&)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void HandleDataDef::copyToOldType(const Adesk::UInt64&, AC_BYTE*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void HandleDataDef::compactify(int, int, const AC_BYTE*, AC_BYTE*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void HandleDataDef::field7(int, const AC_BYTE*, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}


HandleDataBase::HandleDataBase(AC_BYTE* pBytes, Adesk::Int32 unk)
	: HandleDataDef(pBytes, unk)
{

}

Acad::ErrorStatus HandleDataBase::dwgIn(AcDbDwgFiler* pDwgFiler)
{
	Acad::ErrorStatus es = pDwgFiler->readBytes(m_pUnk32, 2);	// 488
	if (es != Acad::eOk)
		return es;

	m_nUnk0 = *(m_pUnk32 + 1) | (*m_pUnk32 << 8);
	if (m_nUnk0 < 2)
		return Acad::eDwgObjectImproperlyRead;

	if (m_nUnk0 > m_nUnk4)
		return Acad::eDwgObjectImproperlyRead;

	es = pDwgFiler->readBytes(m_pUnk32 + 2, m_nUnk0);			// 488
	if (Acad::eOk == es)
	{
		m_pUnk24 = NULL;
		m_uUnk16 = 0;
		m_nUnk8 = -1;
	}

	return es;
}

Acad::ErrorStatus HandleDataBase::dwgOut(AcDbDwgFiler*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Acad::eNotImplementedYet;
}
