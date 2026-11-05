/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FForkPSystemBase.h"
#include <d3d9.h>
#include <forkparticles.h>

class FForkPEffect;
class FGXVector3;
class _t_frkmatrix4;

/// size ?
/// vtable 0x0a301360
class FForkPSystem : public FForkPSystemBase {
public:
    class FForkStats;
    class ForkEffect_Init;
    class ForkEffect_Release;
    class ForkEffect_SetTransform;
    class ForkEffect_SetVisibility;
    /// size ?
    class FForkStats {
    public:
        /// 0x08cb203c
        FForkStats();
    };
    /// size ?
    class ForkEffect_Init {
    public:
        /// 0x08cb2018
        ForkEffect_Init();
    };
    /// size ?
    class ForkEffect_Release {
    public:
        /// 0x08cb2030
        ForkEffect_Release();
    };
    /// size ?
    class ForkEffect_SetTransform {
    public:
        /// 0x08cb2000
        ForkEffect_SetTransform();
    };
    /// size ?
    class ForkEffect_SetVisibility {
    public:
        /// 0x08cb200c
        ForkEffect_SetVisibility(bool a1);
    };
    /// 0x08cb2064
    FForkPSystem();
    /// 0x08cb22d8
    /// deleting 0x08cb22b0
    virtual ~FForkPSystem();
    /// 0x08cb35e6
    virtual UNK_RET LoadTexture(char const* a1, bool a2);
    /// 0x08cb371c
    virtual UNK_RET UnloadTexture(char const* a1);
    /// pure
    virtual UNK_RET vf_08(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_09(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_10(UNK_PARAMS) = 0;
    /// 0x08cb23b8
    UNK_RET Startup(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6);
    /// 0x08cb25b2
    UNK_RET Shutdown();
    /// 0x08cb2732
    UNK_RET DestroyAllForkEffects();
    /// 0x08cb27e4
    UNK_RET BeginProcess();
    /// 0x08cb281c
    UNK_RET Process(float a1);
    /// 0x08cb28dc
    UNK_RET CaptureStats();
    /// 0x08cb293e
    UNK_RET EraseDeadForkEmitters();
    /// 0x08cb29ca
    UNK_RET DestroyDeadForkEffects();
    /// 0x08cb2aa0
    UNK_RET EndProcess();
    /// 0x08cb2ac4
    UNK_RET LoadForkPSB(char const* a1, unsigned int a2);
    /// 0x08cb2b8c
    UNK_RET IsValidForkPSBHandle(unsigned int a1) const;
    /// 0x08cb2bd8
    UNK_RET UnloadForkPSB(unsigned int a1);
    /// 0x08cb2c60
    UNK_RET DestroyForkEffectsByPSBHandle(unsigned int a1);
    /// 0x08cb2d3e
    UNK_RET ReloadForkPSB(char const* a1, unsigned int a2);
    /// 0x08cb2dfc
    UNK_RET DestroyForkEffect(FForkPEffect& a1);
    /// 0x08cb2e6e
    UNK_RET SetActiveForkPSystem(unsigned int a1);
    /// 0x08cb2f3a
    UNK_RET DestroyAllActiveForkEffects();
    /// 0x08cb3018
    UNK_RET SetViewMatrixAt(unsigned int a1, D3DXMATRIX const& a2);
    /// 0x08cb304c
    UNK_RET SetProjMatrix(D3DXMATRIX const& a1);
    /// 0x08cb3064
    UNK_RET SetCallbackUserData(void* a1);
    /// 0x08cb3070
    UNK_RET SetForkEmitterViewMatrixIndexCallback(unsigned int (*a1)(void*, FGXVector3 const*));
    /// 0x08cb3094
    UNK_RET SetForkEffectHandleDestroyedCallback(void (*a1)(void*, unsigned int));
    /// 0x08cb30b8
    UNK_RET IsValidForkEffectHandle(unsigned int a1) const;
    /// 0x08cb30e6
    UNK_RET GetForkPSBHandleEmitterCount(unsigned int a1) const;
    /// 0x08cb3134
    UNK_RET CanCreateForkEmitters(unsigned int a1) const;
    /// 0x08cb315c
    UNK_RET CanCreateForkEffects(unsigned int a1) const;
    /// 0x08cb3184
    UNK_RET InitHandle(unsigned int& a1, FForkPSystem::ForkEffect_Init const& a2);
    /// 0x08cb346c
    UNK_RET SetupEffectCallbacks(_t_frkpeffect* a1);
    /// 0x08cb34da
    UNK_RET ReleaseHandle(unsigned int a1, FForkPSystem::ForkEffect_Release const& a2);
    /// 0x08cb3548
    UNK_RET UpdateFromHandle(unsigned int a1, FForkPSystem::ForkEffect_SetTransform const& a2);
    /// 0x08cb35b6
    UNK_RET UpdateFromHandle(unsigned int a1, FForkPSystem::ForkEffect_SetVisibility const& a2);
    /// 0x08cb37d0
    UNK_RET OnEmitterProcess(_t_frkpemitter* a1, float a2);
    /// 0x08cb38b6
    UNK_RET OnEmitterDestroy(_t_frkpemitter* a1);
    /// 0x08cb3934
    UNK_RET OnSuperEmit(_t_frkpemitter** a1, void** a2, _t_frkpsystem** a3, char* a4, _t_frkmatrix4* a5, _t_frkpemitterinfo* a6, _t_frkpemitter* a7);
    /// 0x08cb3aaa
    UNK_RET SubmitEmitterParticles(_t_frkpemitter* a1);
};
