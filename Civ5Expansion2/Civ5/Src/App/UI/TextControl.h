/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include <lua.h>
#include <memory>
#include <vector>

class ColorSet;
class FGXFont;
class FGXVector2;
class FGXVector3;
class FGXVector4;
class FXmlElement;
class Glyph;
class LuaContext;
class TextControlBlock;
class UIRenderBuffer;
enum UIAABB : int;
namespace FireGrafix {
template <int T0> class VStruct;
}

/// size ?
/// vtable 0x0a2f9220
class TextControl : public ControlBase {
public:
    /// 0x086996a2
    TextControl();
    /// 0x08699844
    /// deleting 0x0869981c
    virtual ~TextControl();
    /// 0x0869b860
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x0869da38
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0869ea68
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x08699498
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x0869fa40
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x08699a28
    virtual UNK_RET Initialize();
    /// 0x0869e036
    virtual UNK_RET PerspectiveRender(unsigned int a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x0869f0a2
    virtual UNK_RET PerspectiveRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x08699622
    virtual int PushSelf(lua_State* a1);
    /// 0x0869d4cc
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0869e4aa
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x086a1872
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x086a1878
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x086a189e
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x086998c6
    UNK_RET Release();
    /// 0x08699b54
    UNK_RET SetFont(FGXFont* a1);
    /// 0x08699be6
    UNK_RET SetStyle(FStringA const& a1);
    /// 0x08699c2e
    UNK_RET RebuildText();
    /// 0x08699c7a
    UNK_RET SetColor(FGXVector4 a1, int a2);
    /// 0x08699cbe
    UNK_RET SetColorSet(ColorSet& a1);
    /// 0x08699d08
    UNK_RET SetColorByName(char const* a1);
    /// 0x08699d52
    UNK_RET SetStyleByID(unsigned int a1);
    /// 0x08699d7e
    UNK_RET SetWrapWidth(int a1);
    /// 0x08699dac
    UNK_RET SetTruncateWidth(int a1);
    /// 0x08699dda
    UNK_RET SetParseMode(bool a1);
    /// 0x08699dfe
    UNK_RET SetText(char const* a1);
    /// 0x08699e96
    UNK_RET ParseTextIntoBlocks();
    /// 0x0869a9b8
    UNK_RET ProcessWordwrap();
    /// 0x0869aea4
    UNK_RET CalculateTruncation();
    /// 0x0869aefc
    UNK_RET CalculateSize();
    /// 0x0869b2d0
    UNK_RET GetText() const;
    /// 0x0869b2e4
    UNK_RET BuildCoreFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x0869b8ae
    UNK_RET GetIconSurfaceIndexByFont(FGXFont* a1);
    /// 0x0869b96a
    UNK_RET FontHasGlyph(unsigned int a1) const;
    /// 0x0869b9b4
    UNK_RET FontGetGlyphFromIntValue(unsigned int a1) const;
    /// 0x0869b9fc
    UNK_RET IsBreakGlyph(unsigned int a1) const;
    /// 0x0869ba3e
    UNK_RET GetStringSize(char const* a1) const;
    /// 0x0869bb3a
    UNK_RET ShouldTruncateHere(unsigned int a1, Glyph const& a2, unsigned int a3) const;
    /// 0x0869bbae
    UNK_RET InsertTruncationTrail(unsigned int a1, unsigned int a2, char const* a3, TextControlBlock const& a4);
    /// 0x0869c166
    UNK_RET BuildBuffersFromBlocks();
    /// 0x0869cdba
    UNK_RET AllocateAndFillBuffers(std::vector<std::vector<FireGrafix::VStruct<9>, std::allocator<FireGrafix::VStruct<9> > >, std::allocator<std::vector<FireGrafix::VStruct<9>, std::allocator<FireGrafix::VStruct<9> > > > > const& a1);
    /// 0x0869f562
    static int LocalizeAndSetText(lua_State* a1);
    /// 0x0869f854
    /// lua ?
    static int lSetText(lua_State* a1);
    /// 0x0869f8c2
    UNK_RET SetForceIME(bool a1);
    /// 0x0869f94c
    UNK_RET SetForceNonIME(bool a1);
    /// 0x0869f9dc
    UNK_RET SetFontByName(char const* a1);
    /// 0x0869fa1c
    UNK_RET SetTextObscured(bool a1);
    /// 0x086a0f06 ShouldWrapHere<FFastVector<TextControlBlock, true, 19u, 8u, FTempHeapFastVectorAllocator> >
    template <class T0>
    bool ShouldWrapHere(T0 const& a1, unsigned int a2, unsigned int a3, float a4) const;
    /// 0x0a3caf88
    static UNK_DATA m_TempIconVB[0xc];
    /// 0x0a3caf64
    static UNK_DATA m_TempImeVB[0x24];
    /// 0x0a3cae8c
    static UNK_DATA m_TempVBLayer[0xd8];
    /// 0x0a3cae80
    static UNK_DATA m_abBreakRenderChars[0xc];
    /// 0x0a3cae74
    static UNK_DATA m_acBreakChars[0xc];
    /// 0x0a3cae58
    static UNK_DATA s_Cache[0x1c];
};

/// 0x0869fc31
/// lua ?
int LuaThunk_TextControl_SetString(lua_State* a1);
/// 0x0869fc45
int LuaToString_TextControl_SetString(lua_State* a1);
/// 0x0869fca4
/// lua ?
int LuaThunk_TextControl_SetText(lua_State* a1);
/// 0x0869fcb8
int LuaToString_TextControl_SetText(lua_State* a1);
/// 0x0869fd17
/// lua ?
int LuaThunk_TextControl_LocalizeAndSetText(lua_State* a1);
/// 0x0869fd2b
int LuaToString_TextControl_LocalizeAndSetText(lua_State* a1);
/// 0x0869fd8a
/// lua ?
int LuaThunk_TextControl_SetColor(lua_State* a1);
/// 0x086a002b
int LuaToString_TextControl_SetColor(lua_State* a1);
/// 0x086a00ba
/// lua ?
int LuaThunk_TextControl_SetColorByName(lua_State* a1);
/// 0x086a01a6
int LuaToString_TextControl_SetColorByName(lua_State* a1);
/// 0x086a022d
/// lua ?
int LuaThunk_TextControl_GetText(lua_State* a1);
/// 0x086a04b4
int LuaToString_TextControl_GetText(lua_State* a1);
/// 0x086a0527
/// lua ?
int LuaThunk_TextControl_SetWrapWidth(lua_State* a1);
/// 0x086a060b
int LuaToString_TextControl_SetWrapWidth(lua_State* a1);
/// 0x086a068c
/// lua ?
int LuaThunk_TextControl_SetTruncateWidth(lua_State* a1);
/// 0x086a06cf
int LuaToString_TextControl_SetTruncateWidth(lua_State* a1);
/// 0x086a0750
/// lua ?
int LuaThunk_TextControl_SetForceIME(lua_State* a1);
/// 0x086a083c
int LuaToString_TextControl_SetForceIME(lua_State* a1);
/// 0x086a08bd
/// lua ?
int LuaThunk_TextControl_SetForceNonIME(lua_State* a1);
/// 0x086a0900
int LuaToString_TextControl_SetForceNonIME(lua_State* a1);
/// 0x086a0981
/// lua ?
int LuaThunk_TextControl_SetFontByName(lua_State* a1);
/// 0x086a09c4
int LuaToString_TextControl_SetFontByName(lua_State* a1);

namespace Localization { namespace UTF8 {
/// 0x0895421c NextUnsafe<char, int>
/// 0x086a1440 NextUnsafe<char, unsigned int>
template <class T0, class T1>
unsigned char NextUnsafe(T0 const* a1, unsigned int a2, T1& a3);
}}
