/// origin guess
#pragma once
#include <d3d9.h>

class IDirect3DDevice9;
namespace FireGrafix {
class CZState;
}

namespace FireGrafix {
/// size ?
class CZState {
public:
    /// 0x08d1444c
    UNK_RET InitZ(bool a1, bool a2, _D3DCMPFUNC a3, _D3DCULL a4, float a5);
    /// 0x08d1447e
    UNK_RET InitZStencil(bool a1, bool a2, _D3DCMPFUNC a3, _D3DCULL a4, float a5, unsigned char a6, unsigned char a7, _D3DSTENCILOP a8, _D3DSTENCILOP a9, _D3DSTENCILOP a10, _D3DCMPFUNC a11, _D3DSTENCILOP a12, _D3DSTENCILOP a13, _D3DSTENCILOP a14, _D3DCMPFUNC a15);
    /// 0x08d144f6
    UNK_RET Apply(IDirect3DDevice9* a1);
};
}

namespace FireGrafix {
/// 0x08d146de
UNK_RET CreateDefaultZStates(FireGrafix::CZState* a1);
}
