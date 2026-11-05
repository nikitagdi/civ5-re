/// origin guess
//------------------------------------------------------------------------------------------------
//
//  ***************** FIRAXIS GAME ENGINE   ********************
//
//! \file		FLuaGlobalData.h
//! \author		Eric Jordan -- 3/23/2009
//! \brief		Use to expose static global data to lua.
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef FLuaGlobalData_h
#define FLuaGlobalData_h
#pragma once

#include "FirePlace/FireWorks/FLua/Include/FLuaCommon.h"

// FLUA_EXPOSE_GLOBAL_DATA(PTR, NAME)
// Exposes PTR to lua with NAME as the global variable name in lua
#define FLUA_EXPOSE_GLOBAL_DATA(DATA_PTR, NAME) \
	static FLua::GlobalData::Registrar LuaReg_Data_##NAME##(#NAME, (##DATA_PTR##))

namespace FLua
{
	/// size 0xc4
	class GlobalData
	{
	public:
		/// inline
		static const unsigned int sm_uiMaxDataEntries = 16;

		/// size 0x1
		struct Registrar {
			/// inline
			template<class T>
			inline Registrar( _In_z_ const char *szName, T *pData) {
				GlobalData &kRef = Ref();

				// If there are no free entries for global data then we're in trouble
				assert(kRef.m_uiDataEntryCount < sm_uiMaxDataEntries && "Not enough space for exposing global data");

				// Add the entry
				kRef.m_aDataEntries[kRef.m_uiDataEntryCount++].Set(szName, pData);
			}
		};

		// Register globals with the given lua state
		/// inline
		static void Register(lua_State *L) {
			GlobalData &kRef = Ref();
			for( size_t i = 0; i < kRef.m_uiDataEntryCount; ++i ) {
				const DataEntry &kEntry = kRef.m_aDataEntries[i];
				(*kEntry.m_pfnAddGlobal)(L, kEntry.m_pvData, kEntry.m_szName);
			}
		}

	private:
		/// size 0xc
		struct DataEntry
		{
			typedef void(AddGlobalFn)(lua_State*, void*, const char*);
			/// inline
			template<class T>
			static void AddGlobal(lua_State *L, void *pvData, const char *szName) {
				Details::Push(L, (T*)pvData);
				lua_setglobal(L, szName);
			}

			/// inline
			template<class T>
			inline void Set(const char *szName, T *pData) {
				m_szName = szName;
				m_pvData = (void*)pData;
				m_pfnAddGlobal = &DataEntry::AddGlobal<T>;
			}

			const char *m_szName; ///< +0x0
			void *m_pvData; ///< +0x4
			AddGlobalFn *m_pfnAddGlobal; ///< +0x8
		};

		/// inline
		GlobalData() : m_uiDataEntryCount(0) {}
		/// inline
		static inline GlobalData &Ref() { static GlobalData kInst; return kInst; }

		DataEntry m_aDataEntries[sm_uiMaxDataEntries]; ///< +0x0
		size_t m_uiDataEntryCount; ///< +0xc0
	};
}

#endif
