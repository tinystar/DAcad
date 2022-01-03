#include "dbHdlTblUpdater.h"
#include "dbHdlTblBlk.h"
#include "dbQueue.h"


HandleTableUpdater::HandleTableUpdater(void)
{

}

HandleTableUpdater::~HandleTableUpdater()
{

}

void HandleTableUpdater::clear(void)
{
	m_pUnkBlock24 = m_pHdlTblBlk;
	if (m_pHdlTblBlk != NULL)
	{
		HandleTableBlock* pBlock = m_pHdlTblBlk;
		do 
		{
			m_pUnkBlock32 = pBlock;
			HandleTableBlock* pNextBlock = pBlock->m_pNextBlock;
			m_pUnkBlock24 = pNextBlock;
			if (pBlock)
			{
				HandleOffsetBuffer* pOffsetBuffer = pBlock->m_pHdlOffsetBuf;
				if (pOffsetBuffer)
				{
					HandleTableUpdater::addBuffer(pOffsetBuffer);
					pBlock->m_pHdlOffsetBuf = NULL;
				}

				delete pBlock;
			}
			
			pBlock = pNextBlock;
		} while (pBlock);
	}

	m_pHdlTblBlk->m_pHdlOffsetBuf = NULL;
	m_pHdlTblBlk->m_bUnk8 = false;
	m_pHdlTblBlk->m_nUnkOffset = 0;
	m_pUnkBlock32 = NULL;
	m_pQueue->clear();
	m_pDwgFiler = NULL;
}

void HandleTableUpdater::freeBufferAvail(void)
{

}

Acad::ErrorStatus HandleTableUpdater::addBlock(const AcDbHandle& h1, const AcDbHandle& h2, const AC_BYTE* pUnk, Adesk::UInt32 uUnk, Adesk::Int64 nUnk)
{
	HandleTableBlock* pBlock = new HandleTableBlock(h1, h2, pUnk, uUnk, nUnk);
	if (NULL == pBlock)
		return Acad::eOutOfMemory;

	pBlock->m_pNextBlock = NULL;
	if (m_pUnkBlock16 != NULL)
		m_pUnkBlock16->m_pNextBlock = pBlock;
	else
		m_pHdlTblBlk = pBlock;
	m_pUnkBlock16 = pBlock;

	return Acad::eOk;
}

Acad::ErrorStatus HandleTableUpdater::setFrom(AcDbDwgFiler* pDwgFiler, bool bUnk)
{
	m_pDwgFiler = pDwgFiler;
	Acad::ErrorStatus es = Acad::eOk;
	if (bUnk)
	{
		if (m_pHdlTblBlk != NULL)
		{
			HandleTableBlock* pBlock = m_pHdlTblBlk;
			do 
			{
				if (pBlock->getUnk8())
					es = pBlock->readFromFiler(m_pDwgFiler);
				pBlock = pBlock->m_pNextBlock;
			} while (pBlock && Acad::eOk == es);
		}
	}
	return es;
}

void HandleTableUpdater::removeEntry(AcDbObjectId)
{

}

void HandleTableUpdater::updateEntry(AcDbObjectId, Adesk::Int64)
{

}

Acad::ErrorStatus HandleTableUpdater::greatestHandle(void)
{

}

void HandleTableUpdater::updateToFiler(FreeSpaceAllocator*, long long &, long long &)
{

}

Acad::ErrorStatus HandleTableUpdater::addBlockAtCur(const HandleTableBlock&)
{

}

Acad::ErrorStatus HandleTableUpdater::deleteCurBlock(void)
{

}

Adesk::UInt32 HandleTableUpdater::numBlocks(void)
{

}

HandleOffsetBuffer* HandleTableUpdater::getBuffer(void)
{

}

void HandleTableUpdater::addBuffer(HandleOffsetBuffer*)
{

}
