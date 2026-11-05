/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include <lua.h>

class FXmlElement;
class LuaContext;

/// size ?
/// vtable 0x0a2fdf90
class AnimControl : public ControlBase {
public:
    /// 0x0899d214
    AnimControl();
    /// 0x089a285c
    /// deleting 0x089a2894
    virtual ~AnimControl();
    /// 0x0899d2b4
    virtual UNK_RET BranchResetAnimation();
    /// 0x0899d870
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x0899d37a
    virtual UNK_RET Initialize();
    /// 0x0899d490
    virtual UNK_RET Update(float a1);
    /// pure
    virtual UNK_RET vf_19(UNK_PARAMS) = 0;
    /// 0x089a28d6
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// pure
    virtual UNK_RET UpdateAnim(float a1) = 0;
    /// pure
    virtual UNK_RET vf_49(UNK_PARAMS) = 0;
    /// 0x0899d318
    UNK_RET SetToBeginning();
    /// 0x0899d370
    UNK_RET Play();
    /// 0x0899d3c0
    UNK_RET SetToEnd();
    /// 0x0899d434
    UNK_RET Stop();
    /// 0x0899d43e
    UNK_RET Reverse();
    /// 0x0899d7ec
    UNK_RET GetFunctionOut(float a1);
};

/// 0x0899f3d4
/// lua ?
int LuaThunk_SlideAnim_SetBeginVal(lua_State* a1);
/// 0x0899f6dc
int LuaToString_SlideAnim_SetBeginVal(lua_State* a1);
/// 0x0899f765
/// lua ?
int LuaThunk_SlideAnim_SetEndVal(lua_State* a1);
/// 0x0899f7a8
int LuaToString_SlideAnim_SetEndVal(lua_State* a1);
/// 0x0899f831
/// lua ?
int LuaThunk_AnimControl_RegisterAnimCallback(lua_State* a1);
/// 0x0899f953
int LuaToString_AnimControl_RegisterAnimCallback(lua_State* a1);
/// 0x0899f9d4
/// lua ?
int LuaThunk_AnimControl_IncrementPause(lua_State* a1);
/// 0x0899fac4
int LuaToString_AnimControl_IncrementPause(lua_State* a1);
/// 0x0899fb45
/// lua ?
int LuaThunk_AnimControl_SetPauseTime(lua_State* a1);
/// 0x0899fb88
int LuaToString_AnimControl_SetPauseTime(lua_State* a1);
/// 0x0899fc09
/// lua ?
int LuaThunk_AnimControl_Play(lua_State* a1);
/// 0x0899fcd1
int LuaToString_AnimControl_Play(lua_State* a1);
/// 0x0899fd44
/// lua ?
int LuaThunk_AnimControl_Stop(lua_State* a1);
/// 0x0899fd87
int LuaToString_AnimControl_Stop(lua_State* a1);
/// 0x0899fdfa
/// lua ?
int LuaThunk_AnimControl_IsStopped(lua_State* a1);
/// 0x0899fed6
int LuaToString_AnimControl_IsStopped(lua_State* a1);
/// 0x0899ff49
/// lua ?
int LuaThunk_AnimControl_Reverse(lua_State* a1);
/// 0x0899ff8c
int LuaToString_AnimControl_Reverse(lua_State* a1);
/// 0x0899ffff
/// lua ?
int LuaThunk_AnimControl_IsReversing(lua_State* a1);
/// 0x089a0040
int LuaToString_AnimControl_IsReversing(lua_State* a1);
/// 0x089a00b3
/// lua ?
int LuaThunk_AnimControl_SetToBeginning(lua_State* a1);
/// 0x089a00f6
int LuaToString_AnimControl_SetToBeginning(lua_State* a1);
/// 0x089a0169
/// lua ?
int LuaThunk_AnimControl_SetToEnd(lua_State* a1);
/// 0x089a01ac
int LuaToString_AnimControl_SetToEnd(lua_State* a1);
/// 0x089a021f
/// lua ?
int LuaThunk_AnimControl_GetProgress(lua_State* a1);
/// 0x089a02a7
int LuaToString_AnimControl_GetProgress(lua_State* a1);
/// 0x089a031a
/// lua ?
int LuaThunk_AnimControl_SetProgress(lua_State* a1);
/// 0x089a035d
int LuaToString_AnimControl_SetProgress(lua_State* a1);
/// 0x089a03de
/// lua ?
int LuaThunk_FlipAnim_SetFrame(lua_State* a1);
/// 0x089a0652
int LuaToString_FlipAnim_SetFrame(lua_State* a1);
/// 0x089a06d3
/// lua ?
int LuaThunk_AlphaAnim_SetTexture(lua_State* a1);
/// 0x089a077c
int LuaToString_AlphaAnim_SetTexture(lua_State* a1);
/// 0x089a0803
/// lua ?
int LuaThunk_AlphaAnim_SetTextureOffset(lua_State* a1);
/// 0x089a08a5
int LuaToString_AlphaAnim_SetTextureOffset(lua_State* a1);
/// 0x089a0926
/// lua ?
int LuaThunk_AlphaAnim_SetTextureOffsetVal(lua_State* a1);
/// 0x089a0a42
int LuaToString_AlphaAnim_SetTextureOffsetVal(lua_State* a1);
/// 0x089a0acb
/// lua ?
int LuaThunk_AlphaAnim_SetSize(lua_State* a1);
/// 0x089a0b6f
int LuaToString_AlphaAnim_SetSize(lua_State* a1);
/// 0x089a0bf0
/// lua ?
int LuaThunk_AlphaAnim_SetSizeVal(lua_State* a1);
/// 0x089a0c31
int LuaToString_AlphaAnim_SetSizeVal(lua_State* a1);
/// 0x089a0cba
/// lua ?
int LuaThunk_ScrollAnim_SetTexture(lua_State* a1);
/// 0x089a0dbb
int LuaToString_ScrollAnim_SetTexture(lua_State* a1);
/// 0x089a0e42
/// lua ?
int LuaThunk_ScrollAnim_SetMask(lua_State* a1);
/// 0x089a0e83
int LuaToString_ScrollAnim_SetMask(lua_State* a1);
/// 0x089a0f0a
/// lua ?
int LuaThunk_ScrollAnim_SetTextureAndMask(lua_State* a1);
/// 0x089a0fd3
int LuaToString_ScrollAnim_SetTextureAndMask(lua_State* a1);
/// 0x089a1062
/// lua ?
int LuaThunk_ScrollAnim_SetTextureOffset(lua_State* a1);
/// 0x089a1100
int LuaToString_ScrollAnim_SetTextureOffset(lua_State* a1);
/// 0x089a1181
/// lua ?
int LuaThunk_ScrollAnim_SetTextureOffsetVal(lua_State* a1);
/// 0x089a1234
int LuaToString_ScrollAnim_SetTextureOffsetVal(lua_State* a1);
