/// origin guess
#pragma once
#include <utility>

class FGXVector2;
namespace FGXMathRect {
class Rect;
}
namespace TerrainIce {
class CrackLineGraph;
class CrackPolygon;
class FractureMesh;
class IceInputGeometry;
class IceSettings;
class IndexCountPolyBuffer;
class IndexExtractPolyBuffer;
class KDSegmentHit;
class KDSweepEvent;
class PolyGraphBuffer;
class PolygonGraph;
class TriangulationGraph;
template <class T0, int T1> class HeapVector;
template <class T0, int T1> class TempVector;
}

namespace TerrainIce {
/// size ?
class KDTree {
public:
    class SegNode;
    class Segment;
    /// 0x088f9634
    KDTree();
    /// 0x088f9694
    UNK_RET Build(TerrainIce::IceInputGeometry const& a1);
    /// 0x088f9886
    UNK_RET ComputeSegmentAABB(TerrainIce::KDTree::Segment const& a1, FGXVector2& a2, FGXVector2& a3);
    /// 0x088f98f0
    UNK_RET BuildRecurse(unsigned int a1, unsigned int a2, FGXVector2 const& a3, FGXVector2 const& a4, unsigned int a5, unsigned int a6);
    /// 0x088f9bde
    UNK_RET Build(TerrainIce::FractureMesh const& a1);
    /// 0x088f9e1a
    UNK_RET Build(TerrainIce::CrackLineGraph const& a1);
    /// 0x088fa056
    UNK_RET Build(FGXVector2 const& a1, FGXVector2 const& a2, TerrainIce::PolygonGraph const& a3);
    /// 0x088fa21a
    UNK_RET PointInShape(FGXVector2 const& a1) const;
    /// 0x088fa378
    UNK_RET PointShapeDistanceSq(FGXVector2 const& a1, float a2) const;
    /// 0x088fa590
    UNK_RET FindSegmentCrossing(FGXVector2 const& a1, FGXVector2 const& a2, std::pair<unsigned int, float>& a3) const;
    /// 0x088fa7f0
    UNK_RET AxisSweep(unsigned int a1, float a2, TerrainIce::TempVector<TerrainIce::KDSweepEvent, 16>& a3);
    /// 0x088fabea
    UNK_RET FindSegmentIntersections(TerrainIce::TempVector<TerrainIce::KDSegmentHit, 16>& a1);
    /// 0x088faf0e
    UNK_RET FindSegmentPairs(TerrainIce::TempVector<TerrainIce::KDSegmentHit, 16>& a1);
    /// 0x088fb1da
    UNK_RET CreateLeafNode(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x088fb24c
    UNK_RET PreprocessSplits(TerrainIce::KDTree::SegNode* a1, unsigned int a2, unsigned int (*a3)[3], float (*a4)[2], FGXVector2 const& a5, FGXVector2 const& a6, FGXVector2 const& a7);
    /// 0x088fb356
    UNK_RET SelectSplit(unsigned int (*a1)[3], float (*a2)[2], FGXVector2 const& a3, FGXVector2 const& a4, FGXVector2 const& a5);
    /// 0x088fb4a6
    UNK_RET BuyNodes(unsigned int a1);
    /// 0x088fb538
    UNK_RET BuildLeftAndRightSublists(unsigned int a1, float a2, TerrainIce::KDTree::SegNode* a3, unsigned int a4, TerrainIce::KDTree::SegNode* a5);
    /// 0x088fb66e
    UNK_RET CreateInnerNode(unsigned int a1, std::pair<unsigned int, float> const& a2);
};
}

namespace TerrainIce {
/// 0x088fdd3c
UNK_RET ExtractCrackPolygons(TerrainIce::IceSettings const& a1, TerrainIce::CrackLineGraph& a2, TerrainIce::PolygonGraph& a3, FGXMathRect::Rect& a4, TerrainIce::HeapVector<TerrainIce::CrackPolygon, 8>& a5);
/// 0x08a69e90 MarkBoundaryEdges<14, TerrainIce::TriangulationGraph>
/// 0x088fe0a2 MarkBoundaryEdges<9, TerrainIce::CrackLineGraph>
template <int T0, class T1>
void MarkBoundaryEdges(T1& a1);
/// 0x088fe258 ExtractPolygons<TerrainIce::CrackLineGraph, TerrainIce::PolyGraphBuffer>
/// 0x08a6a05c ExtractPolygons<TerrainIce::TriangulationGraph, TerrainIce::IndexCountPolyBuffer>
/// 0x08a6a22e ExtractPolygons<TerrainIce::TriangulationGraph, TerrainIce::IndexExtractPolyBuffer>
template <class T0, class T1>
void ExtractPolygons(T0& a1, T1& a2);
/// 0x088fef8c
UNK_RET SplitSelfIntersectingPolygons(TerrainIce::PolygonGraph& a1, FGXMathRect::Rect const& a2);
/// 0x088ff9b2
UNK_RET PolygonFixup(TerrainIce::PolygonGraph& a1, FGXMathRect::Rect const& a2, TerrainIce::HeapVector<TerrainIce::CrackPolygon, 8>& a3);
}
