/// origin guess
#pragma once
#include <d3d9.h>

class FGXVector3;

/// size ?
class FGXViewFrustum {
public:
    /// 0x08d1491c
    UNK_RET ComputeCorners(FGXVector3* a1) const;
    /// 0x08d14a54
    UNK_RET SetFromD3DMatrix(D3DXMATRIX const& a1, bool a2);
    /// 0x08d14be8
    UNK_RET CullConvexPolygonExact(FGXVector3 const* a1, unsigned int a2) const;
};
