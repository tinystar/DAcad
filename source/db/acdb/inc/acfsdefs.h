#ifndef _ACFS_DEFS_H_
#define _ACFS_DEFS_H_

static const int ACFS_HEADER_SIZE = 108;	// 0x6C

#define MEMBLKS_HEADER_SIGN		0x41630E3B
#define STRMBLK_HEADER_SIGN		0x4163003B

#define ACFS_STREAM_HEADER				ACRX_T("AcDb:Header")
#define ACFS_STREAM_AUXHEADER			ACRX_T("AcDb:AuxHeader")
#define ACFS_STREAM_CLASSES				ACRX_T("AcDb:Classes")
#define ACFS_STREAM_PREVIEW				ACRX_T("AcDb:Preview")
#define ACFS_STREAM_HANDLES				ACRX_T("AcDb:Handles")
#define ACFS_STREAM_TEMPLATE			ACRX_T("AcDb:Template")
#define ACFS_STREAM_OBJFREESPACE		ACRX_T("AcDb:ObjFreeSpace")
#define ACFS_STREAM_ACDBOBJECTS			ACRX_T("AcDb:AcDbObjects")
#define ACFS_STREAM_SHAREINFO			ACRX_T("AcDb:ShareInfo")
#define ACFS_STREAM_FILEDEPLIST			ACRX_T("AcDb:FileDepList")
#define ACFS_STREAM_SUMMARYINFO			ACRX_T("AcDb:SummaryInfo")
#define ACFS_STREAM_VBAPROJECT			ACRX_T("AcDb:VBAProject")
#define ACFS_STREAM_REVHISTORY			ACRX_T("AcDb:RevHistory")
#define ACFS_STREAM_APPINFO				ACRX_T("AcDb:AppInfo")
#define ACFS_STREAM_APPINFOHISTORY		ACRX_T("AcDb:AppInfoHistory")
#define ACFS_STREAM_ACDSPROTOTYPE_1B	ACRX_T("AcDb:AcDsPrototype_1b")

#define MAX_STREAM_NAME_LEN		64

enum DwgSectionId
{
	kSecNone = 0,
	kSecHeader = 1,
	kSecAuxHdr,
	kSecClasses,
	kSecPreview,
	kSecHandles,
	kSecTemplate,
	KSecObjFreeSpace,
	kSecObjects,
	kSecShareInfo,
	kSecFileDepList,
	kSecSummaryInfo,
	kSecVBAProject,
	kSecRevHistory,
	kSetAppInfo,
	kSecAppInfoHistory,
	kSecPrototype1b,
	kSecMax
};

struct AcFs_INFO_t
{
	Adesk::Int64	nMemBlkEndAddr;		// 0
	Adesk::UInt64	uTotalSize;			// 8
	Adesk::Int64	nTotalPosBlkSize;	// 16
	Adesk::Int64	nTotalNegBlkSize;	// 24
	Adesk::Int32	nPosBlkCount;		// 32
	Adesk::Int32	nNegBlkCount;		// 36
	Adesk::Int32	nAlignment;			// 40
	Adesk::Int32	nStreamCount;		// 48
	Adesk::Int32	nAppFlags;			// 52
};

struct AcFs6_INFO_t
{

};

struct AcFs_STREAMINFO_t
{
	Adesk::Int64	nTotalSize;			// 0
	Adesk::Int64	nUnk8;				// 8
	Adesk::UInt64	uFileSize;			// 16
	int				nComprType;			// 24
	int				nComprLevel;		// 28
	Adesk::UInt32	uBlockSize;			// 32
	int				nStreamId;			// 36
	Adesk::UInt32	uAppFlags;			// 40
	ACHAR			szStreamName[MAX_STREAM_NAME_LEN];	// 44
};

#endif // _ACFS_DEFS_H_