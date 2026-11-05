/// origin binary
//------------------------------------------------------------------------------------------------
//
//  ***************** FIRAXIS GAME ENGINE   ********************
//
//! \file		FLuaCommon.h
//! \author		Eric Jordan -- 3/09/2009
//! \brief		Contains some internal helper code for FLua.  Rarely included directly.
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef FLuaCommon_h
#define FLuaCommon_h
#pragma once

#include <lua.hpp>

#include <Windows.h>
#if defined(_MSC_VER)
#include <tchar.h>
#endif
#include <stdio.h>
#include <assert.h>

class FCriticalSection;

// Kind of like an assert for the compiler.  If the condition is not true then a compilation error is caused.
#define FLUA_COMPILE_TIME_CONDITION(CONDITION, ERR_NAME) typedef int ERROR_##ERR_NAME[(CONDITION)? 1 : -1]

// Forces a compile time error.  Useful for template specializations that aren't suppose to compile.
#define FLUA_COMPILE_TIME_ERROR(ERR_NAME) FLUA_COMPILE_TIME_CONDITION(sizeof(TFunc) == 0, ERR_NAME)

#define FLUA_MULTITHREAD_SUPPORT

namespace FLua
{
	// Enum for types in lua
	enum Type
	{
		TYPE_NONE = LUA_TNONE,
		TYPE_NIL = LUA_TNIL,
		TYPE_NUMBER = LUA_TNUMBER,
		TYPE_BOOLEAN = LUA_TBOOLEAN,
		TYPE_STRING = LUA_TSTRING,
		TYPE_TABLE = LUA_TTABLE,
		TYPE_FUNCTION = LUA_TFUNCTION,
		TYPE_USERDATA = LUA_TUSERDATA,
		TYPE_LIGHTUSERDATA = LUA_TLIGHTUSERDATA,
		TYPE_THREAD = LUA_TTHREAD,
	};

	namespace Details
	{
		// Used by FLua::ErrorHandler to trick the compiler
		/// inline
		template<class TOut, class TIn>
		TOut UnsafeCast(TIn inVal)
		{
			FLUA_COMPILE_TIME_CONDITION(sizeof(TIn) <= sizeof(TOut), CantUseUnsafeCast);
			union { TIn inVal; TOut outVal; } u;
			u.outVal = TOut();
			u.inVal = inVal;
			return u.outVal;
		}

		/// 0x08dd979b
		void LockAccess();
		/// 0x08dd97be
		void UnlockAccess();	
	}

	// ErrorHandler - Called when FLua encounters an error.  Use SetErrorHandler to set it to your own function.
	/// size 0xc
	class ErrorHandler
	{
	public:
		typedef void(*ErrorHandlerStaticFunc)(const TCHAR*);

		// Static functions
		/// inline
		ErrorHandler(ErrorHandlerStaticFunc pfn) :
			m_pkClass(NULL),
			m_pfnFunc(Details::UnsafeCast<ErrorHandlerFunc>(pfn))
			{ assert(pfn); }
		
		// Member functions
		/// inline
		template<class T, class TFuncOwner>
		ErrorHandler(T *p, void(TFuncOwner::*pfn)(const TCHAR*)) :
			m_pkClass(Details::UnsafeCast<ErrorHandlerClass*>(p)),
			m_pfnFunc(Details::UnsafeCast<ErrorHandlerFunc>(pfn))
			{ assert(pfn); }

		// Const Member functions
		/// inline
		template<class T, class TFuncOwner>
		ErrorHandler(const T *p, void(TFuncOwner::*pfn)(const TCHAR*)const) :
			m_pkClass(Details::UnsafeCast<ErrorHandlerClass*>(p)),
			m_pfnFunc(Details::UnsafeCast<ErrorHandlerFunc>(pfn))
			{ assert(pfn); }

		// Call the error handler
		/// 0x08dd7a54
		void operator()( _In_z_ const TCHAR *szError) const;

	private:
		/// size 0x1
		class ErrorHandlerClass {};
		typedef void(ErrorHandlerClass::*ErrorHandlerFunc)(const TCHAR*);
		ErrorHandlerClass *m_pkClass; ///< +0x0
		ErrorHandlerFunc m_pfnFunc; ///< +0x4
	};

	namespace Details {
		/// inline
		static void DefaultErrorHandler(const TCHAR *szError) { fprintf_s(stderr, _T("%s\n"), szError); }
		/// size 0x1
		/// inline
		struct ErrorHandlerHolder { static inline ErrorHandler &Get() { static ErrorHandler kHandler = &DefaultErrorHandler; return kHandler; } };
	}

	// SetErrorHandler - Determines which function FLua will call in the case of an error
	/// inline
	static void SetErrorHandler(ErrorHandler kHandler) { Details::ErrorHandlerHolder::Get() = kHandler; }
	/// inline
	static ErrorHandler GetErrorHandler() { return Details::ErrorHandlerHolder::Get(); }

	// ScopedErrorHandler - Assign a special error handler for the current scope.  Previous error handler is restored in dtor.
	/// size 0xc
	class ScopedErrorHandler
	{
	public:
		/// 0x08dd7a9c
		explicit ScopedErrorHandler(ErrorHandler kErrorHandler);
		/// 0x08dd7bbc
		~ScopedErrorHandler();

	private:
		ErrorHandler m_kPrevErrorHandler; ///< +0x0
	};

	// CreateLuaState - Creates a new lua state and loads all standard lua libraries.
	/// 0x08dd7cf0
	_Ret_opt_ lua_State *CreateLuaState();

	// CreateLuaState - Creates a new lua state with the given allocator and loads all standard lua libraries.
	/// 0x08dd7d56
	_Ret_opt_ lua_State *CreateLuaState(lua_Alloc pfnAllocFn, void *pvUserData);

	// Execute - Executes code from a string in the given lua state.  Returns true if the code compiled and executed
	//    without error.  Provides error handling otherwise.  Does not leave chunk or error messages on the stack.
	/// 0x08dd7dcd
	bool Execute(lua_State *L, _In_z_ const char *szCode);

	// LoadFile - Loads a lua file into the given lua state.  Returns true if the code compiled and executed
	//   without error.  Provides error handling otherwise.  Does not leave chunk or error messages on the stack.
	/// 0x08dd816d
	bool LoadFile(lua_State *L, _In_z_ const wchar_t *wszFilename);

	// GetField - Acts like lua_getfield but can handle nested tables with '.'s separating the field names.
	/// 0x08dd83ca
	void GetField(lua_State *L, int iTableIndex, const char *szName);

	// GetGlobal - Acts like lua_getglobal but can handle nested tables with '.'s separating the field names.
	/// inline
	static inline void GetGlobal(lua_State *L, _In_z_ const char *szName) { GetField(L, LUA_GLOBALSINDEX, szName); }

	// LuaStackToString - Helpful for debugging.  Prints the lua stack to a string buffer.
	/// 0x08dd866c
	void LuaStackToString(lua_State *L, char *szBuffer, size_t uiBufferSize);

	// ConsoleCommand - Execute a command specifically designed for a lua command console.  Takes in text
	//   to execute as code and then writes output to output buffer.  Errors during ConsoleCommand are
	//   sent to szOutput rather than the regular ErrorHandler for FLua.  Returns true if szInput executed without error.
	/// 0x08dd87c2
	bool ConsoleCommand(lua_State *L, _In_z_ const char *szInput, char *szOutput, size_t uiOutputSize);

	// FunctionDescription - Represents the description of a function
	/// size 0x8
	struct FunctionDescription
	{
		/// inline
		FunctionDescription(const char *szFunction, const char *szDescription) :
			szFunction(szFunction), szDescription(szDescription) {}
		/// inline
		FunctionDescription() : szFunction(NULL), szDescription(NULL) {}
		const char *szFunction; ///< +0x0
		const char *szDescription; ///< +0x4
	};

	// SetFunctionDescription - Set the description for a function specified by name
	/// 0x08dd8ba6
	bool SetFunctionDescription(lua_State *L, _In_z_ const char *szFunction, _In_z_ const char *szDescription);

	// SetFunctionDescriptions - Set the descriptions for a set of functions specified by name.
	//   A FunctionDescription with a NULL szFunction pointer signals the end of the akDescriptions array.
	/// 0x08dd8e07
	bool SetFunctionDescriptions(lua_State *L, FunctionDescription *akDescriptions);

	namespace Details
	{
		template<class T> struct LuaAnalog;
		template<class T> struct ArgValidator;
		template<class T> struct FromLuaAnalog;
		/// inline
		template<class T> static bool CompatibleType(lua_State *L, int idx);
		/// inline
		template<class T> static inline T Get(lua_State *L, int idx);
	}

	// StackValue - Represents a generic argument on the lua stack.
	/// size 0x8
	class StackValue
	{
	public:
		/// 0x08dd8ece
		StackValue(lua_State *L, int iStackIndex);

		/// inline
		inline void Push() const { lua_pushvalue(m_pkLuaState, m_iStackIndex); }
		/// inline
		inline int Reference() const {
			lua_checkstack(m_pkLuaState, 1);
			lua_pushvalue(m_pkLuaState, m_iStackIndex); // Push the value
			return lua_ref(m_pkLuaState, true); // Ref then pop the value
		}

		/// inline
		inline Type GetType() const { return (Type)lua_type(m_pkLuaState, m_iStackIndex); }
		/// inline
		inline const char *GetTypeName() const { return lua_typename(m_pkLuaState, lua_type(m_pkLuaState, m_iStackIndex)); }

		/// inline
		inline bool IsNil() const { return lua_isnil(m_pkLuaState, m_iStackIndex); }

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

			// Get the lua analog for this type off of the lua stack
			typedef typename Details::LuaAnalog<T>::Result Analog;
			Analog analog = Details::Get<Analog>(m_pkLuaState, m_iStackIndex);

			// Validate the value from lua
			bool bValid(true); Details::ArgValidator<T>::Validate(analog, bValid);

			// Convert and return the value if it is valid
			if( bValid ) ret = Details::FromLuaAnalog<T>::Convert(analog);

			return ret;
		}

		// IsOfType - Determines if the value may be cast to a pointer of type T
		/// inline
		template<class T>
		bool IsOfType() const { return Details::CompatibleType<T>(m_pkLuaState, m_iStackIndex); }

		/// inline
		inline int GetIndex() const { return m_iStackIndex; }
		/// inline
		inline lua_State *GetLuaState() const { return m_pkLuaState; }

	private:
		lua_State *m_pkLuaState; ///< +0x0
		int m_iStackIndex; ///< +0x4
	};

	namespace Details
	{
		// Error - Call for internal errors in FLua.  Uses ErrorHandler
		/// 0x08dd7c43
		void Error( _In_z_ const TCHAR *szFormat, ...);
		/// 0x08dd8f0b
		void Error( lua_State *L, _In_z_ const TCHAR *szFormat, ...);
		/// 0x08dd967f
		const TCHAR *SaveCallStack(lua_State *L);
		/// 0x08dd9788
		const TCHAR* GetLastCallstackBuffer();

		// CallWithErrorHandling - Call a lua function with error handling automatically provided.
		//   Expects that the function and then the arguments are already on the stack (in that order).
		//   uiArgCount provides the number of arguments.
		//   uiResults controls the exact number of return values from lua that will be pushed onto the stack.
		//   If the call succeeds with no errors then the function returns true and uiResults are pushed onto the stack.
		//   If the call fails then an error is reported with FLua::Details::Error and nothing is left on the stack.
		//   Note: LUA_MULTRET (a uiResults value of -1) is not supported by this function.  Use MultRetCallWithErrorHandling for LUA_MULTRET.
		/// 0x08dd7edf
		bool CallWithErrorHandling(lua_State *L, unsigned int uiArgCount, unsigned int uiResults);

		// MultRetCallWithErrorHandling - Call a lua function with error handling automatically provided.
		//   Expects that the function and then the arguments are already on the stack (in that order).
		//   uiArgCount provides the number of arguments.
		//   uiReturnCountOut provides the number of return values pushed onto the stack after the call is done.
		//   If the call succeeds with no errors then the function returns true and uiReturnCountOut values are pushed onto the stack.
		//   If the call fails then an error is reported with FLua::Details::Error and nothing is left on the stack.
		/// 0x08dd9044
		bool MultRetCallWithErrorHandling(lua_State *L, unsigned int uiArgCount, unsigned int &uiReturnCountOut);

		// LuaNew - New an object using the allocator of the lua state L
		/// inline
		template<class T> static T *LuaNew(lua_State *L)
		{
			void *pvUserData = NULL;
			lua_Alloc pfnAlloc = lua_getallocf(L, &pvUserData);
			return new((*pfnAlloc)(pvUserData, NULL, 0, sizeof(T)))T();
		}

		// LuaDelete - Delete an object that has been allocated with LuaNew
		/// inline
		template<class T> void LuaDelete(lua_State *L, T *&p)
		{
			if( p )
			{
				p->~T();
				void *pvUserData = NULL;
				lua_Alloc pfnAlloc = lua_getallocf(L, &pvUserData);
				p = (T*)(*pfnAlloc)(pvUserData, p, sizeof(T), 0);
			}
		}

		// StripPtr - Use to rip pointer off of a type in a template
		/// size ?
		template<class T> struct StripPtr    { typedef T Result; };
		/// size ?
		template<class T> struct StripPtr<T*>{ typedef T Result; };

		// StripPtrAndRef - Use to rip pointer and reference (&) off of a type in a template
		/// size ?
		template<class T> struct StripPtrAndRef    { typedef T Result; };
		/// size ?
		template<class T> struct StripPtrAndRef<T*>{ typedef T Result; };
		/// size ?
		template<class T> struct StripPtrAndRef<T&>{ typedef T Result; };

		// StripConst - Use to rip const off of a type in a template
		/// size ?
		template<class T> struct StripConst          { typedef T Result; };
		/// size ?
		template<class T> struct StripConst<const T> { typedef T Result; };
		/// size ?
		template<class T> struct StripConst<const T*>{ typedef T* Result; };

		// ConstCheck - Use to check for const type in a template
		/// size ?
		template<class T> struct ConstCheck { static const bool bIsConst = false; };
		/// size ?
		template<class T> struct ConstCheck<const T> { static const bool bIsConst = true; };
		/// size ?
		template<class T> struct ConstCheck<const T*> { static const bool bIsConst = true; };

		// ConstCheck specializations for const member functions
		/// size ?
		template<class T, class TRet> struct ConstCheck<TRet(T::*)()const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0> struct ConstCheck<TRet(T::*)(TArg0)const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0, class TArg1>
		struct ConstCheck<TRet(T::*)(TArg0, TArg1)const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0, class TArg1, class TArg2>
		struct ConstCheck<TRet(T::*)(TArg0, TArg1, TArg2)const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0, class TArg1, class TArg2, class TArg3>
		struct ConstCheck<TRet(T::*)(TArg0, TArg1, TArg2, TArg3)const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0, class TArg1, class TArg2, class TArg3, class TArg4>
		struct ConstCheck<TRet(T::*)(TArg0, TArg1, TArg2, TArg3, TArg4)const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0, class TArg1, class TArg2, class TArg3, class TArg4, class TArg5>
		struct ConstCheck<TRet(T::*)(TArg0, TArg1, TArg2, TArg3, TArg4, TArg5)const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0, class TArg1, class TArg2, class TArg3, class TArg4, class TArg5, class TArg6>
		struct ConstCheck<TRet(T::*)(TArg0, TArg1, TArg2, TArg3, TArg4, TArg5, TArg6)const> { static const bool bIsConst = true; };
		/// size ?
		template<class T, class TRet, class TArg0, class TArg1, class TArg2, class TArg3, class TArg4, class TArg5, class TArg6, class TArg7>
		struct ConstCheck<TRet(T::*)(TArg0, TArg1, TArg2, TArg3, TArg4, TArg5, TArg6, TArg7)const> { static const bool bIsConst = true; };

		// Do a const check on a value
		/// inline
		template<class T> inline bool DoConstCheck(T data) { return ConstCheck<T>::bIsConst; };

		// CData - Used to associate C++ objects with lua tables.  Added to the table as userdata.
		/// size 0x4
		class CData {
		public:
			/// inline
			CData(void *pvObject) : m_pvObject(pvObject) {}
			/// inline
			inline void *GetPtr() const { return m_pvObject; }
			/// inline
			inline void NullPtr() { m_pvObject = NULL; }
		private:
			void *m_pvObject; ///< +0x0
		};

		// PushMetaTableFn - Pointer to a function that pushes a meta table onto the stack
		typedef void(*PushMetaTableFn)(lua_State *L);

		// MetaTableFnHolder - Allows each type to have it's own static PushMetaTableFn function pointer that is NULL until assigned.
		/// size ?
		template<class T> struct MetaTableFnHolder { static PushMetaTableFn pfn; };
		/// 0x0a3c98e8 FLua::Details::MetaTableFnHolder<ControlBase const>
		/// 0x0a3c98ec FLua::Details::MetaTableFnHolder<ControlBase>
		/// 0x0a3cb154 FLua::Details::MetaTableFnHolder<TextControl const>
		/// 0x0a3cb158 FLua::Details::MetaTableFnHolder<TextControl>
		/// 0x0a3cbfa0 FLua::Details::MetaTableFnHolder<LuaContext const>
		/// 0x0a3cbfa4 FLua::Details::MetaTableFnHolder<LuaContext>
		/// 0x0a3cbfa8 FLua::Details::MetaTableFnHolder<ToolTipManager>
		/// 0x0a3cbfac FLua::Details::MetaTableFnHolder<UIManager>
		/// 0x0a3d1168 FLua::Details::MetaTableFnHolder<GridControl>
		/// 0x0a3d16c8 FLua::Details::MetaTableFnHolder<ToolTipManager const>
		/// 0x0a3d1f70 FLua::Details::MetaTableFnHolder<UIManager const>
		/// 0x0a3e2ae4 FLua::Details::MetaTableFnHolder<LineControl>
		/// 0x0a3e3040 FLua::Details::MetaTableFnHolder<LineControl const>
		/// 0x0a3e3760 FLua::Details::MetaTableFnHolder<ReplayMapControl const>
		/// 0x0a3e3764 FLua::Details::MetaTableFnHolder<ReplayMapControl>
		/// 0x0c079458 FLua::Details::MetaTableFnHolder<ScrollAnim>
		/// 0x0c0799b8 FLua::Details::MetaTableFnHolder<ScrollAnim const>
		/// 0x0c0799bc FLua::Details::MetaTableFnHolder<AlphaAnim>
		/// 0x0c079f18 FLua::Details::MetaTableFnHolder<AlphaAnim const>
		/// 0x0c07a478 FLua::Details::MetaTableFnHolder<FlipAnim const>
		/// 0x0c07a47c FLua::Details::MetaTableFnHolder<FlipAnim>
		/// 0x0c07a480 FLua::Details::MetaTableFnHolder<AnimControl>
		/// 0x0c07a9e0 FLua::Details::MetaTableFnHolder<AnimControl const>
		/// 0x0c07af40 FLua::Details::MetaTableFnHolder<SlideAnim const>
		/// 0x0c07af44 FLua::Details::MetaTableFnHolder<SlideAnim>
		/// 0x0c07b10c FLua::Details::MetaTableFnHolder<BarControl>
		/// 0x0c07b668 FLua::Details::MetaTableFnHolder<BarControl const>
		/// 0x0c07bd88 FLua::Details::MetaTableFnHolder<ButtonControl const>
		/// 0x0c07bd8c FLua::Details::MetaTableFnHolder<ButtonControl>
		/// 0x0c07c2e8 FLua::Details::MetaTableFnHolder<ButtonControlBase const>
		/// 0x0c07c2ec FLua::Details::MetaTableFnHolder<ButtonControlBase>
		/// 0x0c07c65c FLua::Details::MetaTableFnHolder<TextButtonControl>
		/// 0x0c07c660 FLua::Details::MetaTableFnHolder<CheckBoxControl>
		/// 0x0c07cbc0 FLua::Details::MetaTableFnHolder<CheckBoxControl const>
		/// 0x0c07d2e0 FLua::Details::MetaTableFnHolder<ColorBoxButtonControl const>
		/// 0x0c07d2e4 FLua::Details::MetaTableFnHolder<ColorBoxButtonControl>
		/// 0x0c07d4ac FLua::Details::MetaTableFnHolder<ColorBoxControl>
		/// 0x0c07da08 FLua::Details::MetaTableFnHolder<ColorBoxControl const>
		/// 0x0c07dbd0 FLua::Details::MetaTableFnHolder<EditBoxControl const>
		/// 0x0c07dbd4 FLua::Details::MetaTableFnHolder<EditBoxControl>
		/// 0x0c07e850 FLua::Details::MetaTableFnHolder<GridButtonControl const>
		/// 0x0c07e854 FLua::Details::MetaTableFnHolder<GridButtonControl>
		/// 0x0c07ef80 FLua::Details::MetaTableFnHolder<GridControl const>
		/// 0x0c07f148 FLua::Details::MetaTableFnHolder<ImageControl>
		/// 0x0c07f6a8 FLua::Details::MetaTableFnHolder<ImageControl const>
		/// 0x0c07f870 FLua::Details::MetaTableFnHolder<MeterControl>
		/// 0x0c07fdd0 FLua::Details::MetaTableFnHolder<MeterControl const>
		/// 0x0c07ff98 FLua::Details::MetaTableFnHolder<StackControl>
		/// 0x0c07ff9c FLua::Details::MetaTableFnHolder<PullDownControl>
		/// 0x0c07ffa0 FLua::Details::MetaTableFnHolder<ScrollPanelControl>
		/// 0x0c080500 FLua::Details::MetaTableFnHolder<PullDownControl const>
		/// 0x0c080c20 FLua::Details::MetaTableFnHolder<RadioButtonControl const>
		/// 0x0c080c24 FLua::Details::MetaTableFnHolder<RadioButtonControl>
		/// 0x0c080df0 FLua::Details::MetaTableFnHolder<SliderControl>
		/// 0x0c081350 FLua::Details::MetaTableFnHolder<ScrollPanelControl const>
		/// 0x0c081a70 FLua::Details::MetaTableFnHolder<SliderControl const>
		/// 0x0c0821d8 FLua::Details::MetaTableFnHolder<StackControl const>
		/// 0x0c0828f8 FLua::Details::MetaTableFnHolder<TextButtonControl const>
		/// 0x0c082ac0 FLua::Details::MetaTableFnHolder<TextureBarControl>
		/// 0x0c083020 FLua::Details::MetaTableFnHolder<TextureBarControl const>
		/// 0x0c083400 FLua::Details::MetaTableFnHolder<WorldAnchorControl>
		/// 0x0c083960 FLua::Details::MetaTableFnHolder<WorldAnchorControl const>
		template<class T> PushMetaTableFn MetaTableFnHolder<T>::pfn = NULL;

		// GetPushMetaTableFn - Retrieves the PushMetaTableFn for type T
		/// inline
		template<class T> static inline PushMetaTableFn &GetPushMetaTableFn() { return MetaTableFnHolder<T>::pfn; }

		// LuaAnalog - Defines the type used for the lua stack
		/// size ?
		template<class T> struct LuaAnalog{ typedef T Result; };
		/// size ?
		template<class T> struct LuaAnalog<T&>{ typedef T* Result; };
		/// size ?
		template<class T> struct LuaAnalog<const T>{ typedef T Result; };
		/// size ?
		template<class T> struct LuaAnalog<const T*>{ typedef const T* Result; };
		/// size ?
		template<class T> struct LuaAnalog<const T&>{ typedef const T* Result; };
		/// size 0x1
		template<> struct LuaAnalog<char>{ typedef const char* Result; };
		/// size 0x1
		template<> struct LuaAnalog<const char>{ typedef const char* Result; };
		/// size 0x1
		template<> struct LuaAnalog<int>{ typedef lua_Integer Result; };
		/// size 0x1
		template<> struct LuaAnalog<const int>{ typedef lua_Integer Result; };
		/// size 0x1
		template<> struct LuaAnalog<unsigned int>{ typedef lua_Number Result; };
		/// size 0x1
		template<> struct LuaAnalog<const unsigned int>{ typedef lua_Number Result; };
		/// size 0x1
		template<> struct LuaAnalog<unsigned long>{ typedef lua_Number Result; };
		/// size 0x1
		template<> struct LuaAnalog<const unsigned long>{ typedef lua_Number Result; };
		/// size 0x1
		template<> struct LuaAnalog<double>{ typedef lua_Number Result; };
		/// size 0x1
		template<> struct LuaAnalog<const double>{ typedef lua_Number Result; };
		/// size 0x1
		template<> struct LuaAnalog<char*>{ typedef const char* Result; }; // char* ok for push but not get!
		/// size 0x1
		template<> struct LuaAnalog<const char*>{ typedef const char* Result; };

		// PushAnalog - Defines the type used for Push on the lua stack (usually the same as LuaAnalog)
		/// size ?
		template<class T> struct PushAnalog : LuaAnalog<T>{};
		/// size 0x1
		template<> struct PushAnalog<char>{ typedef char Result; };
		/// size 0x1
		template<> struct PushAnalog<const char>{ typedef const char Result; };

		// CompatibleType - Uses metatables like an RTTI system to determine type compatibility.  Do not use a negative index!!!
		/// inline
		template<class T>
		static bool CompatibleType(lua_State *L, int idx)
		{
			// This function does not support negative indexes at this time
			assert(idx > 0);

			// If T is a const type and the value at idx is a compatible non-const type then a cast is fine.
			if( ConstCheck<T>::bIsConst && CompatibleType<typename StripConst<T>::Result>(L, idx)) return true;

			// Compare metatables to determine type compatibility
			PushMetaTableFn pfnPushMetaTable = GetPushMetaTableFn<StripPtr<T>::Result>();
			if( pfnPushMetaTable )
			{
				(*pfnPushMetaTable)(L); // Push the metatable for T
				if( lua_getmetatable(L, idx) )
				{	// Compare the metatables for type equality
					bool bCompatibleTypes = true;
					while( lua_equal(L, -1, -2) == 0 )
					{
						lua_getfield(L, -1, "BaseType"); // Check for base type
						if( lua_istable(L, -1) ) 
						{
							lua_remove(L, -2); // Remove child metatable
						}
						else
						{	// No compatibility between these types
							lua_pop(L, 3); // Pop nil and both metatables
							return false;
						}
					}
					
					// The type of the value at idx or a parent of that type is equal to T
					lua_pop(L, 2); // Pop both metatables
					return true;
				}
				else
				{	// The value at idx does not have a metatable but T does.  This means they are incompatible.
					lua_pop(L, 1); // Pop the metatable for T.
					return false;
				}
			}
			else if( lua_getmetatable(L, idx) )
			{	// T does not have a metatable but the value at idx does.  This means they are incompatible.
				lua_pop(L, 1); // Pop the metatable for the value at idx.
				return false;
			}

			// If neither types have metatables then compatibility is assumed to be true.
			return true;
		}

		// GetTypeNameFromMetatable - Attempts to get the type name from the "CTypeName" field of the metatable of the value at idx.
		/// inline
		static const char *GetTypeNameFromMetatable(lua_State *L, int idx)
		{
			const char *szTypeName = NULL;
			if( lua_getmetatable(L, idx) != 0 )
			{
				lua_getfield(L, -1, "CTypeName");
				szTypeName = lua_tostring(L, -1);
				lua_pop(L, 2);
			}

			if( !szTypeName ) szTypeName = "Unkown_Type";

			return szTypeName;
		}

		// GetTypeNameFromMetatable - Attempts to get the type name from the "CTypeName" field of the metatable for T
		/// inline
		template<class T> static const char *GetTypeNameFromMetatable(lua_State *L)
		{
			const char *szTypeName = NULL;
			PushMetaTableFn pfnPushMetaTable = GetPushMetaTableFn<StripPtr<T>::Result>();
			if( pfnPushMetaTable )
			{
				(*pfnPushMetaTable)(L);
				lua_getfield(L, -1, "CTypeName");
				szTypeName = lua_tostring(L, -1);
				lua_pop(L, 2);
			}

			if( !szTypeName ) szTypeName = "Unkown_Type";

			return szTypeName;
		}

		// Get - Gets a value off of the lua stack.  Default implementation looks for a table with CData or light user data.
		/// inline
		template<class T>
		static inline T Get(lua_State *L, int idx)
		{
			T pkRet = NULL;
			if( lua_istable(L, idx) )
			{
				// If this is a compatible type then use it
				if( CompatibleType<T>(L, idx) )
				{
					lua_pushvalue(L, idx); // Push table
					lua_getfield(L, -1, "CData");
					CData kCData = (CData)lua_touserdata(L, -1);
					pkRet = (T)kCData.GetPtr();
					lua_pop(L, 2); // Pop table and CData field
				}
				else Error(_T("Cannot cast from %s to %s.  Using NULL instead."), GetTypeNameFromMetatable(L, idx), GetTypeNameFromMetatable<T>(L));
			}
			else if( lua_islightuserdata(L, idx) )
			{
				pkRet = (T)lua_touserdata(L, idx);
			}
			else
			{
				Error(_T("Incorrect type for pointer argument.  Using NULL instead."));
			}

			return pkRet;
		}

		// Get functions for primitive types
		/// inline
		template<> static inline bool Get(lua_State *L, int idx) { return lua_toboolean(L, idx) != 0; }
		/// inline
		template<> static inline lua_Integer Get(lua_State *L, int idx) { return lua_tointeger(L, idx); }
		/// inline
		template<> static inline long Get(lua_State *L, int idx) { return (long)lua_tointeger(L, idx); }
		/// inline
		template<> static inline lua_Number Get(lua_State *L, int idx) { return lua_tonumber(L, idx); }
		/// inline
		template<> static inline float Get(lua_State *L, int idx) { return (float)lua_tonumber(L, idx); }
		/// inline
		template<> static inline const char *Get(lua_State *L, int idx) { return lua_tostring(L, idx); }
		/// inline
		template<> static inline StackValue Get(lua_State *L, int idx) { return StackValue(L, idx); }

		// PushCData - Allows the creation of CData to be overriden for certain types
		/// inline
		template<class T> static void PushCData(lua_State *L, T *pVal)
		{
			// Create a new CData object as lua user data
			lua_pushlightuserdata(L, pVal);
		}

		// Push - Push a value onto the lua stack. Default implementation pushes a table with CData onto the stack.
		/// inline
		template<class T>
		static inline void Push(lua_State *L, T *pVal)
		{
			// Table representing this object
			lua_newtable(L);

			// Add the CData to the table
			PushCData(L, pVal);
			lua_setfield(L, -2, "CData");

			// Set the meta table
			PushMetaTableFn pfnPushMetaTable = GetPushMetaTableFn<T>();
			if( pfnPushMetaTable )
			{
				(*pfnPushMetaTable)(L);
				lua_setmetatable(L, -2);
			}
		}

		// Push functions for primitive types
		/// inline
		static inline void Push(lua_State *L, const bool bVal) { lua_pushboolean(L, bVal); }
		/// inline
		static inline void Push(lua_State *L, const char cVal) { char szVal[2]; szVal[0] = cVal; szVal[1] = '\0'; lua_pushstring(L, szVal); }
		/// inline
		static inline void Push(lua_State *L, const lua_Integer iVal) { lua_pushinteger(L, iVal); }
		/// inline
		static inline void Push(lua_State *L, const lua_Number fVal) { lua_pushnumber(L, fVal); }
		/// inline
		static inline void Push(lua_State *L, const float fVal) { lua_pushnumber(L, fVal); }
		/// inline
		static inline void Push(lua_State *L, _In_z_ const char *szVal) { lua_pushstring(L, szVal); }
		/// inline
		static inline void Push(lua_State *,  const StackValue kVal) { kVal.Push(); }

		// ArgValidator - Determines if an argument is valid.  Default case does nothing.
		/// size ?
		template<class T>
		struct ArgValidator {
			template<class TArgType> static inline void Validate(TArgType&, bool&){}
		};

		// ArgValidator for reference types
		/// size ?
		template<class T>
		struct ArgValidator<T&>{
			template<class TArgType> static inline void Validate(TArgType &arg, bool &bValid) {}
			/// inline
			static inline void Validate(T *&arg, bool &bValid) {
				if( arg == NULL ) {
					Error(_T("Bad argument: NULL reference"));
					bValid = false;
				}
			}
		};

		// ArgValidator for const reference types
		/// size ?
		template<class T> struct ArgValidator<const T&> : public ArgValidator<T&> {};

		// Congratulations!  You have made it deep into the bowls of FLua.  Turn Back!!! It's scary in here :-0

		// ArgValidator for uint
		/// size 0x1
		template<>
		struct ArgValidator<unsigned int>{
			/// inline
			static inline void Validate(lua_Number &iArg, bool &bValid) {
				if( iArg < 0 ) {
					Error(_T("Bad argument: Negative number passed for uint"));
					bValid = false;
				}
			}
		};

		// ArgValidator for const uint
		/// size 0x1
		template<> struct ArgValidator<const unsigned int> : ArgValidator<unsigned int> {};

		// ArgValidator for char
		/// size 0x1
		template<>
		struct ArgValidator<char>{
			/// inline
			static inline void Validate(const char *szArg, bool &bValid) {
				size_t uiLength = strlen(szArg);
				if( uiLength == 0 ) {
					Error(_T("Bad argument: Empty string representing character."));
					bValid = false;
				}
				else if( uiLength > 1 ) {
					Error(_T("Bad argument: \"%s\" passed to one character argument."), szArg);
					bValid = false;
				}
			}
		};

		// ArgValidator for const char
		/// size 0x1
		template<> struct ArgValidator<const char> : public ArgValidator<char> {};

		// FromLuaAnalog - Converts arguments to the type we would like to send.  Default case does nothing.
		/// size ?
		template<class T>
		struct FromLuaAnalog {
			template<class TLuaAnalog> static inline TLuaAnalog &Convert(TLuaAnalog &arg) { return arg; }
		};

		// FromLuaAnalog for reference types
		/// size ?
		template<class T>
		struct FromLuaAnalog<T&> {
			/// inline
			static inline T &Convert(T *&arg) { FAssertMsg(arg, "Forgot to check for NULL before dereferencing"); return *arg; }
			template<class TArgType> static inline TArgType &Convert(TArgType &arg) { return arg; }
		};

		// FromLuaAnalog for const reference types
		/// size ?
		template<class T> struct FromLuaAnalog<const T&> : public FromLuaAnalog<T&> {};

		// FromLuaAnalog for char
		/// size 0x1
		template<>
		struct FromLuaAnalog<char> {
			/// inline
			static inline char Convert( _In_z_ const char *szArg)
			{
				assert(szArg && *szArg && "Empty string");
				if( szArg )
					return szArg[0];
				else
					return 0;
			}
		};

		// FromLuaAnalog for const char
		/// size 0x1
		template<> struct FromLuaAnalog<const char> : public FromLuaAnalog<char> {};

		// FromLuaAnalog for unsigned int
		/// size 0x1
		template<>
		struct FromLuaAnalog<unsigned int> {
			/// inline
			static inline unsigned int Convert(const lua_Number fArg) { return (unsigned int)fArg; }
		};

		// FromLuaAnalog for const unsigned int
		/// size 0x1
		template<> struct FromLuaAnalog<const unsigned int> : public FromLuaAnalog<unsigned int> {};

		// ToLuaAnalog - Converts types to a type we can push onto the lua stack.  Default case does nothing.
		/// size ?
		template<class TLuaAnalog>
		struct ToLuaAnalog {
			template<class T> static inline T &Convert(T&arg) { return arg; }
		};

		// ToLuaAnalog for pointer types.  Converts references to pointers.
		/// size ?
		template<class TLuaAnalog>
		struct ToLuaAnalog<TLuaAnalog*> {
			/// inline
			static inline TLuaAnalog *Convert(TLuaAnalog &arg) { return &arg; }
			/// inline
			static inline const TLuaAnalog *Convert(const TLuaAnalog &arg) { return &arg; }
			template<class T> static inline T &Convert(T&arg) { return arg; }
		};

		// ToLuaAnalog for const pointer types.
		/// size ?
		template<class TLuaAnalog> struct ToLuaAnalog<const TLuaAnalog*> : public ToLuaAnalog<TLuaAnalog*> {};

		// ToLuaAnalog for const char*
		/// size 0x1
		template<>
		struct ToLuaAnalog<const char*> {
			/// inline
			static inline const char *Convert(char &cArg) { return &cArg; }
			/// inline
			template<class T> static inline T &Convert(T&arg) { return arg; }
		};

		// Call - Use this to call functions using a lua_State
		/// inline
		template<class TFunc>
		static int Call(TFunc pfn, lua_State *L, const TCHAR *szLuaFnName) {
			// Compile error if there is no specialization of Call for this function signature.
			// If you get a compile error here it means that you probably exposed a function
			// whose signature is not yet recognized by FLua.  You cannot expose this function
			// until a template specialization for its signature is added.
			FLUA_COMPILE_TIME_ERROR(CantDoCall);
		}

		// Call for lua_CFunctions
		/// inline
		static inline int Call(lua_CFunction pfn, lua_State *L, const TCHAR * /*szLuaFnName*/ ) { return (*pfn)(L); }

		// Call specializations for static functions in FLuaStaticFunctions.h
		// Call specializations for non-static member functions in FLuaTypeExposure.h

		// TypeNameHolder - Used to hold a typename for a C++ type being used by lua.
		/// size ?
		template<class T> struct TypeNameHolder {
			/// inline
			static const char *&Get() { static const char *szName = "Unknown_Type"; return szName; }
		};

		// FunctionToString - ToString function for functions.  Can be used to display the full function signature.
		/// 0x08dd92fc
		int FunctionToString(lua_State *L);

		// PushCodeBuddyTable - Pushes the code buddy table.  Creates the table if it doesn't exist.
		/// 0x08dd9420
		void PushCodeBuddyTable(lua_State *L);

		// PushFunctionDocumentationTable - Pushes the function documentation table from CodeBuddy.
		//   Creates the table if it doesn't exist.
		/// 0x08dd8c73
		void PushFunctionDocumentationTable(lua_State *L);

		// SetToStringForFunc - Assign a tostring function for a function exposed to lua.
		//   The function is expected to be at the top of the stack.
		/// 0x08dd94c8
		void SetToStringForFunc(lua_State *L, lua_CFunction pfnToString);

		// DescribeType - Used to describe function arguments and return values to lua.
		/// inline
		template<class T> static const char *DescribeType() { return TypeNameHolder<typename StripPtrAndRef<T>::Result>::Get(); }
		/// inline
		template<> static const char *DescribeType<void>() { return "void"; }
		/// inline
		template<> static const char *DescribeType<bool>() { return "bool"; }
		/// inline
		template<> static const char *DescribeType<const bool>() { return "const bool"; }
		/// inline
		template<> static const char *DescribeType<char>() { return "char"; }
		/// inline
		template<> static const char *DescribeType<const char>() { return "const char"; }
		/// inline
		template<> static const char *DescribeType<int>() { return "int"; }
		/// inline
		template<> static const char *DescribeType<const int>() { return "const int"; }
		/// inline
		template<> static const char *DescribeType<unsigned int>() { return "uint"; }
		/// inline
		template<> static const char *DescribeType<const unsigned int>() { return "const uint"; }
		/// inline
		template<> static const char *DescribeType<unsigned long>() { return "unsigned long"; }
		/// inline
		template<> static const char *DescribeType<const unsigned long>() { return "const unsigned long"; }
		/// inline
		template<> static const char *DescribeType<float>() { return "float"; }
		/// inline
		template<> static const char *DescribeType<const float>() { return "const float"; }
		/// inline
		template<> static const char *DescribeType<double>() { return "double"; }
		/// inline
		template<> static const char *DescribeType<const double>() { return "const double"; }
		/// inline
		template<> static const char *DescribeType<char*>() { return "char"; } // Pointer notation added later
		/// inline
		template<> static const char *DescribeType<const char*>() { return "const char"; }
		/// inline
		template<> static const char *DescribeType<StackValue>() { return "variant"; }

		// PtrDescription - Used to add proper pointer or reference (&) notation to a described type.
		/// size ?
		/// inline
		template<class T> struct PtrDescription { static const char *Get() { return ""; } };
		/// size ?
		/// inline
		template<class T> struct PtrDescription<T*> { static const char *Get() { return "*"; } };
		/// size ?
		/// inline
		template<class T> struct PtrDescription<T&> { static const char *Get() { return "&"; } };

		// DescribeFunc - Use this to describe a function being used by lua.  The default function description is not at all compelling.
		//   The magic is in the specializations of DescribeFunc in FLuaStaticFunctions.h and FLuaTypeExposure.h
		/// inline
		template<class TFunc>
		static void DescribeFunc(TFunc pfn, _In_z_ const char *szFuncName, char *szDescription, size_t uiSize) {
			sprintf_s(szDescription, uiSize, "CFunction: %s", szFuncName);
		}

		// DescribeFunc for lua_CFunctions
		/// inline
		static void DescribeFunc(lua_CFunction, _In_z_ const char *szFuncName, char *szDescription, size_t uiSize) {
			sprintf_s(szDescription, uiSize, "CFunction: %s", szFuncName);
		}

		// PushFunction - Push just about any C++ function onto the stack in a form callable by lua.
		//   szFuncName is for error handling and tostring.
		/// inline
		template<class TSignature>
		void PushFunction(lua_State *L, TSignature pfn, _In_z_ const char *szFuncName)
		{
			// FuncPtrAsLightUserData used to convert a function pointer to a void*
			union FuncPtrAsLightUserData
			{
				void *pvUserData;
				TSignature pfnFunc;
			};

			// LuaInterface defines the functions that lua will use to interact with this function
			struct LuaInterface
			{
				// Thunk simply calls the function
				static int Thunk(lua_State *L)
				{
					FuncPtrAsLightUserData kConverter;
					kConverter.pvUserData = lua_touserdata(L, lua_upvalueindex(1));
					const char *szFuncName = lua_tostring(L, lua_upvalueindex(2));
					return Call(kConverter.pfnFunc, L, szFuncName);
				}

				// ToString will report the function signature
				static int ToString(lua_State *L)
				{
					if( lua_iscfunction(L, -1) )
					{
						// Get the function pointer from an upvalue
						FuncPtrAsLightUserData kConverter;
						if( lua_getupvalue(L, -1, 1) )
						{
							kConverter.pvUserData = lua_touserdata(L, -1);
							lua_pop(L, 1);
						}
						else
							kConverter.pvUserData = NULL;

						// Get the function name from an upvalue
						const char *szFuncName;
						if( lua_getupvalue(L, -1, 2) )
						{
							szFuncName = lua_tostring(L, -1);
							lua_pop(L, 1);
						}
						else
							szFuncName = "UnknownFunction";

						// Use DescribeFunc to get the full function signature
						char szDescription[256];
						DescribeFunc(kConverter.pfnFunc, szFuncName, szDescription, 256);
						lua_pushstring(L, szDescription);
					}
					else // If this is not a C function then someone is misusing this tostring function
						lua_pushstring(L, "Misused ToString Function!");

					return 1;
				}
			};

			// Push the function onto the stack as a C closure with the function name and address
			FuncPtrAsLightUserData kConverter;
			kConverter.pfnFunc = pfn;
			lua_pushlightuserdata(L, kConverter.pvUserData);
			lua_pushstring(L, szFuncName);
			lua_pushcclosure(L, &LuaInterface::Thunk, 2);
			SetToStringForFunc(L, &LuaInterface::ToString);
		}

		// CCallWithErrorHandling - Make a lua protected call to a c function with error handling automatically provided.
		/// 0x08dd9545
		bool CCallWithErrorHandling(lua_State *L, lua_CFunction pfn, void *pvUserData = NULL);

		// HandleCPCallError - Used internally by MakeProtectedCall to handle errors
		/// 0x08dd95bd
		void HandleCPCallError(lua_State *L, int iCPCallResults);
	}

	// MakeProtectedCall - Use this to call a C Function in lua protected mode.  The functions must return void
	//   and take a lua_State* as its first argument.  If the lua encounters an error it will jump out of the
	//   function and return to MakeProtectedCall where it will report the error.  A return value of true means
	//   that the function executed without error.
	/// inline
	template<class TFunc>
	static bool MakeProtectedCall(lua_State *L, TFunc pfn) {
		// Compile error if there is no specialization of MakeProtectedCall for this function signature.
		// If you get a compile error here it means that you are trying to use MakeProtectedCall on a
		// function is not yet supported.  You cannot expose this function until a template specialization
		// for its signature is added.
		FLUA_COMPILE_TIME_ERROR(CantDoProtectedCall);
		return false;
	}

	// MakeProtectedCall specializations for static functions

	/// inline
	template<>
	static bool MakeProtectedCall(lua_State *L, void(*pfn)(lua_State*)) {
		typedef void(*Func)(lua_State*); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(Func pfnFunc) : m_pfnFunc(pfnFunc) {}
			Func m_pfnFunc;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct != NULL);
				if( pkCallStruct != NULL )
					(*pkCallStruct->m_pfnFunc)(L);

				return 0;
			}
		};

		CallStruct kCall(pfn); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// inline
	template<class TArg1, class TPassedArg1>
	static bool MakeProtectedCall(lua_State *L, void(*pfn)(lua_State*, TArg1), TPassedArg1 arg1) {
		typedef void(*Func)(lua_State*, TArg1); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(Func pfnFunc, TArg1 arg1) : m_pfnFunc(pfnFunc), m_Arg1(arg1){}
			Func m_pfnFunc;
			TArg1 m_Arg1;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct);
				(*pkCallStruct->m_pfnFunc)(L, pkCallStruct->m_Arg1);
				return 0;
			}
		};

		CallStruct kCall(pfn, arg1); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// inline
	template<class TArg1, class TArg2, class TPassedArg1, class TPassedArg2>
	static bool MakeProtectedCall(lua_State *L, void(*pfn)(lua_State*, TArg1, TArg2), TPassedArg1 arg1, TPassedArg2 arg2) {
		typedef void(*Func)(lua_State*, TArg1, TArg2); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(Func pfnFunc, TArg1 arg1, TArg2 arg2) : m_pfnFunc(pfnFunc), m_Arg1(arg1), m_Arg2(arg2) {}
			Func m_pfnFunc;
			TArg1 m_Arg1;
			TArg2 m_Arg2;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct);
				(*pkCallStruct->m_pfnFunc)(L, pkCallStruct->m_Arg1, pkCallStruct->m_Arg2);
				return 0;
			}
		};

		CallStruct kCall(pfn, arg1, arg2); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// inline
	template<class TArg1, class TArg2, class TArg3, class TPassedArg1, class TPassedArg2, class TPassedArg3>
	static bool MakeProtectedCall(lua_State *L, void(*pfn)(lua_State*, TArg1, TArg2, TArg3), TPassedArg1 arg1, TPassedArg2 arg2, TPassedArg3 arg3) {
		typedef void(*Func)(lua_State*, TArg1, TArg2, TArg3); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(Func pfnFunc, TArg1 arg1, TArg2 arg2, TArg3 arg3) : m_pfnFunc(pfnFunc), m_Arg1(arg1), m_Arg2(arg2), m_Arg3(arg3) {}
			Func m_pfnFunc;
			TArg1 m_Arg1;
			TArg2 m_Arg2;
			TArg3 m_Arg3;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct);
				(*pkCallStruct->m_pfnFunc)(L, pkCallStruct->m_Arg1, pkCallStruct->m_Arg2, pkCallStruct->m_Arg3);
				return 0;
			}
		};

		CallStruct kCall(pfn, arg1, arg2, arg3); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// inline
	template<class TArg1, class TArg2, class TArg3, class TArg4, class TPassedArg1, class TPassedArg2, class TPassedArg3, class TPassedArg4>
	static bool MakeProtectedCall(lua_State *L, void(*pfn)(lua_State*, TArg1, TArg2, TArg3, TArg4),
	                              TPassedArg1 arg1, TPassedArg2 arg2, TPassedArg3 arg3, TPassedArg4 arg4) {
		typedef void(*Func)(lua_State*, TArg1, TArg2, TArg3, TArg4); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(Func pfnFunc, TArg1 arg1, TArg2 arg2, TArg3 arg3, TArg4 arg4) :
				m_pfnFunc(pfnFunc), m_Arg1(arg1), m_Arg2(arg2), m_Arg3(arg3), m_Arg4(arg4) {}
			Func m_pfnFunc;
			TArg1 m_Arg1;
			TArg2 m_Arg2;
			TArg3 m_Arg3;
			TArg4 m_Arg4;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct);
				(*pkCallStruct->m_pfnFunc)(L, pkCallStruct->m_Arg1, pkCallStruct->m_Arg2, pkCallStruct->m_Arg3, pkCallStruct->m_Arg4);
				return 0;
			}
		};

		CallStruct kCall(pfn, arg1, arg2, arg3, arg4); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	// MakeProtectedCall specializations for member functions

	/// inline
	template<class TClass, class TFuncOwner>
	static bool MakeProtectedCall(lua_State *L, TClass *pkObj, void(TFuncOwner::*pfn)(lua_State*)) {
		typedef void(TFuncOwner::*Func)(lua_State*); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(TClass *pkObj, Func pfnFunc) : m_pkObj(pkObj), m_pfnFunc(pfnFunc) {}
			TClass *m_pkObj;
			Func m_pfnFunc;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct);
				(pkCallStruct->m_pkObj->*(pkCallStruct->m_pfnFunc))(L);
				return 0;
			}
		};

		assert(pkObj);
		CallStruct kCall(pkObj, pfn); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// inline
	template<class TClass, class TFuncOwner, class TArg1, class TPassedArg1>
	static bool MakeProtectedCall(lua_State *L, TClass *pkObj, void(TFuncOwner::*pfn)(lua_State*, TArg1), TPassedArg1 arg1) {
		typedef void(TFuncOwner::*Func)(lua_State*, TArg1); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(TClass *pkObj, Func pfnFunc, TArg1 arg1) : m_pkObj(pkObj), m_pfnFunc(pfnFunc), m_Arg1(arg1){}
			TClass *m_pkObj;
			Func m_pfnFunc;
			TArg1 m_Arg1;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct);
				(pkCallStruct->m_pkObj->*(pkCallStruct->m_pfnFunc))(L, pkCallStruct->m_Arg1);
				return 0;
			}
		};

		assert(pkObj);
		CallStruct kCall(pkObj, pfn, arg1); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// inline
	template<class TClass, class TFuncOwner, class TArg1, class TPassedArg1, class TArg2, class TPassedArg2>
	static bool MakeProtectedCall(lua_State *L, TClass *pkObj, void(TFuncOwner::*pfn)(lua_State*, TArg1, TArg2),
	                              TPassedArg1 arg1, TPassedArg2 arg2) {
		typedef void(TFuncOwner::*Func)(lua_State*, TArg1, TArg2); // typedef for the function signature
		struct CallStruct { // The call struct serves as our user data
			CallStruct(TClass *pkObj, Func pfnFunc, TArg1 arg1, TArg2 arg2) : m_pkObj(pkObj), m_pfnFunc(pfnFunc), m_Arg1(arg1), m_Arg2(arg2) {}
			TClass *m_pkObj;
			Func m_pfnFunc;
			TArg1 m_Arg1;
			TArg2 m_Arg2;

			// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
			static int Execute(lua_State *L) {
				CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
				assert(pkCallStruct);
				(pkCallStruct->m_pkObj->*(pkCallStruct->m_pfnFunc))(L, pkCallStruct->m_Arg1, pkCallStruct->m_Arg2);
				return 0;
			}
		};

		assert(pkObj);
		CallStruct kCall(pkObj, pfn, arg1, arg2); // Build the call object for this call
		return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// inline
	template<class TClass, class TFuncOwner, class TArg1, class TPassedArg1, class TArg2, class TPassedArg2, class TArg3, class TPassedArg3>
	static bool MakeProtectedCall(lua_State *L, TClass *pkObj, void(TFuncOwner::*pfn)(lua_State*, TArg1, TArg2),
	                              TPassedArg1 arg1, TPassedArg2 arg2, TPassedArg3 arg3) {
			typedef void(TFuncOwner::*Func)(lua_State*, TArg1, TArg2, TArg3); // typedef for the function signature
			struct CallStruct { // The call struct serves as our user data
				CallStruct(TClass *pkObj, Func pfnFunc, TArg1 arg1, TArg2 arg2, TArg3 arg3) :
					m_pkObj(pkObj), m_pfnFunc(pfnFunc), m_Arg1(arg1), m_Arg2(arg2), m_Arg3(arg3) {}
				TClass *m_pkObj;
				Func m_pfnFunc;
				TArg1 m_Arg1;
				TArg2 m_Arg2;
				TArg3 m_Arg3;

				// The Execute function is here as a sneaky way of defining it inside MakeProtectedCall
				static int Execute(lua_State *L) {
					CallStruct *pkCallStruct = (CallStruct*)lua_touserdata(L, -1);
					assert(pkCallStruct);
					(pkCallStruct->m_pkObj->*(pkCallStruct->m_pfnFunc))(L, pkCallStruct->m_Arg1, pkCallStruct->m_Arg2, pkCallStruct->m_Arg3);
					return 0;
				}
			};

			assert(pkObj);
			CallStruct kCall(pkObj, pfn, arg1, arg2, arg3); // Build the call object for this call
			return Details::CCallWithErrorHandling(L, &CallStruct::Execute, &kCall); // Make the cp call
	}

	/// 0x101b2a50
	extern FCriticalSection	ms_CriticalSection;
}

#if !defined(FINAL_RELEASE)
#define FLUA_OUTPUT_CALLSTACKS
#endif

#ifdef FLUA_OUTPUT_CALLSTACKS
#define FLUA_SAVECALLSTACK(x) FLua::Details::SaveCallStack(x)
#else
#define FLUA_SAVECALLSTACK(x) ((void)0)
#endif

#ifdef FLUA_MULTITHREAD_SUPPORT
 #define FLUA_ENTER_CRITICAL_SECTION		FLua::ms_CriticalSection.Enter()
 #define FLUA_LEAVE_CRITICAL_SECTION		FLua::ms_CriticalSection.Leave()
#else
 #define FLUA_ENTER_CRITICAL_SECTION		((void)0)
 #define FLUA_LEAVE_CRITICAL_SECTION		((void)0)
#endif
#endif
