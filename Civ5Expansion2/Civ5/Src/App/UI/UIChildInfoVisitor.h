/// origin guess
#pragma once
#include "Civ5/Src/App/IUINodeVisitor.h"
#include <lua.h>

class ControlBase;

/// size ?
/// vtable 0x0a2f99f0
class UIChildInfoVisitor : public IUINodeVisitor {
public:
    /// 0x086d1174
    UIChildInfoVisitor();
    /// 0x086d6812
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x086d6a62
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x086d11e8
    virtual UNK_RET Visit(ControlBase const* a1);
};

/// 0x086d27e1
/// lua ?
int LuaThunk_LookUpControl(lua_State* a1);
/// 0x086d2856
int LuaToString_LookUpControl(lua_State* a1);
/// 0x086d28b5
/// lua ?
int LuaThunk_UIManager_GetMousePos(lua_State* a1);
/// 0x086d290d
int LuaToString_UIManager_GetMousePos(lua_State* a1);
/// 0x086d296c
/// lua ?
int LuaThunk_UIManager_GetMouseDelta(lua_State* a1);
/// 0x086d29c4
int LuaToString_UIManager_GetMouseDelta(lua_State* a1);
/// 0x086d2a23
/// lua ?
int LuaThunk_UIManager_GetScreenSizeVal(lua_State* a1);
/// 0x086d2a73
int LuaToString_UIManager_GetScreenSizeVal(lua_State* a1);
/// 0x086d2ad2
/// lua ?
int LuaThunk_UIManager_GetControl(lua_State* a1);
/// 0x086d2bae
int LuaToString_UIManager_GetControl(lua_State* a1);
/// 0x086d2c21
/// lua ?
int LuaThunk_UIManager_GetAlt(lua_State* a1);
/// 0x086d2c62
int LuaToString_UIManager_GetAlt(lua_State* a1);
/// 0x086d2cd5
/// lua ?
int LuaThunk_UIManager_GetShift(lua_State* a1);
/// 0x086d2d16
int LuaToString_UIManager_GetShift(lua_State* a1);
/// 0x086d2d89
/// lua ?
int LuaThunk_UIManager_GetNumPointers(lua_State* a1);
/// 0x086d2e22
int LuaToString_UIManager_GetNumPointers(lua_State* a1);
/// 0x086d2e95
/// lua ?
int LuaThunk_UIManager_SetUICursor(lua_State* a1);
/// 0x086d2f9e
int LuaToString_UIManager_SetUICursor(lua_State* a1);
/// 0x086d3019
/// lua ?
int LuaThunk_UIManager_GetGameGridTopLeft(lua_State* a1);
/// 0x086d31f2
int LuaToString_UIManager_GetGameGridTopLeft(lua_State* a1);
/// 0x086d3273
/// lua ?
int LuaThunk_UIManager_GetGameGridBottomRight(lua_State* a1);
/// 0x086d32b4
int LuaToString_UIManager_GetGameGridBottomRight(lua_State* a1);
/// 0x086d3335
/// lua ?
int LuaThunk_UIManager_PushModal(lua_State* a1);
/// 0x086d3349
int LuaToString_UIManager_PushModal(lua_State* a1);
/// 0x086d33a8
/// lua ?
int LuaThunk_UIManager_PopModal(lua_State* a1);
/// 0x086d3431
int LuaToString_UIManager_PopModal(lua_State* a1);
/// 0x086d34b8
/// lua ?
int LuaThunk_UIManager_IsModal(lua_State* a1);
/// 0x086d35ad
int LuaToString_UIManager_IsModal(lua_State* a1);
/// 0x086d3634
/// lua ?
int LuaThunk_UIManager_IsTopModal(lua_State* a1);
/// 0x086d3675
int LuaToString_UIManager_IsTopModal(lua_State* a1);
/// 0x086d36fc
/// lua ?
int LuaThunk_UIManager_PopAllInterrupts(lua_State* a1);
/// 0x086d37c4
int LuaToString_UIManager_PopAllInterrupts(lua_State* a1);
/// 0x086d3837
/// lua ?
int LuaThunk_UIManager_PrintAllocationInfo(lua_State* a1);
/// 0x086d387a
int LuaToString_UIManager_PrintAllocationInfo(lua_State* a1);
/// 0x086d38ed
/// lua ?
int LuaThunk_UIManager_PrintTextureInfo(lua_State* a1);
/// 0x086d3930
int LuaToString_UIManager_PrintTextureInfo(lua_State* a1);
/// 0x086d39a3
/// lua ?
int LuaThunk_UIManager_PrintWindowInfo(lua_State* a1);
/// 0x086d3a8f
int LuaToString_UIManager_PrintWindowInfo(lua_State* a1);
/// 0x086d3b16
/// lua ?
int LuaThunk_UIManager_SetHideWorldAnchors(lua_State* a1);
/// 0x086d3c02
int LuaToString_UIManager_SetHideWorldAnchors(lua_State* a1);
/// 0x086d3c83
/// lua ?
int LuaThunk_UIManager_SetDrawControlBounds(lua_State* a1);
/// 0x086d3cc6
int LuaToString_UIManager_SetDrawControlBounds(lua_State* a1);
/// 0x086d3d47
/// lua ?
int LuaThunk_UIManager_ReleaseTextureByName(lua_State* a1);
/// 0x086d3d8a
int LuaToString_UIManager_ReleaseTextureByName(lua_State* a1);
/// 0x086d3e11
/// lua ?
int LuaThunk_UIManager_ReleaseAndUnloadTextureByName(lua_State* a1);
/// 0x086d3e54
int LuaToString_UIManager_ReleaseAndUnloadTextureByName(lua_State* a1);
/// 0x086d3edb
/// lua ?
int LuaThunk_UIManager_RequestReloadUI(lua_State* a1);
/// 0x086d3f1e
int LuaToString_UIManager_RequestReloadUI(lua_State* a1);
/// 0x086d3f91
/// lua ?
int LuaThunk_UIManager_RequestReprocessAnchoring(lua_State* a1);
/// 0x086d3fd4
int LuaToString_UIManager_RequestReprocessAnchoring(lua_State* a1);
/// 0x086d4047
/// lua ?
int LuaThunk_UIManager_QueuePopup(lua_State* a1);
/// 0x086d4139
int LuaToString_UIManager_QueuePopup(lua_State* a1);
/// 0x086d41ce
/// lua ?
int LuaThunk_UIManager_DequeuePopup(lua_State* a1);
/// 0x086d4252
int LuaToString_UIManager_DequeuePopup(lua_State* a1);
/// 0x086d42d9
/// lua ?
int LuaThunk_UIManager_ApplyResolution(lua_State* a1);
/// 0x086d431c
int LuaToString_UIManager_ApplyResolution(lua_State* a1);
/// 0x086d439d
/// lua ?
int LuaThunk_UIManager_IsExtendedFont(lua_State* a1);
/// 0x086d43de
int LuaToString_UIManager_IsExtendedFont(lua_State* a1);
/// 0x086d4451
/// lua ?
int LuaThunk_UIManager_GetResInfo(lua_State* a1);
/// 0x086d4465
int LuaToString_UIManager_GetResInfo(lua_State* a1);
/// 0x086d44c4
/// lua ?
int LuaThunk_UIManager_GetResCount(lua_State* a1);
/// 0x086d44f6
int LuaToString_UIManager_GetResCount(lua_State* a1);
/// 0x086d4555
/// lua ?
int LuaThunk_UIManager_FlushInputMessages(lua_State* a1);
/// 0x086d4598
int LuaToString_UIManager_FlushInputMessages(lua_State* a1);
/// 0x086d460b
/// lua ?
int LuaThunk_UIManager_GetVisibleNamedContext(lua_State* a1);
/// 0x086d461f
int LuaToString_UIManager_GetVisibleNamedContext(lua_State* a1);
/// 0x086d467e
/// lua ?
int LuaThunk_UIManager_GetNamedContextCount(lua_State* a1);
/// 0x086d4739
int LuaToString_UIManager_GetNamedContextCount(lua_State* a1);
/// 0x086d47c0
/// lua ?
int LuaThunk_UIManager_GetNamedContextByIndex(lua_State* a1);
/// 0x086d47d4
int LuaToString_UIManager_GetNamedContextByIndex(lua_State* a1);
