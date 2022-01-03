#include "dbGlobalFuncs.h"
#include "rxmem.h"
#include "dbfiler.h"
#include "dbHandleData.h"
#include "dbHdlTblUpdater.h"
#include "dbHandleTable.h"


void acdbDelString(ACHAR*& pStr)
{
	if (pStr)
	{
		__acFree(pStr);
		pStr = NULL;
	}
}

Acad::ErrorStatus dwgInHandleOffsets(AcDbHandleTable& dbHdlTable, AcDbDwgFiler* pDwgFiler, HandleTableUpdater* pHdlTblUpdater, bool& bUnk)
{
	bUnk = false;
	AC_BYTE buf[2048];
 	HandleDataBase hdata(buf, 2048);

	Acad::ErrorStatus es = Acad::eOk;
	while (true)
	{
		Adesk::Int64 nCurOffset = pDwgFiler->tell();		// 528
		es = hdata.dwgIn(pDwgFiler);
		if (es != Acad::eOk)
			break;

		if (hdata.getUnk0() <= 2)
			break;

		hdata.start();

		AcDbHandle var_860;
		AcDbHandle var_878;

		Adesk::Int32 unk00 = 2;
		while (hdata.getUnk0() > unk00 && (unk00 >= 0 || hdata.getUnk0() != 2))
		{
			hdata.next();
		}

		if (es != Acad::eOk)
			break;

		if (pHdlTblUpdater != NULL)
		{
			es = pHdlTblUpdater->addBlock(var_860, var_878, buf, hdata.getUnk0() + 2, nCurOffset);
			if (es != Acad::eOk)
				break;
		}
	}

	return es;
}


Acad::ErrorStatus dwgInHandleOffsets(AcDbHandleTable& dbHdlTable, AcDbDwgFiler* pDwgFiler, HandleTableUpdater* pHdlTblUpdater, bool& bUnk)
{
// 	bUnk = false;
// 	AC_BYTE buf[2048];
// 	HandleDataBase hdata(buf, 2048);
// 	Adesk::Int64 nCurOffset = pDwgFiler->tell();		// 528
// 	Acad::ErrorStatus es = hdata.dwgIn(pDwgFiler);
// 	if (es != Acad::eOk)
// 		return es;
// 
// 	if (hdata.getUnk0() >= 3)
// 	{
// 		hdata.start();
// 
// 		while (Acad::eOk == es)
// 		{
// 			if (hdata.getUnk8() >= hdata.getUnk0() || hdata.getUnk8() < 0 && hdata.getUnk0() == 2)
// 			{
// 				if (NULL == pHdlTblUpdater)
// 				{
// 					hdata.resetBuffer();
// 				}
// 				pHdlTblUpdater->addBlock();
// 
// 
// 				if (!v21
// 					|| (v6 = (__int64 *)&v24,
// 					result = HandleTableUpdater::addBlock(
// 					v21,
// 					(const AcDbHandle *)&v24,
// 					(const AcDbHandle *)&v25,
// 					v28,
// 					v26 + 2,
// 					v20),
// 					!(_DWORD)result))
// 				{
// 					v5 = v8;
// 					HandleDataDef::resetBuffer(v8);
// 					goto LABEL_3;
// 				}
// 				return result;
// 			}
// 	}
}


Acad::ErrorStatus dwgInHandleOffsets(AcDbHandleTable& dbHdlTable, AcDbDwgFiler* pDwgFiler, HandleTableUpdater* pHdlTblUpdater, bool& bUnk)
{
	AcDbStub* var_870;
	Adesk::Int64 var_868;
	AcDbHandle var_860;
	AcDbHandle var_858;

	//mov     r13, rcx;		// bUnk
	HandleTableUpdater* var_880 = pHdlTblUpdater;
	AcDbDwgFiler* var_890 = pDwgFiler;
	AcDbHandleTable& var_878 = dbHdlTable;

	//mov     byte ptr[r13 + 0], 0
	bUnk = false;

	//lea     r15, [rbp + var_850]
	//lea     rsi, [rbp + var_828]
	//mov     rdi, r15
	//mov     edx, 800h
	//call    __ZN13HandleDataDefC2EPht; HandleDataDef::HandleDataDef(uchar *, ushort)
	AC_BYTE var_828[0x800];
	HandleDataBase var_850(var_828, 0x800);

	//lea     r14, [rbp + var_858]
	//mov     rbx, [rbp + var_890]
	//jmp     short loc_11308D
	goto loc_11308D;

loc_113082 :
    //mov     r15, r12
    //mov     rdi, r15; this
    //call    __ZN13HandleDataDef11resetBufferEv; HandleDataDef::resetBuffer(void)
	var_850.resetBuffer();

loc_11308D:
    //mov     rax, [rbx]
    //mov     rdi, rbx
    //call    qword ptr[rax + 210h]
    //mov[rbp + var_888], rax
	Adesk::Int64 var_888 = pDwgFiler->tell();		// 210h

    //mov     rdi, r15; this
    //mov     r12, r15
    //mov     rsi, rbx; AcDbDwgFiler *
    //call    __ZN14HandleDataBase5dwgInEP12AcDbDwgFiler; HandleDataBase::dwgIn(AcDbDwgFiler *)
	Acad::ErrorStatus es = var_850.dwgIn(pDwgFiler);

    //test    eax, eax
    //jnz     loc_113230
	if (es != Acad::eOk)
		goto loc_113230;

    //xor     eax, eax
    //cmp[rbp + var_850], 3
    //jl      loc_113230
	es = Acad::eOk;
	if (var_850.getUnk0() < 3)
		goto loc_113230;

    //mov     rdi, r12; this
    //call    __ZN13HandleDataDef5startEv; HandleDataDef::start(void)
	var_850.start();

    //xor     r15d, r15d
	var_868 = 0;	// ???
    //jmp     short loc_113109
	goto loc_113109;

loc_1130D2:
	//and     edx, 0FFFFBFFFh
	//mov[rax + 8], edx

loc_1130DB :
    //test    rcx, rcx
    //jnz     short loc_1130FA
    //mov     edx, [rax + 8]
    //test    dh, 80h
    //mov     rsi, rax
    //jz      short loc_1130F1
    //mov     rsi, [rax]
    //mov     edx, [rsi + 8]

loc_113109 :
	//xor     eax, eax
	es = Acad::eOk;

loc_11310B :
	//test    eax, eax
	//jnz     loc_113230
	if (es != Acad::eOk)
		goto loc_113230;

	//mov     r8d, [rbp + var_850]
	//mov     eax, [rbp + var_848]
	//cmp     eax, r8d
	//jge     loc_1131F2
	if (var_850.getUnk8() >= var_850.getUnk0())
		goto loc_1131F2;

	//test    eax, eax
	//jns     short loc_113137
	if (var_850.getUnk8() >= 0)
		goto loc_113137;

	//cmp     r8d, 2
	//jz      loc_1131F2
	if (var_850.getUnk0() == 2)
		goto loc_1131F2;

loc_113137 :
    //mov     rdi, r12; this
    //mov     rsi, r14; unsigned __int64 *
    //lea     rdx, [rbp + var_868]; __int64 *
    //call    __ZN13HandleDataDef4nextERyRx; HandleDataDef::next(ulong long &, long long &)
	bool b1 = var_850.next(var_858, var_868);

    //mov     cl, al
    //test    cl, cl
    //mov     eax, 28h; '('
    //mov     edx, 0
    //cmovnz  eax, edx
	es = Acad::eEndOfObject;	// 28h
	if (b1)
		es = Acad::eOk;

    //test    r15, r15
    //jnz     short loc_113174
	if (var_868 != 0)
		goto loc_113174;

    //mov     rdx, [rbp + var_858]
    //mov[rbp + var_860], rdx
	var_860 = var_858;
    //mov     r15, [rbp + var_868]

loc_113174 :
    //cmp     cl, 1
    //jnz     short loc_11310B
	if (b1)
		goto loc_11310B;

    //mov     rdi, [rbp + var_878]; this
    //lea     rsi, [rbp + var_870]; AcDbStub **
    //mov     rdx, r14; AcDbHandle *
    //call    __ZN15AcDbHandleTable9addHandleERP8AcDbStubRK10AcDbHandle; AcDbHandleTable::addHandle(AcDbStub *&, AcDbHandle const&)
	es = dbHdlTable.addHandle(var_870, var_858);

    //test    eax, eax
    //jz      short loc_1131A1
	if (es == Acad::eOk)
		goto loc_1131A1;

    //cmp     eax, 0Ah
    //jnz     loc_11310B
	if (es != eHandleExists)
		goto loc_11310B;

    //mov     byte ptr[r13 + 0], 1
	bUnk = true;

loc_1131A1 :
    //mov     rax, [rbp + var_870]
    //mov     edx, [rax + 8]
    //mov     rcx, [rbp + var_868]
    //test    dh, 80h
    //jz      short loc_1131BD
	// Stub operation

    //mov     rax, [rax]
    //mov     edx, [rax + 8]

loc_1131BD :
    //test    dh, 80h
    //jz      short loc_1131D1
    //mov     rdx, [rax]
    //or      dword ptr[rdx + 8], 100h
    //mov     edx, [rax + 8]

    //jmp     short loc_1131DA
	goto loc_1131DA;

loc_1131D1 :
    //or      edx, 100h
    //mov[rax + 8], edx

loc_1131DA :
	//test    dh, 80h
	//jz      loc_1130D2
	//mov     rdx, [rax]
	//and     dword ptr[rdx + 8], 0FFFFBFFFh

	//jmp     loc_1130DB
	goto loc_1130DB;

loc_1131F2 :
    //cmp[rbp + var_880], 0
    //jz      loc_113082
	if (pHdlTblUpdater == 0)
		goto loc_113082;

    //mov     rcx, [rbp + var_830]; unsigned __int8 *
    //add     r8d, 2; unsigned int
    //mov     rdi, [rbp + var_880]; this
    //lea     rsi, [rbp + var_860]; AcDbHandle *
    //mov     rdx, r14; AcDbHandle *
    //mov     r9, [rbp + var_888]; __int64
    //call    __ZN18HandleTableUpdater8addBlockERK10AcDbHandleS2_PKhjx; HandleTableUpdater::addBlock(AcDbHandle const&, AcDbHandle const&, uchar const*, uint, long long)
	es = pHdlTblUpdater->addBlock(var_860, var_858, var_830, var_850.getUnk0() + 2, var_888);

    //test    eax, eax
    //jz      loc_113082
	if (es == Acad::eOk)
		goto loc_113082;

loc_113230:
	return es;
}