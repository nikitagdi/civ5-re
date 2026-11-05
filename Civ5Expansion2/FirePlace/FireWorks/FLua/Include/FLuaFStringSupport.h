/// origin guess
//------------------------------------------------------------------------------------------------
//
//  ***************** FIRAXIS GAME ENGINE   ********************
//
//! \file		FLuaFStringSupport.h
//! \author		Eric Jordan -- 3/12/2009
//! \brief		Include to file to provide FString support for FLua.  Requires FString.
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef FLuaFStringSupport_h
#define FLuaFStringSupport_h
#pragma once

#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaCommon.h"

namespace FLua
{
	namespace Details
	{
		/// size 0x1
		template<> struct LuaAnalog<FStringA>{ typedef const char* Result; };
		/// size 0x1
		template<> struct LuaAnalog<FStringA&>{ typedef FStringA Result; };
		/// size 0x1
		template<> struct LuaAnalog<const FStringA&>{ typedef const char *Result; };

		/// inline
		template<> static inline FStringA Get(lua_State *L, int idx) { return lua_tostring(L, idx); }
		/// inline
		static inline void Push(lua_State *L, const FStringA &sVal) { lua_pushstring(L, sVal.c_str()); }

		/// inline
		template<> static const char *DescribeType<FStringA>() { return "FString"; }
		/// inline
		template<> static const char *DescribeType<FStringA&>() { return "FString"; }
		/// inline
		template<> static const char *DescribeType<const FStringA&>() { return "const FString"; }

		// wchar support
		/// inline
		static inline void Push(lua_State *L, const wchar* szVal)
		{
			// Convert string to UTF-8 for lua
			FStringA sConverted;
			FStringW::CopyToUTF8(szVal, &sConverted);
			lua_pushstring(L, sConverted.c_str());
		}

		/// inline
		template<> static const char *DescribeType<const wchar*>() { return "const wchar"; }

		/// size 0x1
		template<> struct LuaAnalog<FStringW&>{ typedef FStringW Result; };
		/// size 0x1
		template<> struct LuaAnalog<const FStringW&>{ typedef FStringW Result; };

		/// inline
		template<> static inline FStringW Get(lua_State *L, int idx)
		{ 
			FStringW sConvert;
			sConvert.CopyUTF8(lua_tostring(L, idx));
			return sConvert; 
		}

		/// inline
		static inline void Push(lua_State *L, const FStringW &sVal)
		{
			Push(L, sVal.c_str());
		}

		/// inline
		template<> static const char *DescribeType<FStringW>() { return "FString"; }
		/// inline
		template<> static const char *DescribeType<FStringW&>() { return "FString"; }
		/// inline
		template<> static const char *DescribeType<const FStringW&>() { return "const FString"; }
	}
}

#endif
