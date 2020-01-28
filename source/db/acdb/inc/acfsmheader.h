#ifndef _ACFS_MHEADER_H_
#define _ACFS_MHEADER_H_

#include "adesk.h"
#include <windows.h>
#include "acfsfheader.h"

class AcFs_mbheader;
class AcFsHeap;

class AcFs_mheader
{
	friend class AcFsI;
public:
	AcFs_mheader();
	~AcFs_mheader();

	void Reset(void);

	int OpenFile(const void*, unsigned int, unsigned int, unsigned int, unsigned int, bool, HANDLE, Adesk::Int64);
	int CloseFile(void);

	int ReadData(void*, int, Adesk::Int64);
	int ReadBlock(AcFs_mbheader*, void*, int, int);
	int ReadFileHeader(Adesk::Int64);
	int ReadMemBlks(void);

protected:
	AcFs_mbheader* ConvertID(int);

protected:
	AcFs_mbheader*		m_pUnk0;		// 0//0h
	AcFs_mbheader*		m_pUnk8;		// 8//8h
	AcFs_mbheader*		m_pUnk16;		// 16//10h
	AcFs_mbheader*		m_pMemBlkHead;	// 24//18h
	AcFs_mbheader*		m_pMemBlkTail;	// 32//20h
	Adesk::Int64		m_nUnk40;		// 40//28h
	Adesk::Int64		m_nUnk48;		// 48//30h
	Adesk::Int64		m_nCurPos;		// 56//38h
	Adesk::Int32		m_nUnk64;		// 64//40h
	Adesk::Int32		m_nUnk68;		// 68//44h
	Adesk::Int32		m_nUnk72;		// 72//48h
	Adesk::Int32		m_nUnk76;		// 76//4Ch
	Adesk::Int32		m_nUnk80;		// 80//50h
	Adesk::Int32		m_nUnk84;		// 84//54h
	Adesk::Int32		m_nUnk88;		// 88//58h
	Adesk::Int32		m_nUnk92;		// 92//5Ch
	HANDLE				m_hFile;		// 96//60h
	AcFs_mbheader*		m_pUnk104;		// 104//68h
	AcFs_mbheader**		m_pUnk112;		// 112//70h
	AcFs_mbheader**		m_pUnk120;		// 120//78h
	Adesk::UInt32		m_uUnk140;		// 140//8Ch
	AcFs_mbheader*		m_pUnk144;		// 144//90h
	AcFs_fheader		m_fHeader;		// 152//98h
// 		char			m_szSign[16];		// 152 + 0  = 152
// 		Adesk::Int32	m_nUnk16;			// 152 + 16 = 168
// 		Adesk::Int32	m_nUnk20;			// 152 + 20	= 172
// 		Adesk::Int32	m_nUnk24;			// 152 + 24 = 176
// 		Adesk::Int32	m_nUnk28;			// 152 + 28 = 180
// 		Adesk::Int32	m_nUnk32;			// 152 + 32 = 184
// 		Adesk::Int32	m_nUnk36;			// 152 + 36 = 188
// 		Adesk::Int32	m_nUnk40;			// 152 + 40 = 192
// 		Adesk::Int64	m_nUnk44;			// 152 + 44 = 196
// 		Adesk::Int64	m_nUnk52;			// 152 + 52 = 204
// 		Adesk::Int32	m_nUnk60;			// 152 + 60 = 212
// 		Adesk::Int32	m_nUnk64;			// 152 + 64 = 216
// 		Adesk::Int32	m_nUnk68;			// 152 + 68 = 220
// 		Adesk::Int32	m_nUnk72;			// 152 + 72 = 224
// 		Adesk::Int32	m_nUnk76;			// 152 + 76 = 228
// 		Adesk::Int32	m_nUnk80;			// 152 + 80 = 232
// 		Adesk::Int64	m_nUnk84;			// 152 + 84 = 236
// 		Adesk::Int32	m_nUnk92;			// 152 + 92 = 244
// 		Adesk::Int32	m_nUnk96;			// 152 + 96 = 248
// 		Adesk::Int32	m_nUnk100;			// 152 + 100 = 252
// 		Adesk::Int32	m_nUnk104;			// 152 + 104 = 256
	Adesk::UInt32		m_uUnk260;		// 260//104h
	Adesk::Boolean		m_bKeepFile;	// 264//108h
	Adesk::Int32		m_nUnk268;		// 268//10Ch
	Adesk::Int32		m_nUnk272;		// 272//110h
	AcFsHeap*			m_pFsHeap;		// 280//118h
	AC_BYTE				m_magic[256];	// 288//120h
};

#endif // _ACFS_MHEADER_H_