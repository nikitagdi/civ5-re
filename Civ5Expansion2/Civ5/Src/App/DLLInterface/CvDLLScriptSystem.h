/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/ICvDLLScriptSystem.h"
#include <lua.h>

/// size ?
/// vtable 0x0a2ffe00
class CvDLLScriptSystem : public ICvEngineScriptSystem1 {
public:
    /// 0x08a02ac8
    CvDLLScriptSystem();
    /// 0x08a02fba
    virtual bool CallAccumulator(char const* a1, ICvEngineScriptSystemArgs1* a2, int& a3);
    /// 0x08a03002
    virtual bool CallAccumulator(char const* a1, ICvEngineScriptSystemArgs1* a2, float& a3);
    /// 0x08a02f0a
    virtual bool CallCFunction(lua_State* a1, int (*a2)(lua_State*), void* a3);
    /// 0x08a02f3e
    virtual bool CallFunction(lua_State* a1, unsigned int a2, unsigned int a3);
    /// 0x08a02f72
    virtual bool CallHook(char const* a1, ICvEngineScriptSystemArgs1* a2, bool& a3);
    /// 0x08a0304a
    virtual bool CallTestAll(char const* a1, ICvEngineScriptSystemArgs1* a2, bool& a3);
    /// 0x08a03092
    virtual bool CallTestAny(char const* a1, ICvEngineScriptSystemArgs1* a2, bool& a3);
    /// 0x08a02df8
    virtual ICvEngineScriptSystemArgs1 * CreateArgs();
    /// 0x08a02e5c
    virtual ICvEngineScriptSystemArgs1 * CreateArgs(unsigned int a1);
    /// 0x08a02b5a
    virtual lua_State * CreateLuaThread(char const* a1);
    /// 0x08a02eb6
    virtual void DestroyArgs(ICvEngineScriptSystemArgs1* a1);
    /// 0x08a02ce6
    virtual void FreeLuaThread(lua_State* a1);
    /// 0x08a02d40
    virtual bool LoadFile(lua_State* a1, char const* a2);
    /// 0x08a02b2a
    /// deleting 0x08a02af4
    virtual ~CvDLLScriptSystem();
};
