/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include <granny.h>
#include "FirePlace/FireGrafix/FGXVector3.h"

class FGXVector2;
class Material;
class MaterialDesc;
class TempForestStencilVert;
namespace FireGrafix {
template <int T0> class VStruct;
}

// One model of the library: runs of the source vertex and index arrays plus its bounds. Y is the model's length axis.
/// size 0x28
class Geometry {
public:
    unsigned int m_uiVertexStart;   ///< +0x0
    unsigned int m_uiVertexCount;   ///< +0x4
    unsigned int m_uiIndexStart;    ///< +0x8
    unsigned int m_uiTriangleCount; ///< +0xc
    FGXVector3 m_vMin;              ///< +0x10
    FGXVector3 m_vMax;              ///< +0x1c
};

// Source models are loaded once into m_pSourceVertices/m_pSourceIndices; the Add* methods append transformed copies to the output arrays.
/// size ?
class cvBaseDynamicGeometryLibrary {
public:
    FireGrafix::VStruct<1>* m_pVertices;      ///< +0x0
    unsigned int m_uiNumVertices;             ///< +0x4
    char unk_8[4];                            ///< +0x8
    TempForestStencilVert* m_pDecalVertices;  ///< +0xc
    unsigned int m_uiNumDecalVertices;        ///< +0x10
    unsigned int m_uiMaxDecalVertices;        ///< +0x14
    unsigned short* m_pDecalIndices;          ///< +0x18
    unsigned int m_uiNumDecalIndices;         ///< +0x1c
    char unk_20[4];                           ///< +0x20
    unsigned int* m_puiIndices;               ///< +0x24
    unsigned int m_uiNumIndices;              ///< +0x28
    char unk_2c[0x94];                        ///< +0x2c
    FireGrafix::VStruct<1>* m_pSourceVertices;///< +0xc0
    unsigned int m_uiNumSourceVertices;       ///< +0xc4
    unsigned int m_uiMaxSourceVertices;       ///< +0xc8
    unsigned short* m_pSourceIndices;         ///< +0xcc
    unsigned int m_uiNumSourceIndices;        ///< +0xd0
    unsigned int m_uiMaxSourceIndices;        ///< +0xd4


    /// 0x08a156d4
    UNK_RET InitFromFiles();
    /// 0x08a156da
    UNK_RET InitFromBinary(FDataStream& a1);
    /// 0x08a157a4
    UNK_RET SaveToBinary(FDataStream& a1) const;
    /// 0x08a15836
    UNK_RET Release();
    /// 0x08a15850
    UNK_RET LoadMaterialFromGranny(granny_file* a1, unsigned int a2, MaterialDesc const& a3, Material* a4) const;
    /// 0x08a15bb2
    UNK_RET LoadMaterialFromGranny(granny_file* a1, char const* a2, MaterialDesc const& a3, Material* a4) const;
    /// 0x08a15c36
    UNK_RET LoadGeometryFromGranny(granny_file* a1, unsigned int a2, Geometry* a3);
    /// 0x08a15e7c
    UNK_RET LoadGeometryFromGranny(granny_file* a1, char const* a2, Geometry* a3, bool a4);
    /// 0x08a15f98
    UNK_RET ReCenterGeometryInX(Geometry& a1);
    /// 0x08a16002
    UNK_RET ReScaleToMatchBoundsY(Geometry& a1, FGXVector3 const& a2, FGXVector3 const& a3);
    /// 0x08a160b6
    UNK_RET SnapToBoundsY(Geometry& a1, float a2);
    /// 0x08a16134
    UNK_RET AddModel(Geometry const& a1, FGXVector3 const& a2, float a3, float a4);
    /// 0x08a16372
    UNK_RET AddDecal(Geometry const& a1, FGXVector3 const& a2, float a3, float a4);
    /// 0x08a165a6
    UNK_RET AddSplineModel(Geometry const* pVariants, unsigned int uiNumVariants, FGXVector3 const* pPoints, unsigned int uiNumPoints, float fScale);
    /// 0x08a16cec
    void AddSplineDecal(Geometry const* pVariants, unsigned int uiNumVariants, FGXVector2 const* pPoints, unsigned int uiNumPoints, float fScale);
    /// 0x08a1730e
    void AddSplineModelWithBookends(Geometry const& first, Geometry const& middle, Geometry const& last, FGXVector3 const* pPoints, unsigned int uiNumPoints, float fScale);
};

namespace cvDynamicGeometryLibrary {
/// 0x08a17998
UNK_RET Load(bool a1);
}
