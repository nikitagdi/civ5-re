/// origin guess
#include "Civ5/Src/App/Graphics/cvMovementPathVisSystem.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include <Windows.h>
#include <math.h>
#include <string.h>

/// 0x088c0896
void cvMovementPathVisSystem::Update(float a1) {
    m_fAnimTime += a1;
    m_fHighlightTime += a1;
}

/// 0x088c1c1e
void cvMovementPathVisSystem::LoadModelIntoRenderList(cvMovementPathVisSystem::MODEL_INDEX a1, D3DXMATRIX const& a2) {
    if (m_uRenderCount < 0x100) {
        m_akRenderList[m_uRenderCount].m_sModel = (short)a1;
        m_akRenderList[m_uRenderCount].m_kWorld = a2;
        m_uRenderCount++;
    }
}

/// 0x088c08c6
void cvMovementPathVisSystem::UpdateFromHandle(cvGeneralRefHandle<cvMovementPathVisSystem, unsigned int, cvMovementPathVisSystemDataDef, cvMovementPathVisSystem_Accessor>& a1, cvMovementPathVisSystem_DisplayPath a2) {
    for (unsigned int uiEntry = 0; uiEntry < m_uRenderCount; uiEntry++) {
        D3DXMATRIX& kWorld = m_akRenderList[uiEntry].m_kWorld;
        memset(&kWorld, 0, sizeof(kWorld));
        kWorld._11 = kWorld._22 = kWorld._33 = kWorld._44 = 1.0f;
        m_akRenderList[uiEntry].m_sModel = 0;
    }
    m_uRenderCount = 0;
    if (!a2.m_bShow) {
        return;
    }
    HEXMatrix_i const& kGameGrid = *reinterpret_cast<HEXMatrix_i const*>(WorldGlobals::g_GameGrid + 0x40);
    // Hex position in grid space as floats, then its world position; unexplored hexes are lifted to the minimum height.
    auto WorldOf = [&](int iCubeX, int iCubeY, HEXVec_i& kHexOut, FGXVector3& kPosOut) {
        HEXVec_i const kCube = {iCubeX, iCubeY, -(iCubeX + iCubeY)};
        kHexOut = Fun_086616f0(kGameGrid, kCube);
        HEXVec kHexPos = {(float)kHexOut.x, (float)kHexOut.y, (float)kHexOut.z, 1.0f};
        FGXVector4 kWorld = MapGlobals::g_pTerrainSampler->GetWorldLocation(kHexPos);
        kPosOut.x = kWorld.x;
        kPosOut.y = kWorld.y;
        kPosOut.z = kWorld.z;
        if (m_fMinHeight > kWorld.z || IsUnexplored(kHexOut)) {
            kPosOut.z = m_fMinHeight;
        }
    };
    auto Translation = [](FGXVector3 const& kPos) {
        D3DXMATRIX kMatrix;
        D3DXMatrixTranslation(&kMatrix, kPos.x, kPos.y, kPos.z);
        return kMatrix;
    };
    if (m_kPath.m_sNodeCount == 0) {
        // No path: mark the hovered hex.
        unsigned char* pApplication = *reinterpret_cast<unsigned char**>(g_pkApplication);
        int iPacked = *reinterpret_cast<int*>(*reinterpret_cast<unsigned char**>(pApplication + 0xc8) + 0x90);
        HEXVec_i kHex;
        FGXVector3 kPos;
        WorldOf((short)iPacked - (iPacked >> 17), iPacked >> 16, kHex, kPos);
        D3DXMATRIX kTranslate = Translation(kPos);
        if (WillTriggerSwap(kHex)) {
            LoadModelIntoRenderList(MODEL_CENTER_EMBARK, kTranslate);
            LoadModelIntoRenderList(MODEL_SELECTED_EMBARK, kTranslate);
        } else if (WillTriggerAttack(kHex)) {
            LoadModelIntoRenderList(MODEL_CENTER_ATTACK, kTranslate);
            LoadModelIntoRenderList(MODEL_SELECTED_ATTACK, kTranslate);
        } else {
            LoadModelIntoRenderList(MODEL_CENTER_INACCESSIBLE, kTranslate);
            LoadModelIntoRenderList(MODEL_SELECTED_INACCESSIBLE, kTranslate);
        }
        return;
    }
    D3DXMATRIX kBigScale;
    D3DXMATRIX kSmallScale;
    D3DXMatrixScaling(&kBigScale, 1.5f, 1.5f, 1.5f);
    D3DXMatrixScaling(&kSmallScale, 0.75f, 0.75f, 0.75f);
    int const iLast = m_kPath.m_sNodeCount - 1;
    HEXVec_i kPrevHex = {0, 0, 0};
    FGXVector3 kPrevPos;
    kPrevPos.x = 0.0f;
    kPrevPos.y = 0.0f;
    kPrevPos.z = 0.0f;
    for (int iNode = 0; iNode < m_kPath.m_sNodeCount; iNode++) {
        cvMovementPathVisSystem_PathNode const& kNode = m_kPath.m_akNodes[iNode];
        HEXVec_i kHex;
        FGXVector3 kPos;
        WorldOf(kNode.m_sX - (kNode.m_sY >> 1), kNode.m_sY, kHex, kPos);
        D3DXMATRIX kTranslate = Translation(kPos);
        bool const bLast = iNode == iLast;
        // The final hex of each turn gets the large marker.
        bool const bLarge = bLast || m_kPath.m_akNodes[iNode + 1].m_sTurns > kNode.m_sTurns;
        D3DXMATRIX kScaled;
        D3DXMatrixMultiply(&kScaled, bLarge ? &kBigScale : &kSmallScale, &kTranslate);
        if (iNode == 0) {
            LoadModelIntoRenderList(MODEL_CENTER_ONE_TURN, kTranslate);
        } else if (bLast && WillTriggerAttack(kHex)) {
            LoadModelIntoRenderList(MODEL_CENTER_ATTACK, kScaled);
            LoadModelIntoRenderList(MODEL_SELECTED_ATTACK, kTranslate);
            LoadConnectorModelsIntoRenderList(MODEL_CONNECTOR_ATTACK, kPrevPos, kPos, 6, true);
        } else if (WillTriggerEmbark(kHex, kPrevHex) || WillTriggerDisembark(kHex, kPrevHex)) {
            LoadModelIntoRenderList(MODEL_CENTER_EMBARK, kScaled);
            LoadModelIntoRenderList(MODEL_SELECTED_EMBARK, kTranslate);
            LoadConnectorModelsIntoRenderList(MODEL_CONNECTOR_EMBARK, kPrevPos, kPos, 6, false);
        } else if (kNode.m_sTurns == 2) {
            if (bLast) {
                LoadModelIntoRenderList(MODEL_SELECTED_TWO_TURNS, kTranslate);
            }
            LoadModelIntoRenderList(MODEL_CENTER_TWO_TURNS, kScaled);
            LoadConnectorModelsIntoRenderList(MODEL_CONNECTOR_TWO_TURNS, kPrevPos, kPos, 6, bLast);
        } else if (kNode.m_sTurns == 1) {
            if (bLast) {
                LoadModelIntoRenderList(MODEL_SELECTED_ONE_TURN, kTranslate);
            }
            LoadModelIntoRenderList(MODEL_CENTER_ONE_TURN, kScaled);
            LoadConnectorModelsIntoRenderList(MODEL_CONNECTOR_ONE_TURN, kPrevPos, kPos, 6, bLast);
        } else {
            if (bLast) {
                LoadModelIntoRenderList(MODEL_SELECTED_THREE_TURNS, kTranslate);
            }
            LoadModelIntoRenderList(MODEL_CENTER_THREE_TURNS, kScaled);
            LoadConnectorModelsIntoRenderList(MODEL_CONNECTOR_THREE_TURNS, kPrevPos, kPos, 6, bLast);
        }
        kPrevHex = kHex;
        kPrevPos = kPos;
    }
}

/// 0x088c279e
void cvMovementPathVisSystem::UpdateFromHandle(cvGeneralRefHandle<cvMovementPathVisSystem, unsigned int, cvMovementPathVisSystemDataDef, cvMovementPathVisSystem_Accessor>& a1, cvMovementPathVisSystem_UpdatePath a2) {
    memcpy(&m_kPath, &a2, sizeof(a2));
}

/// 0x088c1e8e
void cvMovementPathVisSystem::LoadConnectorModelsIntoRenderList(cvMovementPathVisSystem::MODEL_INDEX a1, FGXVector3 const& a2, FGXVector3 const& a3, unsigned int a4, bool a5) {
    auto NormalizeInPlace = [](float* v) {
        float fLen = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if (fLen > 0.0f) {
            v[0] /= fLen;
            v[1] /= fLen;
            v[2] /= fLen;
        }
    };
    float fStep = 1.0f / ((float)a4 + 1.0f);
    float akStep[3] = {(a3.x - a2.x) * fStep, (a3.y - a2.y) * fStep, (a3.z - a2.z) * fStep};
    float akDir[3] = {akStep[0], akStep[1], akStep[2]};
    NormalizeInPlace(akDir);
    float akSide[3] = {akDir[1], -akDir[0], 0.0f};
    NormalizeInPlace(akSide);
    D3DXMATRIX kBasis;
    memset(&kBasis, 0, sizeof(kBasis));
    kBasis._11 = akSide[0]; kBasis._12 = akSide[1]; kBasis._13 = akSide[2];
    kBasis._21 = akDir[0];  kBasis._22 = akDir[1];  kBasis._23 = akDir[2];
    kBasis._33 = 1.0f;
    kBasis._44 = 1.0f;
    float fX = a2.x, fY = a2.y, fZ = a2.z;
    // The last segment (a5) places only half of the connectors.
    for (unsigned int uLeft = a4 >> a5; uLeft != 0; uLeft--) {
        fX += akStep[0];
        fY += akStep[1];
        fZ += akStep[2];
        D3DXMATRIX kWorld = kBasis;
        kWorld._41 = fX;
        kWorld._42 = fY;
        kWorld._43 = fZ;
        if (m_uRenderCount < 0x100) {
            m_akRenderList[m_uRenderCount].m_sModel = (short)a1;
            m_akRenderList[m_uRenderCount].m_kWorld = kWorld;
            m_uRenderCount++;
        }
    }
}
