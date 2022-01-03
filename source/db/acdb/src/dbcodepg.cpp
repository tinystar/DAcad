#include "dbcodepg.h"
#include "acutmacro.h"


//extern bool IsValidCodePage(uint);

bool isValidWinCodePage(code_page_id cpId)
{
	static code_page_id lastValue = (code_page_id)-1;
	static bool lastResult = false;
	if (lastValue == cpId)
		return lastResult;

	lastValue = cpId;
	AC_ASSERT_NOT_IMPLEMENTED();
	lastResult = true;
	return lastResult;
}
