#include "dbHdlTblBlk.h"
#include "dbHdlTblUpdater.h"
#include "dbHandleData.h"
#include "dbfiler.h"


HandleTableBlock::HandleTableBlock(void)
{

}

HandleTableBlock::HandleTableBlock(const HandleTableBlock&)
{

}

HandleTableBlock::HandleTableBlock(const AcDbHandle&, const AcDbHandle&, const AC_BYTE*, Adesk::UInt32, Adesk::Int64)
{

}

HandleTableBlock::~HandleTableBlock()
{

}

HandleTableBlock& HandleTableBlock::operator=(const HandleTableBlock&)
{

}

void HandleTableBlock::reset(bool)
{

}

AcDbHandle HandleTableBlock::nextHandle(void)
{

}

Acad::ErrorStatus HandleTableBlock::add(const AcDbHandle&, Adesk::Int64)
{

}

Acad::ErrorStatus HandleTableBlock::writeToFiler(AcDbDwgFiler*)
{

}

Acad::ErrorStatus HandleTableBlock::readFromFiler(AcDbDwgFiler* pDwgFiler)
{
	if (NULL == m_pHdlOffsetBuf)
		m_pHdlOffsetBuf = HandleTableUpdater::getBuffer();

	m_pHdlOffsetBuf->resetBuffer();
	Acad::ErrorStatus es = pDwgFiler->seek(m_nUnkOffset, AcDb::kSeekFromStart);
	if (Acad::eOk == es)
	{
		es = m_pHdlOffsetBuf->dwgIn(pDwgFiler);
		if (Acad::eOk == es)
			m_bUnk8 = false;
	}
	return es;
}
