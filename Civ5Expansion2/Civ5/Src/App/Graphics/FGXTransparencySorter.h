/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FString.h"
#include <d3d9.h>

class FGXTransparencyScene;
class FGXVector3;

/// size ?
class FGXTransparencySorter {
public:
    class AssignToCellsJob;
    class GenericJob;
    class OverlapTestJob;
    enum AABB : int;
    /// size ?
    /// vtable ?
    class GenericJob : public FJob {
    public:
    };
    /// size ?
    /// vtable 0x0a2fd95c
    class AssignToCellsJob : public FGXTransparencySorter::GenericJob {
    public:
        /// 0x088a4362
        virtual void Execute(unsigned int a1);
    };
    class DependencyGraph;
    class EdgeList;
    /// size ?
    /// vtable 0x0a2fd934
    class OverlapTestJob : public FGXTransparencySorter::GenericJob {
    public:
        /// 0x088a4336
        virtual void Execute(unsigned int a1);
    };
    class SortResults;
    class ThreadContext;
    class Triangle;
    class TriangleOverlapData;
    /// 0x0889e8b0
    FGXTransparencySorter();
    /// 0x0889e96c
    ~FGXTransparencySorter();
    /// 0x0889eab0
    UNK_RET Shutdown();
    /// 0x0889eb0e
    UNK_RET Startup();
    /// 0x0889eb42
    UNK_RET SetScene(FGXTransparencyScene const* a1);
    /// 0x0889ed38
    UNK_RET GetDynamicIB(unsigned int a1, unsigned int a2);
    /// 0x0889ee54
    UNK_RET SortGeometry(D3DXMATRIX const& a1, FGXTransparencySorter::SortResults& a2);
    /// 0x0889f12c
    UNK_RET TransformVertices(D3DXMATRIX const& a1);
    /// 0x0889f18a
    UNK_RET PreprocessTriangles();
    /// 0x0889f246
    UNK_RET BuildDependencyGraphThreaded(FGXTransparencySorter::DependencyGraph& a1);
    /// 0x0889f31c
    UNK_RET RankTriangles(FGXTransparencySorter::EdgeList* a1, FGXTransparencySorter::Triangle* a2, unsigned int a3);
    /// 0x0889f398
    UNK_RET ReorderTriangles(FGXTransparencySorter::Triangle* a1, unsigned int a2);
    /// 0x0889f3ce
    UNK_RET AssembleResults(FGXTransparencySorter::SortResults& a1);
    /// 0x0889f9ce
    UNK_RET GetSortStats(FStringA& a1);
    /// 0x0889fad2
    UNK_RET GetSortTime();
    /// 0x0889fafc
    UNK_RET PutTrianglesInGrid();
    /// 0x0889fcaa
    UNK_RET BuildCompactEdgeLists(FGXTransparencySorter::DependencyGraph& a1, unsigned int a2, FGXTransparencySorter::ThreadContext* a3, unsigned int a4);
    /// 0x0889fe1c
    UNK_RET AssignTrianglesToCells(unsigned int a1);
    /// 0x088a0326
    UNK_RET TriRejectBox(FGXTransparencySorter::TriangleOverlapData const& a1, FGXTransparencySorter::AABB const& a2);
    /// 0x088a03a0
    UNK_RET DoOverlapTesting(unsigned int a1);
    /// 0x088a049a
    UNK_RET DoSameLevelOverlapTests(unsigned int a1, unsigned int a2, unsigned short* a3);
    /// 0x088a07b8
    UNK_RET DoHigherLevelOverlapTests(unsigned int a1, unsigned int a2);
    /// 0x088a0c30
    UNK_RET OverlapTest(FGXTransparencySorter::TriangleOverlapData const* a1, FGXTransparencySorter::TriangleOverlapData const* a2);
    /// 0x088a1112
    UNK_RET PointInTriangle(FGXVector3 const& a1, FGXTransparencySorter::TriangleOverlapData const* a2);
    /// 0x088a1194
    UNK_RET RankGraph(FGXTransparencySorter::EdgeList* a1, FGXTransparencySorter::Triangle* a2, unsigned int a3, unsigned int a4);
};
