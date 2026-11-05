/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"

class TextInfoLayerBase;

/// size ?
/// vtable 0x0a2ffb60
class TextInfoLayerManager {
public:
    /// 0x089dc558
    TextInfoLayerManager();
    /// 0x089dc5c0
    /// deleting 0x089dc58a
    virtual ~TextInfoLayerManager();
    /// 0x089dc5f0
    UNK_RET DeleteAllLayers();
    /// 0x089dc63e
    UNK_RET AddLayer(TextInfoLayerBase* a1);
    /// 0x089dc678
    UNK_RET PopulateLayer(int a1);
    /// 0x089dc6a8
    UNK_RET ActivateLayer(int a1);
    /// 0x089dc700
    UNK_RET DeactivateLayer(int a1);
    /// 0x089dc744
    UNK_RET ClearLayer(int a1);
    /// 0x089dc784
    UNK_RET SetPlayerForLayer(int a1, PlayerTypes a2);
};
