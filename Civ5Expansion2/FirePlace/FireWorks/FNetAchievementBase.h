/// origin guess
#pragma once

/// size ?
/// vtable 0x0a305090
class FNetAchievementBase {
public:
    /// 0x08d6f340
    FNetAchievementBase();
    /// 0x08d6f368
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08d6f36a
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08d6f37e
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08d6f384
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08d6f38a
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x08d6f390
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x08d6f394
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08d6f39a
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08d6f3a0
    virtual UNK_RET vf_08(UNK_PARAMS);
};

namespace FAchievementFactory {
/// 0x08d6f364
UNK_RET CreateAchievementManager();
}
