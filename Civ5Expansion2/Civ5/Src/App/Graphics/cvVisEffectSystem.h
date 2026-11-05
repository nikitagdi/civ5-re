/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FJob.h"

class FGXGrannyCompositeContainerInstance;
class FGXVector3;
class FrameGlobalsData;
class HEXVec_i;
class cvVisEffect;
class cvVisEffectInfo;
namespace FireGrafix {
class CCommandStream;
}

/// size ?
class cvVisEffectSystem {
public:
    class VisEffectRenderJob;
    class VisEffect_EnableAutoRelease;
    class VisEffect_Init;
    class VisEffect_SetTransform;
    class cvStats;
    /// size ?
    /// vtable 0x0a2fdab8
    class VisEffectRenderJob : public FJob {
    public:
        /// 0x088cb968
        virtual void Execute(unsigned int a1);
        /// 0x0c056354
        static UNK_DATA sm_pThreadRenderer[0x4];
    };
    class VisEffectThreadRender;
    /// size ?
    class VisEffect_EnableAutoRelease {
    public:
        /// 0x088c6c78
        VisEffect_EnableAutoRelease(bool a1);
    };
    /// size ?
    class VisEffect_Init {
    public:
        /// 0x088c6c64
        VisEffect_Init();
    };
    class VisEffect_Release;
    /// size ?
    class VisEffect_SetTransform {
    public:
        /// 0x088c6c58
        VisEffect_SetTransform();
    };
    /// size ?
    class cvStats {
    public:
        /// 0x088c6c84
        cvStats();
    };
    /// 0x088c6cac
    cvVisEffectSystem();
    /// 0x088c7218
    ~cvVisEffectSystem();
    /// 0x088c72e0
    UNK_RET Startup(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6);
    /// 0x088c7c1e
    UNK_RET StartupAndLoadParticleEffectsFromXml(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x088c81b2
    UNK_RET OnGetForkEmitterViewMatrixIndex(void* a1, FGXVector3 const* a2);
    /// 0x088c81fc
    UNK_RET OnForkEffectHandleDestroyed(void* a1, unsigned int a2);
    /// 0x088c81fe
    UNK_RET LoadVisEffectInfosFromXml();
    /// 0x088c86ea
    UNK_RET Shutdown();
    /// 0x088c8862
    UNK_RET DestroyAllVisEffects();
    /// 0x088c8940
    UNK_RET StartupGame();
    /// 0x088c89b8
    UNK_RET ShutdownGame();
    /// 0x088c89ea
    UNK_RET GetActiveSystemState() const;
    /// 0x088c89fc
    UNK_RET GetActiveFrameGlobals() const;
    /// 0x088c8a0e
    UNK_RET GetActiveMapState() const;
    /// 0x088c8a28
    UNK_RET BeginProcess();
    /// 0x088c8a50
    UNK_RET Process(float a1);
    /// 0x088c8c0e
    UNK_RET UpdateActiveMapStateCulling();
    /// 0x088c8e28
    UNK_RET DestroyVisEffect(cvVisEffect& a1);
    /// 0x088c8f44
    UNK_RET EndProcess();
    /// 0x088c8f6c
    UNK_RET UpdateGlobals(int a1);
    /// 0x088c94ca
    UNK_RET OnExternalJob(FJob* a1);
    /// 0x088c9510
    UNK_RET OnRenderView(unsigned int a1, unsigned short a2);
    /// 0x088c95d6
    UNK_RET OnRenderShadow(unsigned int a1);
    /// 0x088c965e
    UNK_RET Render();
    /// 0x088c9842
    UNK_RET DrawVisEffect(cvVisEffectSystem::VisEffectThreadRender& a1, cvVisEffect& a2, FGXGrannyCompositeContainerInstance& a3, float a4, float a5, unsigned int a6, unsigned int a7);
    /// 0x088c9c58
    UNK_RET RenderMT();
    /// 0x088c9e64
    UNK_RET Render(unsigned int a1, unsigned int a2, FireGrafix::CCommandStream* a3, unsigned int a4, unsigned short a5);
    /// 0x088ca064
    UNK_RET SetActiveVisEffectSystem(unsigned int a1);
    /// 0x088ca148
    UNK_RET DestroyAllActiveVisEffects();
    /// 0x088ca1fc
    UNK_RET SetActiveFrameGlobals(FrameGlobalsData const& a1);
    /// 0x088ca2c6
    UNK_RET SetAllActiveHexStates(unsigned char a1);
    /// 0x088ca328
    UNK_RET SetActiveHexState(HEXVec_i const& a1, unsigned char a2);
    /// 0x088ca3a4
    UNK_RET SetCallbackUserData(void* a1);
    /// 0x088ca3b0
    UNK_RET SetVisEffectHandleDestroyedCallback(void (*a1)(void*, unsigned int));
    /// 0x088ca3d2
    UNK_RET IsValidVisEffectInfoHandle(unsigned int a1) const;
    /// 0x088ca41e
    UNK_RET IsValidVisEffectHandle(unsigned int a1) const;
    /// 0x088ca44c
    UNK_RET GetAllVisEffectInfoHandles(FFastVector<unsigned int, true, 1u, 52u, BaseVector<unsigned int, true>::FDefaultFastVectorAllocator>& a1);
    /// 0x088ca4a8
    UNK_RET GetVisEffectInfoName(unsigned int a1);
    /// 0x088ca4d4
    UNK_RET GetVisEffectInfo(unsigned int a1);
    /// 0x088ca544
    UNK_RET InitHandle(unsigned int& a1, cvVisEffectSystem::VisEffect_Init const& a2);
    /// 0x088cac26
    UNK_RET CanCreateVisEffect(cvVisEffectInfo const& a1) const;
    /// 0x088cacfe
    UNK_RET UpdateFromHandle(unsigned int a1, cvVisEffectSystem::VisEffect_SetTransform const& a2);
    /// 0x088cb028
    UNK_RET UpdateFromHandle(unsigned int a1, cvVisEffectSystem::VisEffect_EnableAutoRelease const& a2);
    /// 0x088cb058
    UNK_RET ReleaseHandle(unsigned int a1, cvVisEffectSystem::VisEffect_Release const& a2);
    /// 0x088cb0ea
    UNK_RET GetStats();
    /// 0x088cb128
    UNK_RET ReloadVisEffectInfos();
    /// 0x088cb158
    UNK_RET ReloadParticleEffectsFromXml();
    /// 0x088cb558
    UNK_RET ReloadVisEffectInfosFromXml();
    /// 0x088cb8bc
    UNK_RET OnVisEffectInfoLoaded(cvVisEffectInfo& a1);
};
