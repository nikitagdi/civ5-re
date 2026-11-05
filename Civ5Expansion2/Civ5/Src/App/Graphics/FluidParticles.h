/// origin guess
#pragma once
#include <d3d9.h>

class FGXInt2;
class FGXVector2;
class FGXVector3;
class FluidParticlePack;
class _TAL_ID;
namespace FireGrafix {
class CCommandStream;
class TextureData;
}

/// size ?
class FluidParticles {
public:
    /// 0x0886cdac
    FluidParticles();
    /// 0x0886cf7e
    ~FluidParticles();
    /// 0x0886cfca
    UNK_RET Shutdown();
    /// 0x0886d134
    UNK_RET Startup(unsigned int a1, FireGrafix::TextureData* a2, bool a3);
    /// 0x0886d58c
    UNK_RET StartupGame(FGXInt2& a1, FGXInt2& a2, int a3);
    /// 0x0886d7dc
    UNK_RET ShutdownGame();
    /// 0x0886d8a0
    UNK_RET AddParticle(FGXVector3& a1, FGXVector2& a2, float a3, float a4, unsigned int a5);
    /// 0x0886d95c
    UNK_RET ComputeDensitySSE2(FluidParticlePack* a1);
    /// 0x0886db0a
    UNK_RET JustAdd(FluidParticlePack* a1, unsigned int a2);
    /// 0x0886dc1c
    UNK_RET ComputeForcesSSE2(FluidParticlePack* a1, unsigned int a2, FGXVector2 const& a3, FGXVector2 const& a4);
    /// 0x0886e09a
    UNK_RET PackParticles(FGXInt2 const& a1, FGXInt2 const& a2, D3DXMATRIX* a3, _TAL_ID* a4);
    /// 0x0886e1d4
    UNK_RET PackAllParticles();
    /// 0x0886e324
    UNK_RET DoSimulation(FGXVector3& a1, FGXVector3& a2, D3DXMATRIX* a3);
    /// 0x0886e784
    UNK_RET LaunchJobs(FGXVector3& a1, FGXVector3& a2, D3DXMATRIX* a3);
    /// 0x0886e8fe
    UNK_RET Render(FireGrafix::CCommandStream* a1);

    unsigned char unk_0[0x840];     ///< +0x0
    // Simulation settings that FluidFOWSystem::UpdateSettings copies in from its own settings.
    int m_iIterations;              ///< +0x840
    int m_iPadding;                 ///< +0x844
    unsigned char unk_848[0x28];    ///< +0x848
    float m_fSpeed;                 ///< +0x870
    float m_fDrag;                  ///< +0x874
    float m_fMasterSize;            ///< +0x878
    unsigned char unk_87c[0xc];     ///< +0x87c
    float m_fDensity;               ///< +0x888
    float m_fForceStrength;         ///< +0x88c
    float m_fMaxRotate;             ///< +0x890
    unsigned char unk_894[0x64];    ///< +0x894
    // Zeroed by the constructor; FluidFOWSystem::StartupGame scales a random number by it for the particle id.
    int field_8f8;                  ///< +0x8f8
};

/// 0x0886d931
UNK_RET ParticleSortY(void* a1, void const* a2, void const* a3);
/// 0x0886e174
UNK_RET ParticleWSort(void* a1, void const* a2, void const* a3);
/// 0x0886e1a6
UNK_RET ParticleCreateIndexSort(void* a1, void const* a2, void const* a3);
/// 0x0886e8b8
UNK_RET SortSimilarParticle(void const* a1, void const* a2);
