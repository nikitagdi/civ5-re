/// origin guess
//------------------------------------------------------------------------------------------------
//
//  ***************** FIRAXIS GAME ENGINE   ********************
//
//! \file		FLua.h
//! \author		Eric Jordan -- 3/12/2009
//! \brief		Includes core FLua headers
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef FLua_h
#define FLua_h
#pragma once

#include "FirePlace/FireWorks/FAssert.h"

#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaGlobalData.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaStaticFunctions.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypeExposure.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaPointerProtection.h"

namespace FLua
{
	// RegisterAll - Register all exposed statics to lua
	/// inline
	inline void RegisterAll(lua_State *L)
	{
		GlobalData::Register(L);
		StaticFunctions::Register(L);
		TypeExposures::Register(L, TypeExposures::sm_uiUnrestrictedExposure);
	}

	// RegisterAll - Register all exposed statics to lua
	/// inline
	inline void RegisterAll(lua_State *L, unsigned int uiCategoryFlags)
	{
		GlobalData::Register(L);
		StaticFunctions::Register(L, uiCategoryFlags);
		TypeExposures::Register(L, uiCategoryFlags);
	}

	// tointeger with a nul check.
	/// inline
	inline lua_Integer tointeger(lua_State* L, int iIndex) 
	{ 
		FAssert(!lua_isnil(L, iIndex)); 
		return lua_tointeger(L, iIndex);
	}

	// tonumber with a nul check.
	/// inline
	inline lua_Number tonumber(lua_State* L, int iIndex) 
	{ 
		FAssert(!lua_isnil(L, iIndex)); 
		return lua_tonumber(L, iIndex);
	}

	// toboolean with a nul check.
	/// inline
	inline int toboolean(lua_State* L, int iIndex) 
	{ 
		FAssert(!lua_isnil(L, iIndex)); 
		return lua_toboolean(L, iIndex);
	}

}

#endif
