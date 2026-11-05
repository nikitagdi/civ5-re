/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXTransform_i.h"

class HexEdge;

namespace GameSpline {
// One point of a hex-side spline; cvFloodplainsSystem::BuildBankVis reads only the position.
/// size 0xc
class ControlPoint {
public:
    FGXVector2 m_vPos;  ///< +0x0
    float field_8;      ///< +0x8
};

// The spline points of one hex side, up to four kept inline.
/// size 0x40
class Segment : public FStaticVector<ControlPoint, 4u, true> {
public:
    /// inline
    Segment() : field_3d(false) {}

    // Sits in the tail padding of the vector base.
    bool field_3d;  ///< +0x3d
};
}

// Spline points per hex side, keyed by HexEdge. Cells are 200 wide: index (x + y / 2) + 200 * y.
/// size ?
class GameSplineDatabase {
public:
    int m_iFreeRecord;                       ///< +0x0
    int m_iRecordCount;                      ///< +0x4
    unsigned char unk_8[0xc];                ///< +0x8
    HEXMatrix_i m_matrix;                    ///< +0x14
    HEXMatrix_i m_invMatrix;                 ///< +0x54
    HEXTransform_i m_transform;              ///< +0x94
    int m_iWidth;                            ///< +0xa8
    int m_iHeight;                           ///< +0xac
    // Per cell, one record index per HexEdge (0..11); -1 when empty.
    short m_aiRecord[200 * 200][12];         ///< +0xb0

    /// 0x087d4608
    void Startup();
    /// 0x087d4682
    void ClearDatabase();
    /// 0x087d4738
    void Shutdown();
    /// 0x087d474c
    void StartupGame();
    /// 0x087d474e
    void ShutdownGame();
    /// 0x087d4762
    void SubmitSegmentToDatabase(HexEdge const& a1, FGXVector2 const* a2, unsigned int a3);
    /// 0x087d4858
    bool IsAtTopOrBottomBoundary(HexEdge const& a1) const;
    /// 0x087d4904
    bool IsAtLeftOrRightBoundary(HexEdge const& a1) const;
    /// 0x087d49b8
    void InsertEntry(HexEdge const& a1, FGXVector2 const* a2, unsigned int a3, bool a4);
    /// 0x087d4cd2
    bool QuerySavedHexSegment(HexEdge const& a1, GameSpline::Segment* a2) const;
    /// 0x087d4ea0
    void QueryHexSegment(HexEdge const& a1, GameSpline::Segment* a2) const;
    /// 0x087d4ef2
    void QueryGeneratedHexSegment(HexEdge const& a1, GameSpline::Segment* a2) const;
    /// 0x087d4f28
    void GenerateHexSide(HexEdge a1, GameSpline::Segment* a2) const;
};
