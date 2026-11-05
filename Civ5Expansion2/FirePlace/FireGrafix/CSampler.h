/// origin guess
#pragma once
#include <d3d9.h>

namespace FireGrafix {
/// size ?
class CSampler {
public:
    /// 0x08d13e28
    CSampler(_D3DTEXTUREADDRESS a1, _D3DTEXTUREADDRESS a2, _D3DTEXTUREADDRESS a3, _D3DTEXTUREFILTERTYPE a4, _D3DTEXTUREFILTERTYPE a5, _D3DTEXTUREFILTERTYPE a6, unsigned int a7, bool a8);
    /// 0x08d13e60
    UNK_RET Apply(unsigned int a1);
};
}
