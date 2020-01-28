#include "acfs6.h"
#include "acutmacro.h"


AcFs6::AcFs6()
{

}

AcFs6::~AcFs6()
{

}

int AcFs6::CreateFile(const char*, unsigned int, unsigned int, unsigned int, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::CreateFile(const wchar_t*, unsigned int, unsigned int, unsigned int, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::OpenUsing(HANDLE, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::OpenUsing(AcStream*, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

Adesk::Boolean AcFs6::isOpen(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

int AcFs6::SetFileTime(const FILETIME*, const FILETIME*, const FILETIME*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetFileTime(FILETIME*, FILETIME*, FILETIME*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetFileInformationByHandle(BY_HANDLE_FILE_INFORMATION*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::CloseFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::Flush(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::WriteFile(const void*, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::ReadFile(void*, Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::SetFilePointer(Adesk::Int64, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::SetEndOfFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::ZeroData(Adesk::UInt64, Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetFilePointer(Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetFileSize(Adesk::UInt64*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFs6::SetCompressionType(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFs6::GetCompressionType(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFs6::SetCompressionLevel(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFs6::GetCompressionLevel(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFs6::SetBlockSize(Adesk::UInt64)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcFs6::SetMaxCache(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcFs6::SetAlignment(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFs6::GetAlignment(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFs6::EnableWriteCache(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Adesk::UInt64 AcFs6::GetBlockSize(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

void AcFs6::SetPrivateHeaderSize(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFs6::GetPrivateHeaderSize(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetAcFsInfo(AcFs_INFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetAcFs6Info(AcFs6_INFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetStreamInfo(int, AcFs_STREAMINFO_t*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFs6::ResyncSystemFilePointer(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFs6::WritePrivateHeader(const void*, int, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::ReadPrivateHeader(void*, int, int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::OpenStream(const char*, unsigned int, AcFsStream**)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::OpenStream(const wchar_t*, unsigned int, AcFsStream**)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::CreateStream(const char*, unsigned int*, unsigned int, unsigned int, AcFsStream**, int, Adesk::UInt64, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::CreateStream(const wchar_t*, unsigned int*, unsigned int, unsigned int, AcFsStream**, int, Adesk::UInt64, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::DeleteStream(const char*, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::DeleteStream(const wchar_t*, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFs6::RegisterCallback(AcFsCallBack*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

void AcFs6::DefineAppFlags(unsigned int, unsigned int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFs6::GetAppFlags(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

void AcFs6::SetKey(const unsigned int*)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

Adesk::Boolean AcFs6::IsRealAcFsFile(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return Adesk::kFalse;
}

void AcFs6::SetV6StreamCallbackFix(bool)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}

int AcFs6::GetLastWriteError(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return -1;
}

int AcFs6::GetNumberOfWriteErrors(void)
{
	AC_ASSERT_NOT_IMPLEMENTED();
	return 0;
}

void AcFs6::SetThreading(int)
{
	AC_ASSERT_NOT_IMPLEMENTED();
}
