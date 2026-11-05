/// origin guess
#pragma once

class FGXVector2;
class SplineParameters;
class SquareXWrapGridPolicy;
class cvTerrainSampler;
template <class T0, class T1, unsigned int T2, unsigned int T3> class StaticHexGrid;
namespace FGXMathRect {
class Rect;
}
namespace TerrainIce {
class CrackPolygon;
class DisplacementTexture;
class HexData;
class IceInputGeometry;
class IceMesh;
class IceSettings;
class PolygonGraph;
class UncompressedVertex;
template <class T0, int T1> class HeapVector;
template <class T0, int T1> class TempVector;
}

namespace TerrainIce {
/// size ?
class PolygonGraph {
public:
    class PolyGraphVertex;
    /// 0x08901964
    UNK_RET CompactDeletedEdges();
    /// 0x089019d8
    UNK_RET BuildVertexAdjacency();
    /// 0x08901b40
    UNK_RET SortAdjacencyTable(TerrainIce::PolygonGraph::PolyGraphVertex& a1, unsigned int a2);
    /// 0x08901bac
    UNK_RET DeleteUnreferencedVertices();
    /// 0x08901d48
    UNK_RET Preallocate(unsigned int a1, unsigned int a2);
    /// 0x08901d92
    UNK_RET ComputeBoundingRect(FGXVector2& a1, FGXVector2& a2);
    /// 0x08901e14
    UNK_RET LabelCycleEdges(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08901e6c
    UNK_RET FindNextEdge(unsigned int a1, unsigned int a2);
    /// 0x08901ec8
    UNK_RET DeletePolygonEdges(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08902060
    UNK_RET GetPolygonVertices(unsigned int a1, unsigned int a2, TerrainIce::TempVector<FGXVector2, 14>& a3);
    /// 0x089020be
    UNK_RET GetPolygonInfo(TerrainIce::HeapVector<TerrainIce::CrackPolygon, 8>& a1);
};
}

/// 0x08907730
UNK_RET InitializeSplineParameters(SplineParameters& a1, StaticHexGrid<TerrainIce::HexData, SquareXWrapGridPolicy, 255u, 255u> const& a2, cvTerrainSampler const& a3, TerrainIce::IceSettings const& a4);

namespace TerrainIce {
/// 0x08904458
UNK_RET PartitionTriangles(float* a1, float* a2, FGXMathRect::Rect const& a3, TerrainIce::IceSettings const& a4, TerrainIce::UncompressedVertex const* a5, unsigned short* a6, unsigned int a7, unsigned int a8, unsigned int* a9, unsigned int a10, TerrainIce::IceMesh& a11);
/// 0x08904c72
UNK_RET TriangulateIce(float a1, TerrainIce::IceSettings const& a2, TerrainIce::PolygonGraph& a3, FGXMathRect::Rect const& a4, TerrainIce::HeapVector<TerrainIce::CrackPolygon, 8> const& a5, TerrainIce::IceMesh& a6, TerrainIce::DisplacementTexture& a7);
/// 0x089079a0
UNK_RET BuildIceInputGeometry(cvTerrainSampler const& a1, StaticHexGrid<TerrainIce::HexData, SquareXWrapGridPolicy, 255u, 255u>& a2, TerrainIce::IceInputGeometry& a3, TerrainIce::IceSettings const& a4);
}
