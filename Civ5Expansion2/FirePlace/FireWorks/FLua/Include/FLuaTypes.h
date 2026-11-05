/// origin binary
//------------------------------------------------------------------------------------------------
//
//  ***************** FIRAXIS GAME ENGINE   ********************
//
//! \file		FLuaTypes.h
//! \author		Eric Jordan -- 3/12/2009
//! \brief		Classes that wrap lua types for C++
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef FLuaTypes_h
#define FLuaTypes_h
#pragma once

#include "FirePlace/FireWorks/FLua/Include/FLuaCommon.h"

// FLUA_EXPOSE_TO_TABLE_EX(FUNC, NAME)
// Adds FUNC to TABLE with NAME as the function name in lua
#define FLUA_EXPOSE_TO_TABLE_EX(FUNC, NAME, TABLE) \
	void LuaExpose_##NAME##(lua_State *L) { \
		FLua::Table t(L, #TABLE); \
		if( !t.Valid() ) { \
			t = FLua::Table::Create(L); \
			FLua::Table kGlobals(L); \
			kGlobals[#TABLE] = t; \
		} \
		t.AddMemberFunction(#NAME, &FUNC); \
	} \
	static FLua::StaticFunctions::CustomRegistrar LuaReg_##NAME##(&LuaExpose_##NAME##)

// FLUA_EXPOSE_TO_TABLE(FUNC)
// Adds FUNC to TABLE with FUNC as the function name in lua
#define FLUA_EXPOSE_TO_TABLE(FUNC, TABLE) FLUA_EXPOSE_TO_TABLE_EX(FUNC, FUNC, TABLE)

namespace FLua
{
	class Metatable;
	class String;

	// SharedLuaRegistry - Determines if FLua may assume that all lua states share the same registry.
	/// size 0x1
	class SharedLuaRegistry
	{
	public:
		/// inline
		static inline void Set(bool bAllowed) { sm_bAllowed = bAllowed; }
		/// inline
		static inline bool Get() { return sm_bAllowed; }

	private:
		/// 0x101b3b8c
		static bool sm_bAllowed;
	};

	/// size 0x8
	class Value
	{
	public:
		/// 0x08dda018
		Value();
		/// 0x08dda02a
		Value(const Value &kRhs);
		/// 0x08dda0a0
		Value(const StackValue &kStackValue); // Get from lua stack value
		/// 0x08dda0fe
		Value(lua_State *L, int iStackIndex); // Get from the lua stack
		/// 0x08dda160
		Value(lua_State *L, _In_z_ const char *szName); // Find with FLua::GetGlobal
		/// 0x08dda1b6
		~Value();

		/// 0x08dda22a
		void Swap(Value &kRhs);

		/// 0x08dda24c
		const Value &operator =(const Value &kRhs);
		/// 0x08dda2c4
		void Set(lua_State *L, int iStackIndex); // Get from the lua stack
		/// 0x08dda328
		void Set(lua_State *L, _In_z_ const char *szName); // Find with FLua::GetGlobal

		/// inline
		inline void Clear() { Unref(); }
		/// 0x08dda386
		void LuaStateDestroyed(); // Warning: Only call if the lua state has already been destroyed

		/// inline
		inline bool Valid() const { return m_pkLuaState && m_iRef != LUA_NOREF; }

		/// 0x08dda398
		const bool operator ==(const Value &kRhs) const;
		/// inline
		inline const bool operator !=(const Value &kRhs) const { return !(operator==(kRhs)); }

		/// 0x08dda45a
		bool Push() const;

		/// 0x08dda4b4
		Type GetType() const;
		/// 0x08dda536
		_Ret_z_ const char *GetTypeName() const;

		/// 0x08dda5d2
		bool IsNil() const;

		/// 0x08dda62e
		String ToString() const;
		/// 0x08dda70a
		const void *ToPointer() const;

		// Basic type conversions
		/// inline
		inline operator bool() const { return GetAs<bool>(); }
		/// inline
		inline operator int() const { return GetAs<int>(); }
		/// inline
		inline operator char() const { return GetAs<char>(); }
		/// inline
		inline operator unsigned int() const { return GetAs<unsigned int>(); }
		/// inline
		inline operator float() const { return GetAs<float>(); }
		/// inline
		inline operator double() const { return GetAs<double>(); }
		/// inline
		inline operator const char*() const { return GetAs<const char*>(); }
		/// inline
		template<class T> _Ret_opt_ operator T*() const { return GetAs<T*>(); }
		
		// GetAs - Interpret this value as a specific C++ type
		/// inline
		template<class T>
		T GetAs() const {
			T ret = T(); // Don't use with reference types!!! ...EVER!!!

			// Try pushing this value onto the lua stack
			if( Push() ) {
				// Get the lua analog for this type off of the lua stack
				typedef typename Details::LuaAnalog<T>::Result Analog;
				Analog analog = Details::Get<Analog>(m_pkLuaState, lua_gettop(m_pkLuaState));

				// Validate the value from lua
				bool bValid(true); Details::ArgValidator<T>::Validate(analog, bValid);

				// Convert and return the value if it is valid
				if( bValid ) ret = Details::FromLuaAnalog<T>::Convert(analog);

				// Finally pop the lua value off of the stack
				lua_pop(m_pkLuaState, 1);
			}
			return ret;
		}

		// IsOfType - Determines if the value may be cast to a pointer of type T
		/// inline
		template<class T>
		bool IsOfType() const {
			if( Push() ) {
				bool bCompatible = Details::CompatibleType<T>(m_pkLuaState, lua_gettop(m_pkLuaState));
				lua_pop(m_pkLuaState, 1);
				return bCompatible;
			}
			else return false;
		}

		/// 0x08dda760
		Metatable GetMetatable() const;

		// FromCFunction - Makes an FLua::Value out of a C function
		/// inline
		template<class TFunc>
		static Value FromCFunction(lua_State *L, TFunc pfn, _In_z_ const char *szFuncName)
		{
			Details::PushFunction(L, pfn, szFuncName);
			Value fnFunc(L, -1);
			lua_pop(L, 1);
			return fnFunc;
		}

		/// inline
		inline lua_State *GetLuaState() const { return m_pkLuaState; }

		// WARNING: LuaValue owns this lua ref!
		/// inline
		inline int GetRef() const { return m_iRef; }

	private:
		/// 0x08dda1e0
		void Unref();

		lua_State *m_pkLuaState; ///< +0x0
		int m_iRef; ///< +0x4
	};

	/// size 0x8
	class String
	{
	public:
		/// inline
		String() : m_kLuaVal() {}
		/// inline
		String(const String &kRhs) : m_kLuaVal(kRhs.m_kLuaVal) {}
		/// inline
		String(const Value &kLuaVal) : m_kLuaVal(kLuaVal) {}
		/// inline
		String(const StackValue &kStackValue) : m_kLuaVal(kStackValue) {}
		/// inline
		String(lua_State *L, int iStackIndex) : m_kLuaVal(L, iStackIndex) {}
		/// inline
		~String() {}

		/// 0x08dda78c
		static String Create(lua_State *L, _In_z_ const char *sz);

		/// inline
		inline operator const char*() const { return c_str(); }
		/// 0x08dda7f6
		_Ret_opt_ const char *c_str() const;

		/// inline
		inline void Swap(String &kRhs) { m_kLuaVal.Swap(kRhs.m_kLuaVal); }

		/// inline
		inline const String &operator =(const String &kRhs) { m_kLuaVal = kRhs.m_kLuaVal; return *this; }
		/// inline
		inline const String &operator =(const Value &kLuaValue) { m_kLuaVal = kLuaValue; return *this; }
		/// inline
		inline void Set(lua_State *L, int iStackIndex) { m_kLuaVal.Set(L, iStackIndex); }
		/// inline
		inline void Set(lua_State *L, _In_z_ const char *szName) { m_kLuaVal.Set(L, szName); }

		/// inline
		inline void Clear() { m_kLuaVal.Clear(); }

		// Warning: Only call if the lua state has already been destroyed
		/// inline
		inline void LuaStateDestroyed() { m_kLuaVal.LuaStateDestroyed(); }

		/// 0x08dda8aa
		bool Valid() const;

		/// inline
		inline const bool operator ==(const String &kRhs) const { return m_kLuaVal == kRhs.m_kLuaVal; }
		/// inline
		inline const bool operator !=(const String &kRhs) const { return !(m_kLuaVal == kRhs.m_kLuaVal); }
		/// inline
		inline const bool operator ==(const Value &kRhs) const { return m_kLuaVal == kRhs; }
		/// inline
		inline const bool operator !=(const Value &kRhs) const { return !(m_kLuaVal == kRhs); }

		// Concat operators
		/// 0x08dda906
		String operator +(const String &sRhs) const;
		/// 0x08dda9e6
		String operator +( _In_z_ const char *szRhs) const;

		// Prepend and Append functionality
		/// 0x08ddaada
		void Prepend(const String &sStart);
		/// 0x08ddab8a
		void Prepend( _In_z_ const char *szStart);
		/// 0x08ddac14
		void Append(const String &sEnd);
		/// 0x08ddacc0
		void Append( _In_z_ const char *szEnd);

		/// 0x08dda856
		bool Push() const;

		/// inline
		inline const Value &GetLuaValue() const { return m_kLuaVal; }
		/// inline
		inline lua_State *GetLuaState() const { return m_kLuaVal.GetLuaState(); }

	private:
		Value m_kLuaVal; ///< +0x0
	};

	/// size 0x8
	class Table
	{
	public:
		/// inline
		Table() : m_kLuaVal() {}
		/// inline
		Table(const Table &kRhs) : m_kLuaVal(kRhs.m_kLuaVal) {}
		/// inline
		Table(const Value &kLuaVal) : m_kLuaVal(kLuaVal) {}
		/// inline
		Table(const StackValue &kStackValue) : m_kLuaVal(kStackValue) {}
		/// inline
		Table(lua_State *L, int iStackIndex) : m_kLuaVal(L, iStackIndex) {}
		/// inline
		Table(lua_State *L, const char *szName) : m_kLuaVal(L, szName) {}
		/// inline
		explicit Table(lua_State *L) : m_kLuaVal(L, LUA_GLOBALSINDEX) {} // Globals table for lua state
		/// inline
		~Table() {}

		/// 0x08ddad6c
		static Table Create(lua_State *L, unsigned int uiArrayReserve = 0, unsigned int uiNonArrayReserve = 0);

		/// inline
		inline void Swap(Table &kRhs) { m_kLuaVal.Swap(kRhs.m_kLuaVal); }

		/// inline
		inline const Table &operator =(const Table &kRhs) { m_kLuaVal = kRhs.m_kLuaVal; return *this; }
		/// inline
		inline const Table &operator =(const Value &kLuaValue) { m_kLuaVal = kLuaValue; return *this; }
		/// inline
		inline void Set(lua_State *L, int iStackIndex) { m_kLuaVal.Set(L, iStackIndex); }
		/// inline
		inline void Set(lua_State *L, const char *szName) { m_kLuaVal.Set(L, szName); }

		/// inline
		inline void Clear() { m_kLuaVal.Clear(); }

		// Warning: Only call if the lua state has already been destroyed
		/// inline
		inline void LuaStateDestroyed() { m_kLuaVal.LuaStateDestroyed(); }

		/// 0x08ddadf0
		bool Valid() const;

		/// inline
		inline const bool operator ==(const Table &kRhs) const { return m_kLuaVal == kRhs.m_kLuaVal; }
		/// inline
		inline const bool operator !=(const Table &kRhs) const { return !(m_kLuaVal == kRhs.m_kLuaVal); }
		/// inline
		inline const bool operator ==(const Value &kRhs) const { return m_kLuaVal == kRhs; }
		/// inline
		inline const bool operator !=(const Value &kRhs) const { return !(m_kLuaVal == kRhs); }

		/// 0x08ddae4c
		bool Push() const;

		/// 0x08ddaea2
		Metatable GetMetatable() const;
		/// 0x08ddaece
		void SetMetatable(const Table &kMetaTable);

		/// 0x08ddaf38
		bool UsesWeakKeys() const;
		/// 0x08ddb024
		bool UsesWeakValues() const;
		/// inline
		inline void UseWeakKeys(bool bUseWeakKeys = true) { SetMode(bUseWeakKeys, UsesWeakValues()); }
		/// inline
		inline void UseWeakValues(bool bUseWeakValues = true) { SetMode(UsesWeakKeys(), bUseWeakValues); }

		/// 0x08ddaf7a
		_Ret_opt_z_ const char *GetMode() const;
		/// 0x08ddb066
		void SetMode( _In_z_ const char *szMode);
		/// 0x08ddb12c
		void SetMode(bool bWeakKeys, bool bWeakValues);

		/// inline
		inline const Value &GetLuaValue() const { return m_kLuaVal; }
		/// inline
		inline lua_State *GetLuaState() const { return m_kLuaVal.GetLuaState(); }

		// WARNING: LuaValue owns this lua ref!
		/// inline
		inline int GetRef() const { return m_kLuaVal.GetRef(); }

		// Represents a field for the [const char*] operator
		/// size 0xc
		class Field {
		private:
			friend Table;
			/// inline
			template<class T> Field(const Table &kTable, T key) : m_kTable((Table&)kTable) {
				typedef typename Details::PushAnalog<T>::Result KeyAnalog;
				lua_State *L = kTable.GetLuaState();
				if( L != NULL )
				{
					KeyAnalog keyAnalog = Details::ToLuaAnalog<KeyAnalog>::Convert(key);
					lua_checkstack(L, 1);
					Details::Push(L, keyAnalog); // Push the key
					m_kKey.Set(L, -1);	// Ref the key
					lua_pop(L, 1); // Pop the key
				}
			}

		public:
			/// inline
			Field(const Field &kRhs) : m_kTable(kRhs.m_kTable), m_kKey(kRhs.m_kKey) {}
			/// 0x08ddb85e
			const Field &operator=(const Field &kRhs);

			/// inline
			Type GetType() const { return m_kTable.GetField(m_kKey).GetType(); }
			/// inline
			_Ret_z_ const char *GetTypeName() const { return m_kTable.GetField(m_kKey).GetTypeName(); }
			/// inline
			bool Exists() const { return !m_kTable.GetField(m_kKey).IsNil(); }
			/// inline
			inline FLua::String ToString() const { return m_kTable.GetField(m_kKey).ToString(); }
			/// inline
			template<class T> bool IsOfType() const { return m_kTable.GetField(m_kKey).IsOfType<T>(); }
			/// inline
			template<class T> operator T() const { return m_kTable.GetField(m_kKey); }
			/// inline
			template<class T> inline const Field &operator =(T val) { m_kTable.SetField<T>(m_kKey, val); return *this; }

			/// inline
			inline const bool operator ==(const Field &kRhs) const { return kRhs.m_kTable.GetField(kRhs.m_kKey) == m_kTable.GetField(m_kKey); }
			/// inline
			inline const bool operator !=(const Field &kRhs) const { return kRhs.m_kTable.GetField(kRhs.m_kKey) != m_kTable.GetField(m_kKey); }

		private:
			Table &m_kTable; ///< +0x0
			Value m_kKey; ///< +0x4
		};

		// [] operators
		/// inline
		template<class T> inline Field operator [](T key) { return Field(*this, key); }
		/// inline
		template<class T> inline const Field operator [](T key) const { return Field(*this, key); }

		// GetField - Get the value for a given field
		/// 0x08ddb17e
		const Value GetField( _In_z_ const char *szField) const;
		/// 0x08ddb20a
		const Value GetField(int iIndex) const;
		/// 0x08ddb2a6
		const Value GetField(const Value &kKey) const;
		/// 0x08ddb3ac
		bool PushField(const Value &kKey) const;

		// SetField - Assign a value to a field
		/// inline
		template<class T>
		void SetField( _In_z_ const char *szField, T val) {
			if( Push() ) {
				lua_State *L = m_kLuaVal.GetLuaState();
				typedef typename Details::PushAnalog<T>::Result Analog;
				Analog analog = Details::ToLuaAnalog<Analog>::Convert(val);
				lua_checkstack(L, 1);
				Details::Push(L, analog);
				lua_setfield(L, -2, szField);
				lua_pop(L, 1); // Pop the table
			}
		}

		/// inline
		template<class T>
		void SetField(int iIndex, T val) {
			if( Push() ) {
				lua_State *L = m_kLuaVal.GetLuaState();
				typedef typename Details::PushAnalog<T>::Result Analog;
				Analog analog = Details::ToLuaAnalog<Analog>::Convert(val);
				lua_checkstack(L, 1);
				Details::Push(L, analog);
				lua_rawseti(L, -2, iIndex);
				lua_pop(L, 1); // Pop the table
			}
		}

		/// inline
		template<class T>
		void SetField(const Value &kKey, T val) {
			lua_State *L = m_kLuaVal.GetLuaState();
			if( L != kKey.GetLuaState() ) {
				Details::Error("Key sent to FLua::Table::SetField belongs to a different lua state");
			}
			else if( Push() ) {
				if( kKey.Push() ) {
					if( lua_isnil(L, -1) )
					{
						lua_pop(L, 1); // Pop the nil key
						Details::Error("nil key sent to FLua::Table::SetField");
					}
					else
					{
						typedef typename Details::PushAnalog<T>::Result Analog;
						Analog analog = Details::ToLuaAnalog<Analog>::Convert(val);
						lua_checkstack(L, 1);
						Details::Push(L, analog); // Push the value
						lua_rawset(L, -3); // Set the field (pops key and value)
					}
				}
				else Details::Error("Invalid key sent to FLua::Table::SetField");
				lua_pop(L, 1); // Pop the table
			}
		}

		// SetFieldToNil - nil out a field
		/// 0x08ddb46c
		void SetFieldToNil( _In_z_ const char *szField);
		/// 0x08ddb4d8
		void SetFieldToNil(int iIndex);

        /// inline
        template<class TFunc>
        inline void AddMemberFunction( _In_z_ const char *szName, TFunc pfn)
        {
            SetField(szName, FLua::Value::FromCFunction(GetLuaState(), pfn, szName));
        }

		/// inline
		template<class TIterator>
		void Insert(const TIterator &first, const TIterator &last)
		{
			for( TIterator itr = first; itr != last; ++itr )
				(*this)[itr->first] = itr->second;
		}

		/// size 0x14
		class iterator
		{
			friend Table;

			/// size 0x10
			struct Pos
			{
				// Key
				Value first; ///< +0x0
				// Value
				Value second; ///< +0x8
			};

		public:
			/// 0x08ddb544
			iterator();
			/// 0x08ddb56c
			iterator(const iterator &kRhs);

			/// 0x08ddb5ca
			const iterator &operator =(const iterator &kRhs);

			/// 0x08ddb616
			const bool operator ==(const iterator &kRhs) const;
			/// inline
			inline const bool operator !=(const iterator &kRhs) const { return !(operator ==(kRhs)); }

			/// inline
			inline const iterator &operator++() { Advance(); return *this; }
			/// inline
			inline iterator operator++(int) { iterator kTemp(*this); Advance(); return kTemp; }

			/// inline
			inline const Pos *operator *() const { return &m_kPos; }
			/// inline
			inline const Pos *operator ->() const { return &m_kPos; }

		private:
			/// 0x08ddb678
			void Advance();

			const Table *m_pkTable; ///< +0x0
			Pos m_kPos; ///< +0x4
		};

		/// 0x08ddb748
		iterator begin() const;
		/// 0x08ddb834
		iterator end() const;

	private:
		Value m_kLuaVal; ///< +0x0
	};

	/// size 0x8
	class Metatable : public Table
	{
	public:
		/// 0x08ddb8a4
		Metatable(const Value &kLuaValue); // Get the metatable for a FLua::Value
		/// 0x08ddb94a
		Metatable(lua_State *L, int iStackIndex); // Get the metatable for the value at iStackIndex 
		/// 0x08ddb9d6
		Metatable(lua_State *L, _In_z_ const char *szName); // Get the metatable for a global
	};

	/// size 0x8
	class Function
	{
	public:
		/// inline
		Function() : m_kLuaVal() {}
		/// inline
		Function(const Function &kRhs) : m_kLuaVal(kRhs.m_kLuaVal) {}
		/// inline
		Function(const Value &kLuaVal) : m_kLuaVal(kLuaVal) {}
		/// inline
		Function(const StackValue &kStackValue) : m_kLuaVal(kStackValue) {}
		/// inline
		Function(lua_State *L, int iStackIndex) : m_kLuaVal(L, iStackIndex) {}
		/// inline
		Function(lua_State *L, _In_z_ const char *szName) : m_kLuaVal(L, szName) {}
		/// inline
		~Function() {}

		/// 0x08ddba8e
		static Function Create(lua_State *L, _In_z_ const char *szCode);

		/// inline
		inline void Swap(Function &kRhs) { m_kLuaVal.Swap(kRhs.m_kLuaVal); }

		/// inline
		inline const Function &operator =(const Function &kRhs) { m_kLuaVal = kRhs.m_kLuaVal; return *this; }
		/// inline
		inline const Function &operator =(const Value &kLuaValue) { m_kLuaVal = kLuaValue; return *this; }
		/// inline
		inline void Set(lua_State *L, int iStackIndex) { m_kLuaVal.Set(L, iStackIndex); }
		/// inline
		inline void Set(lua_State *L, _In_z_ const char *szName) { m_kLuaVal.Set(L, szName); }

		/// inline
		inline void Clear() { m_kLuaVal.Clear(); }

		// Warning: Only call if the lua state has already been destroyed
		/// inline
		inline void LuaStateDestroyed() { m_kLuaVal.LuaStateDestroyed(); }

		/// 0x08ddbcba
		bool Valid() const;

		/// inline
		inline const bool operator ==(const Function &kRhs) const { return m_kLuaVal == kRhs.m_kLuaVal; }
		/// inline
		inline const bool operator !=(const Function &kRhs) const { return !(m_kLuaVal == kRhs.m_kLuaVal); }
		/// inline
		inline const bool operator ==(const Value &kRhs) const { return m_kLuaVal == kRhs; }
		/// inline
		inline const bool operator !=(const Value &kRhs) const { return !(m_kLuaVal == kRhs); }

		/// 0x08ddbd16
		bool Push() const;

		/// inline
		inline const Value &GetLuaValue() const { return m_kLuaVal; }
		/// inline
		inline lua_State *GetLuaState() const { return m_kLuaVal.GetLuaState(); }

		// WARNING: LuaValue owns this lua ref!
		/// inline
		inline int GetRef() const { return m_kLuaVal.GetRef(); }

		// 0 args
		/// inline
		Value operator()() const {
			Value kRetVal;
			Details::LockAccess();
			if( Push() ) {
				lua_State *L = GetLuaState();
				if( Details::CallWithErrorHandling(L, 0, 1) ) {
					kRetVal.Set(L, -1); // Get the return value
					lua_pop(L, 1); // Clean up the stack
				}
			}
			else Details::Error(_T("Attempt to call a FLua::Function with an invalid function"));
			Details::UnlockAccess();
			return kRetVal;
		}

		/// inline
		template<class TRet>
		TRet CallAsDelegate() const { return (TRet)operator()(); }

		// 1 arg
		/// inline
		template <class TArg0>
		Value operator()(TArg0 a0) const {
			Value kRetVal;
			Details::LockAccess();
			if( Push() ) {
				typedef typename Details::PushAnalog<TArg0>::Result Analog0;
				Analog0 arg0 = Details::ToLuaAnalog<Analog0>::Convert(a0);

				lua_State *L = GetLuaState();
				lua_checkstack(L, 1);
				Details::Push(L, arg0);

				if( Details::CallWithErrorHandling(L, 1, 1) ) {
					kRetVal.Set(L, -1);
					lua_pop(L, 1);
				}
			}
			else Details::Error(_T("Attempt to call a FLua::Function with an invalid function"));
			Details::UnlockAccess();
			return kRetVal;
		}

		/// inline
		template<class TRet, class TArg0>
		TRet CallAsDelegate(TArg0 a0) const { return (TRet)operator()<TArg0>(a0); }

		// 2 args
		/// inline
		template <class TArg0, class TArg1>
		Value operator()(TArg0 a0, TArg1 a1) const {
			Value kRetVal;
			Details::LockAccess();
			if( Push() ) {
				typedef typename Details::PushAnalog<TArg0>::Result Analog0;
				typedef typename Details::PushAnalog<TArg1>::Result Analog1;

				Analog0 arg0 = Details::ToLuaAnalog<Analog0>::Convert(a0);
				Analog1 arg1 = Details::ToLuaAnalog<Analog1>::Convert(a1);

				lua_State *L = GetLuaState();
				lua_checkstack(L, 2);
				Details::Push(L, arg0);
				Details::Push(L, arg1);

				if( Details::CallWithErrorHandling(L, 2, 1) ) {
					kRetVal.Set(L, -1);
					lua_pop(L, 1);
				}
			}
			else Details::Error(_T("Attempt to call a FLua::Function with an invalid function"));
			Details::UnlockAccess();
			return kRetVal;
		}

		/// inline
		template<class TRet, class TArg0, class TArg1>
		TRet CallAsDelegate(TArg0 a0, TArg1 a1) const { return (TRet)operator()<TArg0, TArg1>(a0, a1); }

		// 3 args
		/// inline
		template <class TArg0, class TArg1, class TArg2>
		Value operator()(TArg0 a0, TArg1 a1, TArg2 a2) const {
			Value kRetVal;
			Details::LockAccess();
			if( Push() ) {
				typedef typename Details::PushAnalog<TArg0>::Result Analog0;
				typedef typename Details::PushAnalog<TArg1>::Result Analog1;
				typedef typename Details::PushAnalog<TArg2>::Result Analog2;

				Analog0 arg0 = Details::ToLuaAnalog<Analog0>::Convert(a0);
				Analog1 arg1 = Details::ToLuaAnalog<Analog1>::Convert(a1);
				Analog2 arg2 = Details::ToLuaAnalog<Analog2>::Convert(a2);

				lua_State *L = GetLuaState();
				lua_checkstack(L, 3);
				Details::Push(L, arg0);
				Details::Push(L, arg1);
				Details::Push(L, arg2);

				if( Details::CallWithErrorHandling(L, 3, 1) ) {
					kRetVal.Set(L, -1);
					lua_pop(L, 1);
				}
			}
			else Details::Error(_T("Attempt to call a FLua::Function with an invalid function"));
			Details::UnlockAccess();
			return kRetVal;
		}

		/// inline
		template<class TRet, class TArg0, class TArg1, class TArg2>
		TRet CallAsDelegate(TArg0 a0, TArg1 a1, TArg2 a2) const { return (TRet)operator()<TArg0, TArg1, TArg2>(a0, a1, a2); }

		// 4 args
		/// inline
		template <class TArg0, class TArg1, class TArg2, class TArg3>
		Value operator()(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3) const {
			Value kRetVal;
			Details::LockAccess();
			if( Push() ) {
				typedef typename Details::PushAnalog<TArg0>::Result Analog0;
				typedef typename Details::PushAnalog<TArg1>::Result Analog1;
				typedef typename Details::PushAnalog<TArg2>::Result Analog2;
				typedef typename Details::PushAnalog<TArg3>::Result Analog3;

				Analog0 arg0 = Details::ToLuaAnalog<Analog0>::Convert(a0);
				Analog1 arg1 = Details::ToLuaAnalog<Analog1>::Convert(a1);
				Analog2 arg2 = Details::ToLuaAnalog<Analog2>::Convert(a2);
				Analog3 arg3 = Details::ToLuaAnalog<Analog3>::Convert(a3);

				lua_State *L = GetLuaState();
				lua_checkstack(L, 4);
				Details::Push(L, arg0);
				Details::Push(L, arg1);
				Details::Push(L, arg2);
				Details::Push(L, arg3);

				if( Details::CallWithErrorHandling(L, 4, 1) ) {
					kRetVal.Set(L, -1);
					lua_pop(L, 1);
				}
			}
			else Details::Error(_T("Attempt to call a FLua::Function with an invalid function"));
			Details::UnlockAccess();
			return kRetVal;
		}

		/// inline
		template<class TRet, class TArg0, class TArg1, class TArg2, class TArg3>
		TRet CallAsDelegate(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3) const {
			return (TRet)operator()<TArg0, TArg1, TArg2, TArg3>(a0, a1, a2, a3);
		}

		// 5 args
		/// inline
		template <class TArg0, class TArg1, class TArg2, class TArg3, class TArg4>
		Value operator()(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3, TArg4 a4) const {
			Value kRetVal;
			Details::LockAccess();
			if( Push() ) {
				typedef typename Details::PushAnalog<TArg0>::Result Analog0;
				typedef typename Details::PushAnalog<TArg1>::Result Analog1;
				typedef typename Details::PushAnalog<TArg2>::Result Analog2;
				typedef typename Details::PushAnalog<TArg3>::Result Analog3;
				typedef typename Details::PushAnalog<TArg4>::Result Analog4;

				Analog0 arg0 = Details::ToLuaAnalog<Analog0>::Convert(a0);
				Analog1 arg1 = Details::ToLuaAnalog<Analog1>::Convert(a1);
				Analog2 arg2 = Details::ToLuaAnalog<Analog2>::Convert(a2);
				Analog3 arg3 = Details::ToLuaAnalog<Analog3>::Convert(a3);
				Analog4 arg4 = Details::ToLuaAnalog<Analog4>::Convert(a4);

				lua_State *L = GetLuaState();
				lua_checkstack(L, 5);
				Details::Push(L, arg0);
				Details::Push(L, arg1);
				Details::Push(L, arg2);
				Details::Push(L, arg3);
				Details::Push(L, arg4);

				if( Details::CallWithErrorHandling(L, 5, 1) ) {
					kRetVal.Set(L, -1);
					lua_pop(L, 1);
				}
			}
			else Details::Error(_T("Attempt to call a FLua::Function with an invalid function"));
			Details::UnlockAccess();
			return kRetVal;
		}

		/// inline
		template<class TRet, class TArg0, class TArg1, class TArg2, class TArg3, class TArg4>
		TRet CallAsDelegate(TArg0 a0, TArg1 a1, TArg2 a2, TArg3 a3, TArg4 a4) const {
			return (TRet)operator()<TArg0, TArg1, TArg2, TArg3, TArg4>(a0, a1, a2, a3, a4);
		}

	private:
		Value m_kLuaVal; ///< +0x0
	};

	// Stack manipulation for lua types
	namespace Details
	{
		// For Values
		/// size 0x1
		template<> struct LuaAnalog<Value>{ typedef StackValue Result; };
		/// size 0x1
		template<> struct LuaAnalog<Value&>{ typedef Value Result; };
		/// size 0x1
		template<> struct LuaAnalog<const Value&>{ typedef StackValue Result; };

		/// size 0x1
		template<> struct PushAnalog<Value>{ typedef Value Result; };
		/// size 0x1
		template<> struct PushAnalog<const Value>{ typedef Value Result; };
		/// size 0x1
		template<> struct PushAnalog<Value&>{ typedef Value Result; };
		/// size 0x1
		template<> struct PushAnalog<const Value&>{ typedef Value Result; };

		/// inline
		template<> static const char *DescribeType<Value>() { return "variant"; }

		/// inline
		template<> static inline Value Get(lua_State *L, int idx) { return Value(L, idx); }
		/// inline
		static inline void Push(lua_State *L, const Value &kVal) {
			lua_State *pkValState = kVal.GetLuaState();
			if( pkValState == NULL )
			{
				lua_pushnil(L);
			}
			else if( pkValState != L )
			{
				if( SharedLuaRegistry::Get() )
				{
					lua_checkstack(L, 1);
					int iRef = kVal.GetRef();
					if( iRef == LUA_NOREF )
						lua_pushnil(L);
					else
						lua_getref(L, iRef);
				}
				else
				{
					Error(_T("Can't push lua value.  It belongs to a dfferent lua state"));
					lua_checkstack(L, 1);
					lua_pushnil(L);
				}
			}
			else if( !kVal.Push() )
			{
				lua_checkstack(L, 1);
				lua_pushnil(L);
			}
		}

		// For Strings
		/// size 0x1
		template<> struct LuaAnalog<String>{ typedef StackValue Result; };
		/// size 0x1
		template<> struct LuaAnalog<String&>{ typedef String Result; };
		/// size 0x1
		template<> struct LuaAnalog<const String&>{ typedef StackValue Result; };

		/// size 0x1
		template<> struct PushAnalog<String>{ typedef String Result; };
		/// size 0x1
		template<> struct PushAnalog<const String>{ typedef String Result; };
		/// size 0x1
		template<> struct PushAnalog<String&>{ typedef String Result; };
		/// size 0x1
		template<> struct PushAnalog<const String&>{ typedef String Result; };

		/// inline
		template<> static const char *DescribeType<String>() { return "string"; }

		/// inline
		template<> static inline String Get(lua_State *L, int idx) { return String(L, idx); }
		/// inline
		static inline void Push(lua_State *L, const String &sVal) {
			lua_State *pkValState = sVal.GetLuaState();
			if( pkValState != L )
			{
				lua_checkstack(L, 1);
				lua_pushstring(L, sVal);
			}
			else if( !sVal.Push() )
			{
				lua_checkstack(L, 1);
				lua_pushstring(L, "");
			}
		}

		// For Functions
		/// size 0x1
		template<> struct LuaAnalog<Function>{ typedef StackValue Result; };
		/// size 0x1
		template<> struct LuaAnalog<Function&>{ typedef Function Result; };
		/// size 0x1
		template<> struct LuaAnalog<const Function&>{ typedef StackValue Result; };

		/// size 0x1
		template<> struct PushAnalog<Function>{ typedef Function Result; };
		/// size 0x1
		template<> struct PushAnalog<const Function>{ typedef Function Result; };
		/// size 0x1
		template<> struct PushAnalog<Function&>{ typedef Function Result; };
		/// size 0x1
		template<> struct PushAnalog<const Function&>{ typedef Function Result; };

		/// inline
		template<> static const char *DescribeType<Function>() { return "function"; }

		/// inline
		template<> static inline Function Get(lua_State *L, int idx) { return Function(L, idx); }
		/// inline
		static inline void Push(lua_State *L, const Function &kVal) {
			if( kVal.GetLuaState() != L )
			{
				if( SharedLuaRegistry::Get() )
				{
					lua_checkstack(L, 1);
					int iRef = kVal.GetRef();
					if( iRef == LUA_NOREF )
						lua_pushnil(L);
					else
						lua_getref(L, iRef);
				}
				else
				{
					Error(_T("Can't push function.  It belongs to a dfferent lua state"));
					lua_checkstack(L, 1);
					lua_pushnil(L);
				}
			}
			else if( !kVal.Push() )
			{
				lua_checkstack(L, 1);
				lua_pushnil(L);
			}
		}

		/// size 0x1
		template<>
		struct ArgValidator<Function>{
			/// inline
			static inline void Validate(StackValue &arg, bool &bValid) {
				if( arg.GetType() != TYPE_FUNCTION ) {
					// TODO: Figure out what to do about potentially needing to convert typename string to wide char
					Error(_T("Bad argument: Expected function but got %s"), arg.GetTypeName());
					bValid = false;
				}
			}
		};

		/// size 0x1
		template<> struct ArgValidator<const Function&> : public ArgValidator<Function> {};

		/// size 0x1
		template<>
		struct ArgValidator<Function&>{
			/// inline
			static inline void Validate(Function &arg, bool &bValid) {
				if( !arg.Valid() ) {
					// TODO: Figure out what to do about potentially needing to convert typename string to wide char
					Error(_T("Bad argument: Expected function but got %s"), arg.GetLuaValue().GetTypeName());
					bValid = false;
				}
			}
		};

		// For Tables
		/// size 0x1
		template<> struct LuaAnalog<Table>{ typedef StackValue Result; };
		/// size 0x1
		template<> struct LuaAnalog<Table&>{ typedef Table Result; };
		/// size 0x1
		template<> struct LuaAnalog<const Table&>{ typedef StackValue Result; };

		/// size 0x1
		template<> struct PushAnalog<Table>{ typedef Table Result; };
		/// size 0x1
		template<> struct PushAnalog<const Table>{ typedef Table Result; };
		/// size 0x1
		template<> struct PushAnalog<Table&>{ typedef Table Result; };
		/// size 0x1
		template<> struct PushAnalog<const Table&>{ typedef Table Result; };

		/// inline
		template<> static const char *DescribeType<Table>() { return "table"; }

		/// inline
		static inline void Push(lua_State *L, const Table &kVal) {
			lua_State *pkValState = kVal.GetLuaState();
			if( pkValState == NULL )
			{
				lua_checkstack(L, 1);
				lua_pushnil(L);
			}
			else if( pkValState != L )
			{
				if( SharedLuaRegistry::Get() )
				{
					lua_checkstack(L, 1);
					int iRef = kVal.GetRef();
					if( iRef == LUA_NOREF )
						lua_pushnil(L);
					else
						lua_getref(L, iRef);
				}
				else
				{
					Error(_T("Can't push table.  It belongs to a dfferent lua state"));
					lua_checkstack(L, 1);
					lua_pushnil(L);
				}
			}
			else if( !kVal.Push() )
			{
				lua_checkstack(L, 1);
				lua_pushnil(L);
			}
		}

		// For Table Fields
		/// size 0x1
		template<> struct PushAnalog<Table::Field>{ typedef Value Result; };
		/// size 0x1
		template<> struct PushAnalog<const Table::Field>{ typedef Value Result; };
		/// size 0x1
		template<> struct PushAnalog<Table::Field&>{ typedef Value Result; };
		/// size 0x1
		template<> struct PushAnalog<const Table::Field&>{ typedef Value Result; };
	}
}

#endif
