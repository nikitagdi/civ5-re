/// origin guess
#pragma once

class FSoundBase;
enum EFadeState : int;

/// size ?
class FFadeSound {
public:
    /// 0x08d40130
    FFadeSound();
    /// 0x08d4016c
    UNK_RET Clear();
    /// 0x08d401a8
    UNK_RET Update(float a1);
    /// 0x08d40288
    UNK_RET GetState();
    /// 0x08d40290
    UNK_RET SetState(EFadeState a1);
    /// 0x08d4029c
    UNK_RET SetFade(FSoundBase* a1, EFadeState a2, float a3, float a4, float a5, float a6, float a7);
};
