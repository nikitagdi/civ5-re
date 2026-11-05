/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/TextInfoLayerBase.h"

/// size ?
/// vtable 0x0a2ffb40
class GameCoreHexDebugLayer : public TextInfoLayerBase {
public:
    /// 0x089dc35e
    GameCoreHexDebugLayer(char const* a1);
    /// 0x089dc81a
    /// deleting 0x089dc850
    virtual ~GameCoreHexDebugLayer();
    /// 0x089dc44a
    virtual UNK_RET PopulateHex(ICvPlot1* a1);
};
