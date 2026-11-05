/// origin guess

#pragma once

#include "FirePlace/FireWorks/FFireTypes.h"
#include <stddef.h>
#include <lua.h>
#ifndef ICVDLLSCRIPTSYSTEM_H
#define ICVDLLSCRIPTSYSTEM_H

struct lua_State;

/// size 0x4
/// vtable ?
class ICvEngineScriptSystemArgs1
{
public:
	enum ArgType
	{
		ARGTYPE_NONE,
		ARGTYPE_NULL,
		ARGTYPE_BOOL,
		ARGTYPE_STRING,
		ARGTYPE_INT,
		ARGTYPE_FLOAT,
	};
	/// inline
	virtual ~ICvEngineScriptSystemArgs1(){};

	/// pure
	virtual unsigned int Push(bool bValue) = 0;
	/// pure
	virtual unsigned int Push(const char* szValue, size_t lenValue) = 0;
	/// pure
	virtual unsigned int Push(int iValue) = 0;
	/// pure
	virtual unsigned int Push(float fValue) = 0;
	/// pure
	virtual unsigned int PushNULL() = 0;

	/// pure
	virtual unsigned int Count() = 0;
	
	/// pure
	virtual ArgType GetType(unsigned int argIndex) = 0;

	/// pure
	virtual bool GetBool(unsigned int argIndex, bool& bValue) = 0;
	/// pure
	virtual bool GetFloat(unsigned int argIndex, float& fValue) = 0;
	/// pure
	virtual bool GetInt(unsigned int argIndex, int& iValue) = 0;
	/// pure
	virtual bool GetString(unsigned int argIndex, char*& szValue) = 0;
};

/// size 0x4
/// vtable ?
class ICvEngineScriptSystem1
{
public:

	/// pure
	virtual lua_State* CreateLuaThread(_In_z_ const char* szName) = 0;
	/// pure
	virtual void FreeLuaThread(_In_opt_ lua_State*) = 0;

	// File IO
	/// pure
	virtual bool LoadFile(_In_ lua_State* L, _In_z_ const char* szFilename) = 0;

	//Arguments
	/// pure
	virtual __checkReturn ICvEngineScriptSystemArgs1* CreateArgs() = 0;
	/// pure
	virtual __checkReturn ICvEngineScriptSystemArgs1* CreateArgs(uint uiReserve) = 0;
	/// pure
	virtual void DestroyArgs(_In_opt_ ICvEngineScriptSystemArgs1* pkArgs) = 0;

	// Method calling
	/// pure
	virtual bool CallCFunction(_In_ lua_State* L, lua_CFunction func, void* args) = 0;
	/// pure
	virtual bool CallFunction(_In_ lua_State* L, unsigned int nArgs, unsigned int nRet) = 0;

	/// pure
	virtual bool CallHook(_In_z_ const char* szName, _In_opt_ ICvEngineScriptSystemArgs1* args, bool& value) = 0;

	/// pure
	virtual bool CallAccumulator(_In_z_ const char* szName, _In_opt_ ICvEngineScriptSystemArgs1* args, int& value) = 0;
	/// pure
	virtual bool CallAccumulator(_In_z_ const char* szName, _In_opt_ ICvEngineScriptSystemArgs1* args, float& value) = 0;

	/// pure
	virtual bool CallTestAll(_In_z_ const char* szName, _In_opt_ ICvEngineScriptSystemArgs1* args, bool& value) = 0;
	/// pure
	virtual bool CallTestAny(_In_z_ const char* szName, _In_opt_ ICvEngineScriptSystemArgs1* args, bool& value) = 0;
};


#endif //ICVDLLSCRIPTSYSTEM_H
