/// origin guess
#pragma once
#include <d3d9.h>

class IDirect3DDevice9;
namespace FireGrafix {
class CAlphaState;
}

namespace FireGrafix {
/// size ?
class CAlphaState {
public:
    /// 0x08d13fac
    CAlphaState();
    /// 0x08d13ffc
    CAlphaState(bool a1, _D3DBLEND a2, _D3DBLEND a3, _D3DBLENDOP a4, _D3DBLEND a5, _D3DBLEND a6, _D3DBLENDOP a7, unsigned long a8, unsigned long a9);
    /// 0x08d14040
    UNK_RET Apply(IDirect3DDevice9* a1);
};
}

namespace FireGrafix {
/// 0x08d14182
UNK_RET CreateDefaultAlphaStates(FireGrafix::CAlphaState* a1);
}
