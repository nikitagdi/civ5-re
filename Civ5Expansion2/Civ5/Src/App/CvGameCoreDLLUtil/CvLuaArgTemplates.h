/// origin guess

#pragma once 

#include <lua.h>
#ifndef CVLUAARGTEMPLATES
#define CVLUAARGTEMPLATES

namespace CvLuaArgs
{
	//To
	/// inline
	template<typename T>
	static T toValue(lua_State* L, int idx)
	{
		//This is pretty unsafe, but common.
		//Assume T is a poorly designed enum =(
		return (T)lua_tointeger(L, idx);
	}

	/// inline
	template<> static int toValue(lua_State* L, int idx)
	{
		return lua_tointeger(L, idx);
	}

	/// inline
	template<> static bool toValue(lua_State* L, int idx)
	{
		return lua_toboolean(L, idx) != 0;
	}

	//Push
	/// inline
	template<typename T>
	static void pushValue(lua_State* L, T t)
	{
		//This is pretty unsafe, but common.
		//Assume T is a poorly designed enum =(
		lua_pushinteger(L, t);
	}

	/// inline
	template<>
	static void pushValue(lua_State* L, int t)
	{
		lua_pushinteger(L, t);
	}

	/// inline
	template<>
	static void pushValue(lua_State* L, bool t)
	{
		lua_pushboolean(L, t);
	}

}

#endif //CVLUAARGTEMPLATES
