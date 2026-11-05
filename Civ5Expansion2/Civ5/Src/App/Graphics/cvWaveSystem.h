/// origin guess
#pragma once

class FGXVector2;
class FMemTracker;
class FRandom;
class HEXVec_i;
namespace FireGrafix {
class CCommandStream;
class FGXConstantSet;
class SurfaceSet;
template <int T0> class VStruct;
}
namespace GameSpline {
class Segment;
}

// Coastal surf: crest quads along coast-edge splines, drawn by the WaveShader (VS Wave_VS, PS Wave_PS).
/// size 0xb0
class cvWaveSystem {
public:
    // Attributes of the root element of WaveSettings.xml, in this order; the colour is stored linear.
    /// size 0x50
    class WaveSettings {
    public:
        float m_fWavesPerEdge;           ///< +0x0
        unsigned int m_uiRandomSeed;     ///< +0x4
        unsigned int m_uiCrestPagesU;    ///< +0x8
        unsigned int m_uiCrestPagesV;    ///< +0xc
        unsigned int m_uiCrestPagesInUse; ///< +0x10
        float m_fMinSize;                ///< +0x14
        float m_fMaxSize;                ///< +0x18
        float m_fStartDistance;          ///< +0x1c
        float m_fEndDistance;            ///< +0x20
        float m_fCycleTime;              ///< +0x24
        float m_fFadeInStart;            ///< +0x28
        float m_fFadeInEnd;              ///< +0x2c
        float m_fFadeOutStart;           ///< +0x30
        float m_fFadeOutEnd;             ///< +0x34
        float m_fRestartDelayMin;        ///< +0x38
        float m_fRestartDelayMax;        ///< +0x3c
        float m_fColorR;                 ///< +0x40
        float m_fColorG;                 ///< +0x44
        float m_fColorB;                 ///< +0x48
        float m_fCrestAspect;            ///< +0x4c
    };

    // Per-wave shader constant, one float4 each; u/v pick the crest page, m_fTime runs from a negative delay up to CycleTime.
    /// size 0x10
    class WaveInstance {
    public:
        float m_fPageU;  ///< +0x0
        float m_fPageV;  ///< +0x4
        float m_fTime;   ///< +0x8
        float field_c;   ///< +0xc
    };

    // Up to 248 waves drawn as one surface; the bounds are in world units.
    /// size 0x24
    class WaveBatch {
    public:
        unsigned int m_hSurface;       ///< +0x0
        unsigned int m_hVertexBuffer;  ///< +0x4
        unsigned int m_hIndexBuffer;   ///< +0x8
        unsigned int m_uiFirstWave;    ///< +0xc
        unsigned int m_uiWaveCount;    ///< +0x10
        float m_fMinX;                 ///< +0x14
        float m_fMinY;                 ///< +0x18
        float m_fMaxX;                 ///< +0x1c
        float m_fMaxY;                 ///< +0x20
    };

    // Vertex format 25: two per spline sample (inner edge, outer edge), 9 samples per wave.
    /// size 0xc
    class WaveVertex {
    public:
        unsigned short m_usX;      ///< +0x0
        unsigned short m_usY;      ///< +0x2
        unsigned char m_ucNormalX; ///< +0x4
        unsigned char m_ucNormalY; ///< +0x5
        unsigned char m_ucSide;    ///< +0x6
        unsigned char m_ucAlong;   ///< +0x7
        unsigned char m_ucSlot;    ///< +0x8
        unsigned char unk_9[0x3];  ///< +0x9
    };

    /// 0x0893bbf4
    cvWaveSystem();
    /// 0x0893bc42
    UNK_RET SetupMemTracker(FMemTracker* a1);
    /// 0x0893bc76
    UNK_RET Startup();
    /// 0x0893bd36
    static bool ParseSettingsFromXML(char const* pszPath, cvWaveSystem::WaveSettings& kSettings);
    /// 0x0893c4d6
    UNK_RET Shutdown();
    /// 0x0893c570
    UNK_RET CleanupWaves();
    /// 0x0893c668
    UNK_RET StartupGame();
    /// 0x0893c67c
    UNK_RET SpawnWaves();
    /// 0x0893d206
    UNK_RET ShutdownGame();
    /// 0x0893d21a
    UNK_RET Update(float fDeltaTime);
    /// 0x0893d392
    UNK_RET ComputePageUV(unsigned int uiPage, float& fU, float& fV);
    /// 0x0893d418
    UNK_RET Render(unsigned int uiRenderTarget);
    /// 0x0893d8d8
    UNK_RET Reload();
    /// 0x0893da3a
    static UNK_RET GetSplineForEdge(HEXVec_i const& a1, unsigned int a2, unsigned int a3, GameSpline::Segment& a4);
    // The fSortKey argument is not read.
    /// 0x0893dc6a
    UNK_RET CreateWave(unsigned int uiWaveIndex, GameSpline::Segment& kSpline, float fSortKey, FRandom& kRandom, FireGrafix::VStruct<25>* pVertices, unsigned short* pIndices, unsigned int uiFirstVertex);
    /// 0x0893e0de
    UNK_RET SetupWaveBatch(unsigned int a1, unsigned int a2, FireGrafix::VStruct<25>* a3, unsigned short* a4, unsigned int a5, unsigned int a6);
    // vPos is the point at fT in [0, 1] along the spline, vNormal the unit normal there (gradient rotated by -90 degrees).
    /// 0x0893e3e2
    static UNK_RET EvaluateSpline(GameSpline::Segment const& kSpline, float fT, FGXVector2& vPos, FGXVector2& vNormal);
    /// 0x0893e4f2
    static UNK_RET GetSplineSegment(HEXVec_i const& a1, unsigned int a2, GameSpline::Segment& a3);

    WaveSettings m_kSettings;                      ///< +0x0
    // 2 * max(sampler width / grid width, sampler height / grid height), set by SpawnWaves.
    float m_fWorldScale;                           ///< +0x50
    WaveBatch* m_pBatches;                         ///< +0x54
    unsigned int m_uiBatchCount;                   ///< +0x58
    WaveInstance* m_pWaves;                        ///< +0x5c
    unsigned int m_uiWaveCount;                    ///< +0x60
    FireGrafix::SurfaceSet* m_pSurfaceSet;         ///< +0x64
    unsigned int m_hCrestTexture;                  ///< +0x68
    // [0] is the global set itself, [1..3] its copies; one per camera.
    unsigned int m_auGlobalConstSets[4];           ///< +0x6c
    unsigned char unk_7c[0x4];                     ///< +0x7c
    unsigned int m_uShaderPackageInstance;         ///< +0x80
    unsigned char unk_84[0xc];                     ///< +0x84
    FireGrafix::FGXConstantSet* m_pInstanceConsts; ///< +0x90
    unsigned char unk_94[0x18];                    ///< +0x94
    FireGrafix::CCommandStream* m_pCommandStream;  ///< +0xac
};

namespace FireGrafix {
/// 0x0893f624
UNK_RET InitCiv5VertexFormats();
}

// Render calls it once per visible batch: (surface set, batch surface handle, shader package instance, 0, -1).
/// 0x088c53a6
UNK_RET Fun_088c53a6(FireGrafix::SurfaceSet* a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5);
