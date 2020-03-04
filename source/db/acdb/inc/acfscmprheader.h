#ifndef _ACFS_CMPR_HEADER_H_
#define _ACFS_CMPR_HEADER_H_

#include "adesk.h"

#pragma pack (push, 1)

struct AcFsComprHeader
{
	Adesk::UInt32	uSignature;		// 0
	Adesk::UInt32	uDecomprSize;	// 4
	Adesk::Int32	nComprSize;		// 8
	Adesk::UInt32	uUnk12;			// 12
	Adesk::UInt32	uCheckSum;		// 16

	AcFsComprHeader()
		: uSignature(0)
		, uDecomprSize(0)
		, nComprSize(0)
		, uUnk12(2)
		, uCheckSum(0)
	{}
};

struct AcFsCodedComprHeader
{
	Adesk::UInt32	uSignature;		// 0
	Adesk::Int32	nSectionId;		// 4
	Adesk::UInt32	uComprSize;		// 8
	Adesk::UInt32	uBlockSize;		// 12
	Adesk::Int64	nStartPos;		// 16
	Adesk::UInt32	uCheckSum1;		// 24
	Adesk::UInt32	uCheckSum2;		// 28
};

#pragma pack (pop)

#endif // _ACFS_CMPR_HEADER_H_