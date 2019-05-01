#include "dbHandleTable.h"
#include "dbmain.h"
#include <string>
#include <iostream>

using namespace std;

void testAcDbHandleTable()
{
	AcDbHandleTable* pHandleTable = new AcDbHandleTable(NULL, false);
	AcDbStub* pStub = NULL;
	Acad::ErrorStatus es = pHandleTable->addHandle(pStub, AcDbHandle(1));
	es = pHandleTable->addHandle(pStub, AcDbHandle(1));
	es = pHandleTable->addHandle(pStub, AcDbHandle(2));
	es = pHandleTable->addHandle(pStub, AcDbHandle(25));
	es = pHandleTable->addHandle(pStub, AcDbHandle(10));
	es = pHandleTable->addHandle(pStub, AcDbHandle(100));
	es = pHandleTable->addHandle(pStub, AcDbHandle(70));
	es = pHandleTable->addHandle(pStub, AcDbHandle(256));
	es = pHandleTable->addHandle(pStub, AcDbHandle(321));
	es = pHandleTable->addHandle(pStub, AcDbHandle(11));
	es = pHandleTable->addHandle(pStub, AcDbHandle(3));
	es = pHandleTable->addHandle(pStub, AcDbHandle(5));
	es = pHandleTable->addHandle(pStub, AcDbHandle(555));
	es = pHandleTable->addHandle(pStub, AcDbHandle(7));
	es = pHandleTable->addHandle(pStub, AcDbHandle(8));
	es = pHandleTable->addHandle(pStub, AcDbHandle(6));
	es = pHandleTable->addHandle(pStub, AcDbHandle(9));
	es = pHandleTable->addHandle(pStub, AcDbHandle(15));
	es = pHandleTable->addHandle(pStub, AcDbHandle(13));
	es = pHandleTable->addHandle(pStub, AcDbHandle(14));
	//delete pHandleTable;
}

void testReadDwgFile()
{
	AcDbDatabase* pDb = new AcDbDatabase();
	wstring sDwgFile;
	wcin >> sDwgFile;
	pDb->readDwgFile(sDwgFile.c_str());
	delete pDb;
}

void testDatabaseEntry()
{
	//testAcDbHandleTable();
	testReadDwgFile();
}