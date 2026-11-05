/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"

/// size ?
/// vtable 0x0a2ffb20
class TextInfoLayerBase {
public:
    /// 0x089dc170
    TextInfoLayerBase();
    /// 0x089dc1ae
    /// deleting 0x089dc19a
    virtual ~TextInfoLayerBase();
    /// 0x089dc1b0
    virtual UNK_RET PopulateLayer();
    /// 0x089dc29a
    virtual UNK_RET SetLayerID(int a1);
    /// pure
    virtual UNK_RET PopulateHex(ICvPlot1* a1) = 0;
    /// 0x089dc2a6
    virtual UNK_RET SetHexString(int a1, int a2, char* a3);
};
