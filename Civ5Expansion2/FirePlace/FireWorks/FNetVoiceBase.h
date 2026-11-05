/// origin guess
#pragma once

class FNetPlatformDetailsSteam;
class NetVoice;

/// size ?
/// vtable 0x0a305480
class FNetVoiceBase {
public:
    /// 0x08d79bd4
    FNetVoiceBase(int a1);
    /// 0x08d7a004
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08d7a006
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// pure
    virtual UNK_RET vf_02(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_03(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_04(UNK_PARAMS) = 0;
    /// 0x08d7a01a
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// pure
    virtual UNK_RET vf_06(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_07(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_08(UNK_PARAMS) = 0;
    /// 0x08d7a01c
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x08d7a020
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x08d79eaa
    virtual UNK_RET ToggleCaptureDevice();
    /// 0x08d7a022
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x08d7a026
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x08d7a02a
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x08d79ee6
    virtual UNK_RET TogglePlaybackDevice();
    /// 0x08d7a02c
    virtual UNK_RET vf_16(UNK_PARAMS);
    /// 0x08d7a030
    virtual UNK_RET vf_17(UNK_PARAMS);
    /// 0x08d7a032
    virtual UNK_RET vf_18(UNK_PARAMS);
    /// 0x08d7a036
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x08d7a038
    virtual UNK_RET vf_20(UNK_PARAMS);
    /// 0x08d79c24
    virtual UNK_RET AddTalker(int a1, FNetPlatformDetailsSteam const* a2);
    /// 0x08d79c74
    virtual UNK_RET RemoveTalker(int a1, FNetPlatformDetailsSteam const* a2);
    /// 0x08d7a03c
    virtual UNK_RET vf_23(UNK_PARAMS);
    /// 0x08d7a040
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x08d7a046
    virtual UNK_RET vf_25(UNK_PARAMS);
    /// 0x08d79c66
    UNK_RET AddTalkerIndex(int a1);
    /// 0x08d79ca0
    UNK_RET RemoveTalkerIndex(int a1);
    /// 0x08d79cc6
    UNK_RET SetMute(bool a1, int a2);
    /// 0x08d79e10
    UNK_RET GetMute(int a1) const;
    /// 0x08d79e1e
    UNK_RET GetSession() const;
    /// 0x08d79e4e
    UNK_RET AnyMuted() const;
    /// 0x08d79e6a
    UNK_RET SetSolo(bool a1, int a2);
    /// 0x08d79e94
    UNK_RET GetSolo(int a1) const;
    /// 0x08d79f22
    UNK_RET SendVoiceMessage(NetVoice& a1) const;
};

namespace FVoiceFactory {
/// 0x08d7a000
UNK_RET CreateVoiceManager(int a1);
}
