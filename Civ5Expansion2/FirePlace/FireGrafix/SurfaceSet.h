/// origin guess
#pragma once

namespace FireGrafix {
/// size ?
class SurfaceSet {
public:
    /// 0x08cf2eb2
    SurfaceSet(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08cf2f9c
    ~SurfaceSet();
    /// 0x08cf2f6e
    UNK_RET Flush();
    /// 0x08cf2fe6
    UNK_RET DebugCheckAddSurface(unsigned int a1, unsigned int a2, unsigned short a3);
    /// 0x08cf2fec
    UNK_RET DebugCheckAddSurfaceDataOffset(unsigned int a1, unsigned int a2, unsigned short a3, unsigned int a4);
    /// 0x08cf3000
    UNK_RET StateSort();
    /// 0x08cf303c
    UNK_RET RestrictedStateSort();
};
}
