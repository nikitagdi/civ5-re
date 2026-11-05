/// origin guess
#include <stdarg.h>
#include "FirePlace/FireWorks/FLua/Include/FLuaCommon.h"
#include "FirePlace/FireWorks/FCriticalSection.h"

/// 0x08dd7c43
void FLua::Details::Error(const TCHAR * szFormat, ...)
{
	TCHAR szMessage[1024];
	va_list args;
	va_start(args, szFormat);
	vsprintf_s(szMessage, sizeof(szMessage), szFormat, args);
	va_end(args);
	ErrorHandlerHolder::Get()(szMessage);
}

/// 0x08dd9545
bool FLua::Details::CCallWithErrorHandling(lua_State * L, lua_CFunction pfn, void * pvUserData)
{
	FLua::ms_CriticalSection.Enter();
	int iCPCallResults = lua_cpcall(L, pfn, pvUserData);
	if (iCPCallResults != 0)
	{
		HandleCPCallError(L, iCPCallResults);
		lua_settop(L, -2);
	}
	FLua::ms_CriticalSection.Leave();
	return iCPCallResults == 0;
}
