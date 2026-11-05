/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/CultureBorders.h"
#include "Civ5/Src/App/Graphics/TradeVisSystem.h"

namespace FireGrafix {
class CCommandStream;
}

// The global g_OverlaySystem (0x0c044478) is passed as this. The members up to m_kTradeVisSystem are byte blocks
// sized by the gaps between the Startup call offsets; their classes carry no size yet.
/// size 0x11d38
class OverlaySystem {
public:
    /// 0x088c5dec
    void Startup(int iFlag);
    /// 0x088c6046
    void Shutdown();
    /// 0x088c60e6
    void StartupGame();
    /// 0x088c61fa
    void ShutdownGame();
    /// 0x088c624c
    void SetupFrameGlobals();
    /// 0x088c659e
    void Render();
    /// 0x088c6a82
    void Update(float fDeltaTime, bool bFlag);

    FireGrafix::CCommandStream* m_pCommandStream;   ///< +0x0
    HexOverlay m_kHexOverlay;                       ///< +0x4
    char m_kCultureBorders[0x11254];                ///< +0x470
    char m_kMovementOverlay[0x5d0];                 ///< +0x116c4
    char m_kHighlightSystem[0x90];                  ///< +0x11c94
    TradeVisSystem m_kTradeVisSystem;               ///< +0x11d24
    unsigned int m_hZState;                         ///< +0x11d28
    unsigned int m_hAlphaState;                     ///< +0x11d2c
    int m_iEnabled;                                 ///< +0x11d30
    bool m_bFlag;                                   ///< +0x11d34
};
