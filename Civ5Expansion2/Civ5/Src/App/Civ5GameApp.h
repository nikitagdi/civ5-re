/// origin guess
#pragma once
#include "FirePlace/FireWorks/FAppIFace.h"
#include "FirePlace/FireWorks/FString.h"

class FEntity;
class FGrannyAssetInstance;
class FTextKeyModifier;
class NiAVObject;
class NiCamera;
class NiPoint3;

/// size ?
/// vtable 0x0a2fa0c0
class Civ5GameApp : public FAppIFace {
public:
    /// 0x086f45c4
    Civ5GameApp();
    /// 0x086f499a
    virtual UNK_RET AddProfileEntityType(char const* a1);
    /// 0x086f4882
    virtual UNK_RET AllocNewAudioComponent(FEntity* a1) const;
    /// 0x086f494a
    virtual UNK_RET CreateAudioManager();
    /// 0x086f4780
    virtual UNK_RET GetBGLoader() const;
    /// 0x086f4792
    virtual UNK_RET GetCurrentCamera() const;
    /// 0x086f4788
    virtual UNK_RET GetGameSeed();
    /// 0x086f473e
    virtual UNK_RET GetGlobalDefines();
    /// 0x086f460c
    virtual UNK_RET GetHInstance() const;
    /// 0x086f45f0
    virtual UNK_RET GetHandle() const;
    /// 0x086f475a
    virtual UNK_RET GetHotLoader() const;
    /// 0x086f47d2
    virtual UNK_RET GetInput() const;
    /// 0x086f47a0
    virtual UNK_RET GetMapDimensions(int& a1, int& a2, int& a3, int& a4) const;
    /// 0x086f4784
    virtual UNK_RET GetMapSeed();
    /// 0x086f4716
    virtual UNK_RET GetRandomA();
    /// 0x086f472a
    virtual UNK_RET GetRandomMT();
    /// 0x086f4702
    virtual UNK_RET GetRandomS();
    /// 0x086f4878
    virtual UNK_RET GetTuneMgr() const;
    /// 0x086f487c
    virtual UNK_RET GetTunerVariableSystem() const;
    /// 0x086f4670
    virtual UNK_RET GetWindowHeight() const;
    /// 0x086f4646
    virtual UNK_RET GetWindowWidth() const;
    /// 0x086f480a
    virtual UNK_RET HasAcceptedAnInvite() const;
    /// 0x086f46fe
    virtual UNK_RET HeartBeat(unsigned int a1);
    /// 0x086f49a6
    virtual UNK_RET HideLoadingScreen();
    /// 0x086f469a
    virtual UNK_RET IsActive() const;
    /// 0x086f478e
    virtual UNK_RET IsAutoCachingEnabled();
    /// 0x086f46d2
    virtual UNK_RET IsAutoRun() const;
    /// 0x086f46b4
    virtual UNK_RET IsDone() const;
    /// 0x086f46d6
    virtual UNK_RET IsExitingToMenus() const;
    /// 0x086f46fa
    virtual UNK_RET IsExitingToSplash() const;
    /// 0x086f47cc
    virtual UNK_RET IsFogged(int a1, int a2) const;
    /// 0x086f4628
    virtual UNK_RET IsFullScreen() const;
    /// 0x086f4994
    virtual UNK_RET RegisterTextkeyCallbacks(FTextKeyModifier* a1) const;
    /// 0x086f4886
    virtual UNK_RET RegisterUserVolumes();
    /// 0x086f499c
    virtual UNK_RET RemoveProfileEntityType(char const* a1);
    /// 0x086f4876
    virtual UNK_RET RenderInterfaceDebug() const;
    /// 0x086f478c
    virtual UNK_RET SetAutoCachingEnabled(bool a1);
    /// 0x086f480e
    virtual UNK_RET ShowErrorMessage(FStringA const& a1, FStringA const& a2, bool a3);
    /// 0x086f4880
    virtual UNK_RET TunerSendState() const;
    /// 0x086f4644
    virtual UNK_RET UpdateApp(float a1);
    /// 0x086f49a8
    virtual UNK_RET UpdateLoadingScreen();
    /// 0x086f47d4
    virtual UNK_RET UpdateNetwork(float a1) const;
    /// 0x086f49b0
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x086f4796
    virtual UNK_RET IsDebugCameraEnabled() const;
    /// 0x086f479a
    virtual UNK_RET SetCameraMode(int a1);
    /// 0x086f479c
    virtual UNK_RET IsPlotVisible(NiPoint3 const& a1) const;
    /// 0x086f47d0
    virtual UNK_RET GetFogMask(int a1, int a2, int* a3, int* a4) const;
    /// 0x086f4866
    virtual UNK_RET IsEntityMovingToATile(FEntity const* a1) const;
    /// 0x086f486a
    virtual UNK_RET IsInCombat() const;
    /// 0x086f486e
    virtual UNK_RET IsCombatAnim(int a1) const;
    /// 0x086f4872
    virtual UNK_RET ExplicitEntUpdate(FEntity* a1, float a2, bool a3);
    /// 0x086f4874
    virtual UNK_RET RenderDebug(NiCamera* a1) const;
    /// 0x086f4996
    virtual UNK_RET ProcessMarkeredSound(FStringA& a1, NiAVObject* a2, FGrannyAssetInstance const* a3) const;
    /// 0x086f499e
    virtual UNK_RET LoadTerrainPAK();
    /// 0x086f49a4
    virtual UNK_RET ShowLoadingScreen(int a1, bool a2);
    /// 0x086f49aa
    virtual UNK_RET IsTrophyRoomEnabled() const;
};
