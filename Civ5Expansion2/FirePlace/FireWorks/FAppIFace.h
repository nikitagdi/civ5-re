/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FEntity;
class FTextKeyModifier;

/// size ?
/// vtable 0x0a301820
class FAppIFace {
public:
    /// 0x08cbb1e0
    FAppIFace();
    /// 0x086f49ae
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cbb1fc
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// pure
    virtual UNK_RET GetHandle() const = 0;
    /// pure
    virtual UNK_RET GetHInstance() const = 0;
    /// pure
    virtual UNK_RET IsFullScreen() const = 0;
    /// pure
    virtual UNK_RET UpdateApp(float a1) = 0;
    /// pure
    virtual UNK_RET GetWindowWidth() const = 0;
    /// pure
    virtual UNK_RET GetWindowHeight() const = 0;
    /// pure
    virtual UNK_RET IsActive() const = 0;
    /// pure
    virtual UNK_RET IsDone() const = 0;
    /// 0x08cbb210
    virtual UNK_RET IsExitingToMenus() const;
    /// 0x08cbb214
    virtual UNK_RET IsExitingToSplash() const;
    /// 0x08cbb218
    virtual UNK_RET IsAutoRun() const;
    /// 0x08cbb21c
    virtual UNK_RET GetHotLoader() const;
    /// 0x08cbb220
    virtual UNK_RET GetBGLoader() const;
    /// 0x086f49c4
    virtual UNK_RET vf_15(UNK_PARAMS);
    /// 0x08cbb224
    virtual UNK_RET SetAutoCachingEnabled(bool a1);
    /// 0x08cbb226
    virtual UNK_RET IsAutoCachingEnabled();
    /// pure
    virtual UNK_RET HeartBeat(unsigned int a1) = 0;
    /// pure
    virtual UNK_RET GetRandomS() = 0;
    /// pure
    virtual UNK_RET GetRandomA() = 0;
    /// pure
    virtual UNK_RET GetRandomMT() = 0;
    /// pure
    virtual UNK_RET GetGlobalDefines() = 0;
    /// pure
    virtual UNK_RET GetMapSeed() = 0;
    /// 0x08cbb22a
    virtual UNK_RET GetGameSeed();
    /// pure
    virtual UNK_RET GetCurrentCamera() const = 0;
    /// 0x086f49c6
    virtual UNK_RET vf_26(UNK_PARAMS);
    /// pure
    virtual UNK_RET GetMapDimensions(int& a1, int& a2, int& a3, int& a4) const = 0;
    /// 0x08cbb22e
    virtual UNK_RET IsFogged(int a1, int a2) const;
    /// pure
    virtual UNK_RET GetInput() const = 0;
    /// 0x08cbb232
    virtual UNK_RET UpdateNetwork(float a1) const;
    /// 0x086f49c8
    virtual UNK_RET vf_31(UNK_PARAMS);
    /// 0x08cbb234
    virtual UNK_RET HasAcceptedAnInvite() const;
    /// 0x08cbb238
    virtual UNK_RET ShowErrorMessage(FStringA const& a1, FStringA const& a2, bool a3);
    /// 0x08cbb23a
    virtual UNK_RET RenderInterfaceDebug() const;
    /// 0x08cbb23c
    virtual UNK_RET GetTuneMgr() const;
    /// 0x08cbb240
    virtual UNK_RET GetTunerVariableSystem() const;
    /// 0x08cbb244
    virtual UNK_RET TunerSendState() const;
    /// 0x08cbb246
    virtual UNK_RET RegisterTextkeyCallbacks(FTextKeyModifier* a1) const;
    /// 0x086f49cc
    virtual UNK_RET vf_39(UNK_PARAMS);
    /// 0x08cbb248
    virtual UNK_RET AllocNewAudioComponent(FEntity* a1) const;
    /// 0x08cbb24c
    virtual UNK_RET RegisterUserVolumes();
    /// 0x08cbb24e
    virtual UNK_RET CreateAudioManager();
    /// 0x08cbb250
    virtual UNK_RET AddProfileEntityType(char const* a1);
    /// 0x08cbb252
    virtual UNK_RET RemoveProfileEntityType(char const* a1);
    /// 0x086f49d0
    virtual UNK_RET vf_45(UNK_PARAMS);
    /// 0x08cbb254
    virtual UNK_RET HideLoadingScreen();
    /// 0x08cbb256
    virtual UNK_RET UpdateLoadingScreen();
    /// 0x0d267d18
    static UNK_DATA m_pInst[0x4];
};
