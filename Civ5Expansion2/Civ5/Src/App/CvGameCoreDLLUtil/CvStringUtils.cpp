/// origin binary
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStringUtils.h"
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

namespace CvStringUtils
{

/// 0x08b93df9
std::wstring FromUTF8ToUTF16(const std::string& strUTF8String)
{
	std::wstring wstrResult;
	if (strUTF8String.empty())
	{
		return wstrResult;
	}

	const char* pszUTF8 = strUTF8String.c_str();
	const int iLength = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1, NULL, 0);
	if (iLength == 0)
	{
		return wstrResult;
	}

	wchar_t* pwszBuffer = static_cast<wchar_t*>(malloc(iLength * sizeof(wchar_t)));
	if (MultiByteToWideChar(CP_UTF8, 0, pszUTF8, -1, pwszBuffer, iLength) > 0)
	{
		wstrResult.assign(pwszBuffer, wcslen(pwszBuffer));
	}
	free(pwszBuffer);
	return wstrResult;
}

}
