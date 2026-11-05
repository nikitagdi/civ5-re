/// origin guess
#include "Civ5/Src/App/Engine/GameSplineDatabase.h"
#include "Civ5/Src/App/Engine/cvRiverSimSystem.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"

/// 0x087d4608
void GameSplineDatabase::Startup()
{
    m_matrix = m_transform.matrix();
    m_invMatrix = m_transform.inv_matrix();
    ClearDatabase();
}

/// 0x087d474c
void GameSplineDatabase::StartupGame()
{
}

/// 0x087d4858
bool GameSplineDatabase::IsAtTopOrBottomBoundary(HexEdge const& a1) const
{
    HEXVec_i hex = a1.m_kHex;
    Fun_0865f23e(hex);
    const int side = a1.m_iEdge % 6;
    HEXVec_i neighbour;
    neighbour.x = HEXVec_i::TRANSLATIONS[side].x + hex.x;
    neighbour.y = HEXVec_i::TRANSLATIONS[side].y + hex.y;
    neighbour.z = HEXVec_i::TRANSLATIONS[side].z + hex.z;
    const HEXVec_i cell = Fun_086616f0(*reinterpret_cast<const HEXMatrix_i*>(WorldGlobals::g_GameGrid), neighbour);
    return cell.y < 0 || cell.y >= *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + 0x98);
}

/// 0x087d4904
bool GameSplineDatabase::IsAtLeftOrRightBoundary(HexEdge const& a1) const
{
    HEXVec_i hex = a1.m_kHex;
    Fun_0865f23e(hex);
    const int side = a1.m_iEdge % 6;
    HEXVec_i neighbour;
    neighbour.x = HEXVec_i::TRANSLATIONS[side].x + hex.x;
    neighbour.y = HEXVec_i::TRANSLATIONS[side].y + hex.y;
    neighbour.z = HEXVec_i::TRANSLATIONS[side].z + hex.z;
    const HEXVec_i cell = Fun_086616f0(*reinterpret_cast<const HEXMatrix_i*>(WorldGlobals::g_GameGrid), neighbour);
    // Rows are offset by half a cell, so the column is x + y / 2.
    const int column = cell.x + (cell.y >> 1);
    return column < 0 || column >= *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + 0x94);
}

/// 0x087d4762
void GameSplineDatabase::SubmitSegmentToDatabase(HexEdge const& a1, FGXVector2 const* a2, unsigned int a3)
{
    if (IsAtTopOrBottomBoundary(a1))
        return;
    if (WorldGlobals::g_eWrapMode == 0 && IsAtLeftOrRightBoundary(a1))
        return;
    HexEdge canonical = HexEdge();
    Fun_0865f1be(a1, canonical);
    InsertEntry(canonical, a2, a3, false);
    // Same side, other flow direction: edge +6 or -6 within 0..11.
    canonical.m_iEdge += (canonical.m_iEdge % 12 < 6) ? 6 : -6;
    // The second record is stored with its points in reverse order.
    InsertEntry(canonical, a2, a3, true);
}

/// 0x087d4ea0
void GameSplineDatabase::QueryHexSegment(HexEdge const& a1, GameSpline::Segment* a2) const
{
    if (!QuerySavedHexSegment(a1, a2))
        GenerateHexSide(a1, a2);
}

/// 0x087d4ef2
void GameSplineDatabase::QueryGeneratedHexSegment(HexEdge const& a1, GameSpline::Segment* a2) const
{
    GenerateHexSide(a1, a2);
}
