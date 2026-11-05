/// origin guess
#pragma once
#include "Civ5/Src/App/cvUnitSetActionResultCheck.h"
#include <lua.h>

class cvUnitOverrideFSMState;
namespace FireGrafix {
class FGXModelNode;
}

/// size ?
class cvUnitVis {
public:
    class FadeOp;
    class RenderLayer;
    /// 0x08a6cb80
    cvUnitVis();
    /// 0x08a6cc94
    ~cvUnitVis();
    /// 0x08a6cc96
    UNK_RET Init();
    /// 0x08a6ccfc
    UNK_RET Release();
    /// 0x08a6cd38
    void AssignDamageStatesToNodes(float fTime);
    /// 0x08a6cd8a
    void SetAlphaState(float fFromAlpha, float fToAlpha, float fStartTime, float fDuration, cvUnitVis::FadeOp const& a5, unsigned char ucState, int iMember);
    /// 0x08a6cee0
    bool SetAlphaValues(float fFromAlpha, float fToAlpha, float fStartTime, float fDuration, cvUnitVis::FadeOp const& a5, unsigned char ucState, unsigned int uiMember);
    /// 0x08a6d05c
    UNK_RET SetRenderLayerForUnitMember(unsigned int a1, cvUnitVis::RenderLayer a2);
    /// 0x08a6d0b6
    UNK_RET SortNodes();
    /// 0x08a6d0f8
    UNK_RET HasAlphaShader(FireGrafix::FGXModelNode const& a1);
    /// 0x08a6d11a
    UNK_RET OverrideFSMState(cvUnitOverrideFSMState const& a1);
    /// 0x08a6d1cc
    UNK_RET StartFSMDump(int a1, unsigned int a2);
    /// 0x08a6d20c
    UNK_RET GetStateIDByName(char const* a1, unsigned int a2) const;
    /// 0x08a6d250
    UNK_RET GetCurrentStateID(unsigned int a1) const;
    /// 0x08a6d28c
    UNK_RET ResetState(unsigned int a1, int a2, float a3);
    /// 0x08a6d2d8
    UNK_RET SetStatePathOptionValue(unsigned int a1, int a2, float a3);
    /// 0x08a6d324
    UNK_RET SetStatePathActionValue(unsigned int a1, float a2);
    /// 0x08a6d370
    UNK_RET GetStatePathOptionValue(unsigned int a1, int a2) const;
    /// 0x08a6d3b4
    UNK_RET GetStatePathActionValue(unsigned int a1) const;
    /// 0x08a6d3f8
    UNK_RET GetStatePathActionResult(unsigned int a1) const;
    /// 0x08a6d434
    UNK_RET GetStatePathActionResultCheckHitCount(unsigned int a1) const;
    /// 0x08a6d470
    UNK_RET SetStatePathActionResultCheck(unsigned int a1, cvUnitSetActionResultCheck::CHECK_TYPE a2, int a3);
    /// 0x08a6d4cc
    UNK_RET GetStateAnimationComplete(unsigned int a1) const;
    /// 0x08a6d508
    UNK_RET GetStateTime(unsigned int a1) const;
    /// 0x08a6d544
    UNK_RET InitNodeRenderLayers();
    /// 0x08a6d58e
    UNK_RET PublishData(unsigned int a1, unsigned int a2);
    /// 0x08a6d65e
    UNK_RET HasAnimationStates();
    /// 0x08a6d6d4
    UNK_RET HasAnimation(unsigned int a1);
};

namespace cvLuaAutomationLibrary {
/// 0x08a6d884
int Register(lua_State* a1);
/// 0x08a6d8b8
int pRegister(lua_State* a1);
/// 0x08a6da50
/// lua ?
int lSetGameCoreInit(lua_State* a1);
/// 0x08a6ddb6
/// lua ?
int lSetEventScript(lua_State* a1);
/// 0x08a6de14
/// lua ?
int lSetEventFunction(lua_State* a1);
/// 0x08a6dece
/// lua ?
int lGetAutorunTurnLimit(lua_State* a1);
/// 0x08a6df5c
/// lua ?
int lExitGame(lua_State* a1);
/// 0x08a6df94
/// lua ?
int lIsExitingGame(lua_State* a1);
}

namespace cvLuaEventLibrary {
/// 0x08a6e0e4
UNK_RET Register(lua_State* a1, unsigned int a2);
/// 0x08a6e132
int pRegister(lua_State* a1);
/// 0x08a6e1e8
UNK_RET CreateEventFactory(lua_State* a1, unsigned int a2);
/// 0x08a6e378
/// lua ?
int lRegisterAllNonSerialEvents(lua_State* a1);
/// 0x08a6e972
/// lua ?
int lRegisterAllSerialEvents(lua_State* a1);
/// 0x08a71528
/// lua ?
int lRegisterAllLocalMachineEvents(lua_State* a1);
/// 0x08a7200e
/// lua ?
int lRegisterAllLuaOnlyEvents(lua_State* a1);
/// 0x08a720dc
/// lua ?
int lEventFactoryIndexer(lua_State* a1);
}
