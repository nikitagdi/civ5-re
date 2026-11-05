/// origin guess
#pragma once

class FGrannyFileRefObject;
template <class T0> class FTPointer;

/// size ?
class FGrannyFileAndName {
public:
    /// 0x08cc14e0
    FGrannyFileAndName(FTPointer<FGrannyFileRefObject> a1, char const* a2);
    /// 0x08cc1576
    ~FGrannyFileAndName();
    /// 0x0d268140
    static UNK_DATA ms_kStringPalette[0x30];
};

extern "C" {
    /// 0x0d268194
    extern UNK_DATA g_apkGrannyVertexFormats[0xe800];
    /// 0x0d268174
    extern UNK_DATA g_bGrannyVertexInitialization[0x1d];
}

namespace FGranny {
/// 0x0d268170
extern UNK_DATA ms_pkWOffsetLoader[0x4];
}
