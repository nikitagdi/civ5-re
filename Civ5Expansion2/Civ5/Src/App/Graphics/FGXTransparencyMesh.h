/// origin guess
#pragma once
#include <d3d9.h>

class FGXVector4;
namespace FireGrafix {
class FGXModelNode;
}

/// size ?
class FGXTransparencyMesh {
public:
    class Vertex;
    /// 0x08a4115c
    FGXTransparencyMesh();
    /// 0x08a41184
    UNK_RET Init(FGXTransparencyMesh::Vertex* a1, unsigned char* a2, unsigned int a3, unsigned int a4, unsigned int a5);
    /// 0x08a411b2
    UNK_RET TransformVertices(FireGrafix::FGXModelNode const& a1, D3DXMATRIX const& a2, void const* a3, FGXVector4* a4) const;
};
