/// origin guess
#pragma once
#include <d3d9.h>
#include <forkparticles.h>

/// size ?
class FForkPEffect {
public:
    /// 0x08ce8f38
    FForkPEffect();
    /// 0x08ce8f5e
    ~FForkPEffect();
    /// 0x08ce8f60
    UNK_RET Init(_t_frkpeffect* a1, unsigned int a2, unsigned int a3, unsigned int a4, bool a5);
    /// 0x08ce8f9a
    UNK_RET Release();
    /// 0x08ce8fbe
    UNK_RET OnEmitterDestroy();
    /// 0x08ce8fce
    UNK_RET SetEffectLife(_t_frkpeffect* a1, float a2);
    /// 0x08ce9020
    UNK_RET ModifyEffectLife(_t_frkpeffect* a1, float a2);
    /// 0x08ce9088
    UNK_RET ModifyEffectLifeByDefaultProperty(_t_frkpeffect* a1);
    /// 0x08ce914a
    UNK_RET KillEffectParticles(_t_frkpeffect* a1);
    /// 0x08ce9198
    UNK_RET SetEffectTransform(_t_frkpeffect* a1, D3DXMATRIX const& a2, float a3);
};
