/// origin guess

#pragma once 

#include "FirePlace/FireWorks/FLua/Include/FLuaCommon.h"
#ifndef CVLUASTATICINSTANCE_H

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvLuaMethodWrapper.h"

/// size ?
template<class Derived, class InstanceType>
class CvLuaStaticInstance : public CvLuaMethodWrapper<Derived, InstanceType>
{
public:	
	/// inline
	static void Register(lua_State* L);

	//! Used by CvLuaMethodWrapper to know where first argument is.
	/// inline
	static const int GetStartingArgIndex();

protected:
	/// inline
	static int pRegister(lua_State* L);

	//! Called inside of pRegisterMembers to register a method.
	/// inline
	static void RegisterMethod(lua_State* L, lua_CFunction func, const char* funcName);
};

//------------------------------------------------------------------------------
// template implementation
//------------------------------------------------------------------------------
/// inline
template<class Derived, class InstanceType>
void CvLuaStaticInstance<Derived, InstanceType>::Register(lua_State* L)
{
	FLua::Details::CCallWithErrorHandling(L, pRegister);
}
//------------------------------------------------------------------------------
/// inline
template<class Derived, class InstanceType>
const int CvLuaStaticInstance<Derived, InstanceType>::GetStartingArgIndex()
{
	return 1;
}
//------------------------------------------------------------------------------
/// inline
template<class Derived, class InstanceType>
int CvLuaStaticInstance<Derived, InstanceType>::pRegister(lua_State* L)
{
	const char* szInstanceName = Derived::GetInstanceName();
	lua_getglobal(L, szInstanceName);
	if(lua_isnil(L, -1))
	{
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, szInstanceName);
	}

	Derived::RegisterMembers(L);

	return 0;
}
//------------------------------------------------------------------------------
/// inline
template<class Derived, class InstanceType>
void CvLuaStaticInstance<Derived, InstanceType>::RegisterMethod(lua_State *L, lua_CFunction func, const char* funcName)
{
	lua_pushcclosure(L, func, 0);
	lua_setfield(L, -2, funcName);
}
//------------------------------------------------------------------------------


#endif //CVLUASTATICINSTANCE_H
