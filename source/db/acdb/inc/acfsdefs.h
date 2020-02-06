#ifndef _ACFS_DEFS_H_
#define _ACFS_DEFS_H_

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
	Adesk::Int64	uUnk0;		// 0
	Adesk::UInt64	uSize;		// 8
	Adesk::Int64	pUnk16;		// 16
	Adesk::Int64	pUnk24;		// 24
	Adesk::Int32	nUnk32;		// 32
	Adesk::Int32	nUnk36;		// 36
	Adesk::Int32	uUnk40;		// 40
	Adesk::Int32	uUnk48;		// 48
	Adesk::Int32	uUnk52;		// 52
};

struct AcFs6_INFO_t
{

};

struct AcFs_STREAMINFO_t
{
	Adesk::Int64	nUnk0;		// 0
	Adesk::Int64	nUnk8;		// 8
	Adesk::UInt64	uUnk16;		// 16
	int				nUnk24;		// 24
	int				nUnk28;		// 28
	Adesk::UInt32	uUnk32;		// 32
	int				nUnk36;		// 36
	Adesk::UInt32	uUnk40;		// 40
	ACHAR			szStreamName[MAX_STREAM_NAME_LEN];	// 44
};

#endif // _ACFS_DEFS_H_