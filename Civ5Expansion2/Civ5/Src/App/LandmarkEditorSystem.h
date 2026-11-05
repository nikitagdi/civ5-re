/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/CultureBorders.h"

class cvTerrainSampler;

/// size ?
class LandmarkEditorSystem {
public:
    /// 0x08b77f2c
    LandmarkEditorSystem();
    /// 0x08b77f2e
    ~LandmarkEditorSystem();
    /// 0x08b77f30
    UNK_RET Startup();
    /// 0x08b77f32
    UNK_RET Shutdown();
};

// Methods take the object as the first stack argument; Startup passes the sampler on to TerrainEditorOverlay.
/// size ?
class TerrainEditorOverlaySystem {
public:
    /// 0x08b77f34
    UNK_RET Startup(cvTerrainSampler const* a1);
    /// 0x08b78008
    UNK_RET Shutdown();
    /// 0x08b78086
    UNK_RET StartupGame();
    /// 0x08b780c6
    UNK_RET ShutdownGame();
    /// 0x08b780fc
    static UNK_RET SetupFrameGlobals();
    /// 0x08b7817e
    UNK_RET Render();
    /// 0x08b78354
    UNK_RET Update(float a1);

    FireGrafix::CCommandStream* m_pCommandStream;  ///< +0x0
    char unk_4[0x2c];                              ///< +0x4
    HexOverlay m_kHexOverlay;                      ///< +0x30
    char unk_49c[0x90];                            ///< +0x49c
    char unk_52c[0x308];                           ///< +0x52c
    int m_bEnabled;                                ///< +0x834
};
