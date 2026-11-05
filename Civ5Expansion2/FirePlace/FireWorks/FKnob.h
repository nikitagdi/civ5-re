/// origin guess
#pragma once

class FKnob;

/// size ?
class FKnob {
public:
    /// 0x08d2e7f0
    FKnob();
    /// 0x08d2e854
    FKnob(float a1, bool a2);
    /// 0x08d2e902
    ~FKnob();
    /// 0x08d2e822
    UNK_RET Clear();
    /// 0x08d2e8cc
    UNK_RET SetVolume(float a1);
    /// 0x08d2e904
    UNK_RET GetAllVolumes() const;
    /// 0x08d2e964
    UNK_RET SetDisabled(bool a1);
    /// 0x08d2e970
    UNK_RET GetDisabled() const;
    /// 0x08d2e97a
    UNK_RET GetAllDisabled() const;
    /// 0x08d2e9ac
    UNK_RET GetAllLowPassCutoffLevels() const;
    /// 0x08d2ea06
    UNK_RET SetParentKnob(FKnob* a1);
    /// 0x08d2ea12
    UNK_RET FadeToNewVolume(float a1, float a2);
    /// 0x08d2ea84
    UNK_RET Update(float a1);
    /// 0x08d2eb12
    UNK_RET GetFadeTargetVolume();
};
