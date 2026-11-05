/// origin guess
#pragma once
#include "Civ5/Src/App/ButtonControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <list>
#include <lua.h>
#include <memory>

class ControlBase;
class FGXVector2;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2ff730
class TextButtonControl : public ButtonControlBase {
public:
    /// 0x089d396a
    TextButtonControl();
    /// 0x089d4d54
    /// deleting 0x089d4d8c
    virtual ~TextButtonControl();
    /// 0x089d3a84
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089d417e
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d42fe
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d3760
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089d48a4
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089d3a48
    virtual UNK_RET Initialize();
    /// 0x089d38ea
    virtual int PushSelf(lua_State* a1);
    /// 0x089d40c6
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d4246
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d43c6
    virtual UNK_RET SetVisState(unsigned int a1);
    /// 0x089d4dce
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089d4dd4
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089d4dfa
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089d3ad2
    UNK_RET BuildCoreFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089d4426
    UNK_RET SetText(char const* a1);
    /// 0x089d446c
    static int LocalizeAndSetText(lua_State* a1);
    /// 0x0c0821dc
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089d48f2
/// lua ?
int LuaThunk_TextButtonControl_SetText(lua_State* a1);
/// 0x089d49a2
int LuaToString_TextButtonControl_SetText(lua_State* a1);
/// 0x089d4a29
/// lua ?
int LuaThunk_TextButtonControl_LocalizeAndSetText(lua_State* a1);
/// 0x089d4a3d
int LuaToString_TextButtonControl_LocalizeAndSetText(lua_State* a1);
