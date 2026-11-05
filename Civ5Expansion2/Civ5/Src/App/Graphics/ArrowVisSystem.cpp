/// origin binary
#include <math.h>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/Civ5App.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/ArrowVisSystem.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "Civ5/Src/App/cvFloodplainsSystem.h"
#include "Civ5/Src/App/cvPath.h"
#include "FirePlace/FireGrafix/ArrowShader.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlElement.h"

namespace {

typedef EventTemplate<Event_Int2Type<165>, SpawnArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> SpawnArrowEvent;
typedef EventTemplate<Event_Int2Type<166>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> RemoveAllArrowsEvent;
typedef EventTemplate<Event_Int2Type<167>, RemoveArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> RemoveArrowEvent;

const char kArrowSettingsDirectory[] = "assets/UI/Overlays";
const char kArrowSettingsPath[] = "assets/UI/Overlays/ArrowSettings.xml";
const unsigned int kFileMonitorFlags = 0x10;
const unsigned int kGeometryReserve = 0x400;
const unsigned int kColorTextLength = 0x40;
const unsigned int kColorParseLength = 0x100;
const int kColorChannelLimit = 255;
const float kColorChannelMax = 255.0f;
const unsigned int kOpaqueAlpha = 0xff000000u;

// Dynamic buffer capacities: vertex format 5, 32-bit index format 1.
const unsigned int kArrowVertexFormat = 5;
const unsigned int kArrowIndexFormat = 1;
const unsigned int kMaxArrowVertices = 4000;
const unsigned int kMaxArrowIndices = 3000;
const float kMaxProgress = 0.9999f;

// BeginCommandSet alpha and z state indices of the two arrow passes.
const unsigned int kFirstPassAlphaState = 2;
const unsigned int kSecondPassAlphaState = 8;
const unsigned int kArrowZState = 0;
// ArrowShaderConstants float slots after ViewProj.
const unsigned int kConstAlpha = 0x10;
const unsigned int kConstAmbient = 0x14;

// Shaft rings are sampled every kShaftStep of progress; the spline parameter is 2*t over 4 control points.
const float kShaftStep = 0.04f;
const float kShaftStepSlack = 0.001f;
const int kArrowControlPoints = 4;
const float kSplineParamScale = 2.0f;
// |tangent.x| above this banks the tube about the tangent by tangent.x * kBankScale * pi.
const double kBankThreshold = 0.867;
const float kBankScale = 0.175f;
const float kBankPi = 3.1415f;
const float kSqrt3 = 1.7320508f;
const unsigned int kBodyFlag = 0xffffffffu;
const unsigned int kOutlineFlag = 0;
const unsigned int kBoxVertices = 8;
const unsigned int kRingVertices = 16;
const unsigned int kHeadVertices = 26;
const unsigned int kHeadTriangles = 14;
// Head triangles in body winding; the outline head uses the same table with the last two corners swapped.
const unsigned int kHeadTriangleTable[kHeadTriangles][3] = {
    {0, 12, 1}, {1, 12, 2}, {2, 12, 3}, {4, 13, 5}, {5, 13, 14}, {6, 15, 7}, {7, 15, 8},
    {8, 15, 9}, {10, 16, 11}, {11, 16, 17}, {18, 22, 19}, {19, 22, 23}, {20, 24, 21}, {21, 24, 25},
};

typedef FFastVector<FireGrafix::VStruct<5>, true, 62u, 0u, BaseVector<FireGrafix::VStruct<5>, true>::FDefaultFastVectorAllocator> ArrowVertexVector;

const auto Scaled = [](const FGXVector3& v, float f) -> FGXVector3 {
    const FGXVector3 r = {v.x * f, v.y * f, v.z * f};
    return r;
};

const auto Sum = [](const FGXVector3& a, const FGXVector3& b) -> FGXVector3 {
    const FGXVector3 r = {a.x + b.x, a.y + b.y, a.z + b.z};
    return r;
};

const auto Negated = [](const FGXVector3& v) -> FGXVector3 {
    const FGXVector3 r = {-v.x, -v.y, -v.z};
    return r;
};

const auto Cross = [](const FGXVector3& a, const FGXVector3& b) -> FGXVector3 {
    const FGXVector3 r = {a.y * b.z - a.z * b.y, a.z * b.x - b.z * a.x, b.y * a.x - b.x * a.y};
    return r;
};

const auto MakeVertex = [](const FGXVector3& vPosition, const FGXVector3& vNormal, unsigned int uColor, unsigned int uFlag) -> ArrowVis::ArrowVertex {
    ArrowVis::ArrowVertex kVertex = {};
    kVertex.m_vPosition = vPosition;
    kVertex.m_vNormal = vNormal;
    kVertex.m_uColor = uColor;
    kVertex.m_uBodyFlag = uFlag;
    return kVertex;
};

// The vertex vector is typed by the empty VStruct<5>; its elements are ArrowVertex.
const auto PushVertex = [](ArrowVertexVector& aVertices, const ArrowVis::ArrowVertex& kVertex) {
    aVertices.push_back(reinterpret_cast<const FireGrafix::VStruct<5>&>(kVertex));
};

// Spline point and tube frame at fT: side = cross(T, Z), up = cross(side, T); side is banked on steep x.
const auto EvalArrowFrame = [](const FGXVector3* avControl, float fT, FGXVector3& vCenter, FGXVector3& vTangent, FGXVector3& vSide, FGXVector3& vUp) {
    const float fParam = fT * kSplineParamScale;
    vCenter = B_SPLINE::EvalScalingFunc<FGXVector3, FGXVector3 const*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(1), static_cast<B_SPLINE::BSPLINE_END_TYPES>(1)>(kArrowControlPoints, avControl, fParam, 0);
    vTangent = B_SPLINE::EvalGradient<FGXVector3, FGXVector3 const*, static_cast<B_SPLINE::BSPLINE_BEGIN_TYPES>(1), static_cast<B_SPLINE::BSPLINE_END_TYPES>(1)>(kArrowControlPoints, avControl, fParam, 0);
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&vTangent), reinterpret_cast<const D3DXVECTOR3*>(&vTangent));
    const FGXVector3 vZ = {0.0f, 0.0f, 1.0f};
    vSide = Cross(vTangent, vZ);
    vUp = Cross(vSide, vTangent);
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&vSide), reinterpret_cast<const D3DXVECTOR3*>(&vSide));
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&vUp), reinterpret_cast<const D3DXVECTOR3*>(&vUp));

    D3DXVECTOR2 vFlat = {vTangent.x, vTangent.y};
    D3DXVec2Normalize(&vFlat, &vFlat);
    const float fAlongX = vFlat.y * 0.0f + vFlat.x;
    float fAngle = 0.0f;
    bool bBank = true;
    if (static_cast<double>(fAlongX) > kBankThreshold || static_cast<double>(fAlongX) < -kBankThreshold) {
        fAngle = fAlongX * kBankScale * kBankPi;
    } else if (vFlat.x * 0.0f + vFlat.y > 0.0f) {
        fAngle = fAlongX * -kBankScale * kBankPi;
    } else {
        bBank = false;
    }
    if (bBank) {
        D3DXMATRIX mBank;
        D3DXMatrixRotationAxis(&mBank, reinterpret_cast<const D3DXVECTOR3*>(&vTangent), fAngle);
        D3DXVec3TransformCoord(reinterpret_cast<D3DXVECTOR3*>(&vSide), reinterpret_cast<const D3DXVECTOR3*>(&vSide), &mBank);
    }
};

// Four faces of a box cross-section, two vertices each: top (+up), right (+side), bottom (-up), left (-side).
const auto MakeBox = [](ArrowVis::ArrowVertex* aBox, const FGXVector3& vCenter, const FGXVector3& vSide, const FGXVector3& vUp, float fHalfWidth, float fHalfHeight, bool bNormals, unsigned int uColor, unsigned int uFlag) {
    const FGXVector3 vW = Scaled(vSide, fHalfWidth);
    const FGXVector3 vH = Scaled(vUp, fHalfHeight);
    const FGXVector3 vTopLeft = Sum(Sum(vCenter, Negated(vW)), vH);
    const FGXVector3 vTopRight = Sum(Sum(vCenter, vW), vH);
    const FGXVector3 vBottomRight = Sum(Sum(vCenter, vW), Negated(vH));
    const FGXVector3 vBottomLeft = Sum(Sum(vCenter, Negated(vW)), Negated(vH));
    const FGXVector3 vZero = {0.0f, 0.0f, 0.0f};
    const FGXVector3 vTop = bNormals ? vUp : vZero;
    const FGXVector3 vRight = bNormals ? vSide : vZero;
    const FGXVector3 vBottom = bNormals ? Negated(vUp) : vZero;
    const FGXVector3 vLeft = bNormals ? Negated(vSide) : vZero;
    aBox[0] = MakeVertex(vTopLeft, vTop, uColor, uFlag);
    aBox[1] = MakeVertex(vTopRight, vTop, uColor, uFlag);
    aBox[2] = MakeVertex(vTopRight, vRight, uColor, uFlag);
    aBox[3] = MakeVertex(vBottomRight, vRight, uColor, uFlag);
    aBox[4] = MakeVertex(vBottomRight, vBottom, uColor, uFlag);
    aBox[5] = MakeVertex(vBottomLeft, vBottom, uColor, uFlag);
    aBox[6] = MakeVertex(vBottomLeft, vLeft, uColor, uFlag);
    aBox[7] = MakeVertex(vTopLeft, vLeft, uColor, uFlag);
};

const auto FaceNormal = [](const FGXVector3& vOrigin, const FGXVector3& vA, const FGXVector3& vB) -> FGXVector3 {
    const FGXVector3 vEdgeA = {vA.x - vOrigin.x, vA.y - vOrigin.y, vA.z - vOrigin.z};
    const FGXVector3 vEdgeB = {vB.x - vOrigin.x, vB.y - vOrigin.y, vB.z - vOrigin.z};
    const FGXVector3 vCross = Cross(vEdgeA, vEdgeB);
    const float fLength = sqrtf(vCross.z * vCross.z + (vCross.y * vCross.y + vCross.x * vCross.x));
    const FGXVector3 vNormal = {vCross.x / fLength, vCross.y / fLength, vCross.z / fLength};
    return vNormal;
};

// Flat arrowhead: base from -fOuter to +fOuter across vSide, tip fOuter*sqrt(3) along the tangent.
const auto MakeHead = [](ArrowVis::ArrowVertex* aHead, const FGXVector3& vCenter, const FGXVector3& vTangent, const FGXVector3& vSide, const FGXVector3& vUp, float fInner, float fOuter, float fHalfHeight, unsigned int uColor, unsigned int uFlag) {
    const FGXVector3 vH = Scaled(vUp, fHalfHeight);
    const FGXVector3 vMinusH = Negated(vH);
    const FGXVector3 vOuterLeft = Sum(vCenter, Scaled(vSide, -fOuter));
    const FGXVector3 vInnerLeft = Sum(vCenter, Scaled(vSide, -fInner));
    const FGXVector3 vInnerRight = Sum(vCenter, Scaled(vSide, fInner));
    const FGXVector3 vOuterRight = Sum(vCenter, Scaled(vSide, fOuter));
    const FGXVector3 vTip = Sum(vCenter, Scaled(vTangent, fOuter * kSqrt3));
    const FGXVector3 aPositions[kHeadVertices] = {
        Sum(vOuterLeft, vH), Sum(vInnerLeft, vH), Sum(vInnerRight, vH), Sum(vOuterRight, vH),
        Sum(vOuterRight, vH), Sum(vOuterRight, vMinusH), Sum(vOuterRight, vMinusH), Sum(vInnerRight, vMinusH),
        Sum(vInnerLeft, vMinusH), Sum(vOuterLeft, vMinusH), Sum(vOuterLeft, vMinusH), Sum(vOuterLeft, vH),
        Sum(vTip, vH), Sum(vTip, vH), Sum(vTip, vMinusH), Sum(vTip, vMinusH), Sum(vTip, vMinusH), Sum(vTip, vH),
        Sum(vOuterLeft, vMinusH), Sum(vInnerLeft, vMinusH), Sum(vInnerRight, vMinusH), Sum(vOuterRight, vMinusH),
        Sum(vOuterLeft, vH), Sum(vInnerLeft, vH), Sum(vInnerRight, vH), Sum(vOuterRight, vH),
    };
    const FGXVector3 vZero = {0.0f, 0.0f, 0.0f};
    for (unsigned int i = 0; i < kHeadVertices; ++i) {
        aHead[i] = MakeVertex(aPositions[i], vZero, uColor, uFlag);
    }
};

const auto SetHeadNormals = [](ArrowVis::ArrowVertex* aHead, const FGXVector3& vTangent, const FGXVector3& vUp) {
    const FGXVector3 vRight = FaceNormal(aHead[4].m_vPosition, aHead[13].m_vPosition, aHead[5].m_vPosition);
    const FGXVector3 vLeft = FaceNormal(aHead[11].m_vPosition, aHead[10].m_vPosition, aHead[17].m_vPosition);
    const FGXVector3 vDown = Negated(vUp);
    const FGXVector3 vBack = Negated(vTangent);
    const unsigned int auTop[] = {0, 1, 2, 3, 12};
    const unsigned int auRight[] = {4, 5, 13, 14};
    const unsigned int auBottom[] = {6, 7, 8, 9, 15};
    const unsigned int auLeft[] = {10, 11, 16, 17};
    for (unsigned int i = 0; i < 5; ++i) {
        aHead[auTop[i]].m_vNormal = vUp;
        aHead[auBottom[i]].m_vNormal = vDown;
    }
    for (unsigned int i = 0; i < 4; ++i) {
        aHead[auRight[i]].m_vNormal = vRight;
        aHead[auLeft[i]].m_vNormal = vLeft;
    }
    for (unsigned int i = 18; i < kHeadVertices; ++i) {
        aHead[i].m_vNormal = vBack;
    }
};

}

/// 0x0884d104
UNK_RET ArrowVis::CreateBuffers() {
    m_hVB = FireGrafix::g_pRenderer->CreateDynamicVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(kArrowVertexFormat), kMaxArrowVertices, 0);
    m_hIB = FireGrafix::g_pRenderer->CreateDynamicIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(kArrowIndexFormat), kMaxArrowIndices, 0);
}

/// 0x0884d172
UNK_RET ArrowVis::ReleaseBuffers() {
    Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, m_hVB);
    Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_hIB);
    FireGrafix::g_pRenderer->ReleaseSurface(m_hSurface);
}

/// 0x0884d1ce
UNK_RET ArrowVis::UpdateArrowBuffers(FFastVector<FireGrafix::VStruct<5>, true, 62u, 0u, BaseVector<FireGrafix::VStruct<5>, true>::FDefaultFastVectorAllocator>& aVertices, FFastVector<unsigned int, true, 62u, 0u, BaseVector<unsigned int, true>::FDefaultFastVectorAllocator>& aIndices) {
    const ArrowVisSystem& kSettings = g_cvArrowVisSystem;
    const unsigned int uBodyColor = kSettings.m_uBodyColor;
    const unsigned int uOutlineColor = kSettings.m_uOutlineColor;
    const float fMinWidth = kSettings.m_fMinWidth;
    const float fWidthRange = kSettings.m_fMaxWidth - kSettings.m_fMinWidth;
    const float fWidth2 = kSettings.m_fWidth2;
    const float fOutlineWidth = kSettings.m_fOutlineWidth;
    const float fOutlineHeight = kSettings.m_fWidth2 + kSettings.m_fOutlineWidth2;
    const float fArcHeight = kSettings.m_fArcHeight;
    aVertices.clear();
    aIndices.clear();

    // Control points at 0, 1/4, 3/4 and 1 of the way; the middle two are lifted by arc_height.
    FGXVector3 avControl[kArrowControlPoints] = {};
    avControl[0] = m_vStart;
    avControl[1].x = m_vEnd.x * 0.25f + m_vStart.x * 3.0f * 0.25f;
    avControl[1].y = m_vEnd.y * 0.25f + m_vStart.y * 3.0f * 0.25f;
    avControl[1].z = m_vEnd.z * 0.25f + m_vStart.z * 3.0f * 0.25f + fArcHeight;
    avControl[2].x = m_vStart.x * 0.25f + m_vEnd.x * 3.0f * 0.25f;
    avControl[2].y = m_vStart.y * 0.25f + m_vEnd.y * 3.0f * 0.25f;
    avControl[2].z = fArcHeight + (m_vStart.z * 0.25f + m_vEnd.z * 3.0f * 0.25f);
    avControl[3] = m_vEnd;

    FGXVector3 vCenter;
    FGXVector3 vTangent;
    FGXVector3 vSide;
    FGXVector3 vUp;
    float fHeadT = m_fProgress;
    for (float fT = 0.0f; fT < kShaftStep + m_fProgress - kShaftStepSlack; fT += kShaftStep) {
        if (fT > m_fProgress) {
            fT = m_fProgress;
        }
        const float fWidth = fWidthRange * fT + fMinWidth;
        EvalArrowFrame(avControl, fT, vCenter, vTangent, vSide, vUp);
        ArrowVertex aRing[kRingVertices];
        MakeBox(aRing, vCenter, vSide, vUp, fWidth, fWidth2, true, uBodyColor, kBodyFlag);
        MakeBox(aRing + kBoxVertices, vCenter, vSide, vUp, fWidth + fOutlineWidth, fOutlineHeight, false, uOutlineColor, kOutlineFlag);
        for (unsigned int i = 0; i < kRingVertices; ++i) {
            PushVertex(aVertices, aRing[i]);
        }
        const unsigned int uNumVertices = aVertices.size();
        if (uNumVertices >= 2 * kRingVertices) {
            const unsigned int uPrev = uNumVertices - 2 * kRingVertices;
            const unsigned int uCur = uNumVertices - kRingVertices;
            // Outline faces are wound the other way so only their inside shows.
            const unsigned int aBodyOrder[6] = {0, 1, 2, 3, 4, 5};
            const unsigned int aOutlineOrder[6] = {0, 2, 1, 3, 5, 4};
            for (unsigned int k = 0; k < 2 * kBoxVertices; k += 2) {
                const unsigned int aQuad[6] = {uPrev + k, uCur + k, uPrev + k + 1, uPrev + k + 1, uCur + k, uCur + k + 1};
                const unsigned int* pOrder = k < kBoxVertices ? aBodyOrder : aOutlineOrder;
                for (unsigned int i = 0; i < 6; ++i) {
                    aIndices.push_back(aQuad[pOrder[i]]);
                }
            }
            // The shaft stops one head length plus outline short of the end point.
            const FGXVector3 vToEnd = {m_vEnd.x - vCenter.x, m_vEnd.y - vCenter.y, m_vEnd.z - vCenter.z};
            const float fDistance = sqrtf(vToEnd.z * vToEnd.z + (vToEnd.y * vToEnd.y + vToEnd.x * vToEnd.x));
            if ((fWidth + fWidth) * kSqrt3 + fOutlineWidth >= fDistance) {
                m_bAnimating = false;
                fHeadT = fT;
                break;
            }
        }
    }

    // Head base: a body box at the head start and a copy moved outline_width along the tangent.
    const float fHeadWidth = fWidthRange * fHeadT + fMinWidth;
    EvalArrowFrame(avControl, fHeadT, vCenter, vTangent, vSide, vUp);
    const FGXVector3 vOutlineShift = Scaled(vTangent, fOutlineWidth);
    ArrowVertex aCollar[2 * kBoxVertices];
    MakeBox(aCollar, vCenter, vSide, vUp, fHeadWidth, fWidth2, true, uBodyColor, kBodyFlag);
    for (unsigned int i = 0; i < kBoxVertices; ++i) {
        aCollar[kBoxVertices + i] = aCollar[i];
        aCollar[kBoxVertices + i].m_vPosition = Sum(aCollar[i].m_vPosition, vOutlineShift);
    }
    for (unsigned int i = 0; i < 2 * kBoxVertices; ++i) {
        PushVertex(aVertices, aCollar[i]);
    }
    const unsigned int uCollar = aVertices.size() - 2 * kBoxVertices;
    for (unsigned int k = 0; k < kBoxVertices; k += 2) {
        const unsigned int uNear = uCollar + k;
        const unsigned int uFar = uCollar + kBoxVertices + k;
        const unsigned int aQuad[6] = {uNear, uFar, uNear + 1, uNear + 1, uFar, uFar + 1};
        for (unsigned int i = 0; i < 6; ++i) {
            aIndices.push_back(aQuad[i]);
        }
    }

    // Body head, moved outline_width forward so the outline head shows around it.
    ArrowVertex aHead[kHeadVertices];
    MakeHead(aHead, vCenter, vTangent, vSide, vUp, fHeadWidth, fHeadWidth + fHeadWidth, fWidth2, uBodyColor, kBodyFlag);
    SetHeadNormals(aHead, vTangent, vUp);
    for (unsigned int i = 0; i < kHeadVertices; ++i) {
        aHead[i].m_vPosition = Sum(aHead[i].m_vPosition, vOutlineShift);
        PushVertex(aVertices, aHead[i]);
    }
    unsigned int uHead = aVertices.size() - kHeadVertices;
    for (unsigned int t = 0; t < kHeadTriangles; ++t) {
        aIndices.push_back(uHead + kHeadTriangleTable[t][0]);
        aIndices.push_back(uHead + kHeadTriangleTable[t][1]);
        aIndices.push_back(uHead + kHeadTriangleTable[t][2]);
    }

    // Outline head: not moved, wider by outline_width, taller by outline_width2, zero normals.
    MakeHead(aHead, vCenter, vTangent, vSide, vUp, fHeadWidth, fHeadWidth + fHeadWidth + fOutlineWidth, fOutlineHeight, uOutlineColor, kOutlineFlag);
    for (unsigned int i = 0; i < kHeadVertices; ++i) {
        PushVertex(aVertices, aHead[i]);
    }
    uHead = aVertices.size() - kHeadVertices;
    for (unsigned int t = 0; t < kHeadTriangles; ++t) {
        aIndices.push_back(uHead + kHeadTriangleTable[t][0]);
        aIndices.push_back(uHead + kHeadTriangleTable[t][2]);
        aIndices.push_back(uHead + kHeadTriangleTable[t][1]);
    }

    const unsigned int uNumIndices = aIndices.size();
    if (uNumIndices == 0) {
        return;
    }
    const unsigned int uNumVertices = aVertices.size();
    void* pVertexData = FireGrafix::g_pRenderer->LockDynamicVB(m_hVB, 0);
    void* pIndexData = FireGrafix::g_pRenderer->LockDynamicIB(m_hIB, 0);
    memcpy(pVertexData, aVertices.begin(), uNumVertices * sizeof(ArrowVertex));
    memcpy(pIndexData, aIndices.begin(), uNumIndices * sizeof(unsigned int));
    FireGrafix::g_pRenderer->UnlockIB(m_hIB, 0, uNumIndices);
    FireGrafix::g_pRenderer->UnlockVB(m_hVB, 0, uNumVertices);
    FireGrafix::g_pRenderer->ReleaseSurface(m_hSurface);
    FireGrafix::FGXSurface* pSurface;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, &m_hSurface, 0);
    // Primitive type 0 is drawn indexed; field_c is the triangle count.
    pSurface->field_60 = 0;
    pSurface->m_hIB = m_hIB;
    pSurface->m_hVB = m_hVB;
    pSurface->field_c = uNumIndices / 3;
    m_bDirty = false;
}

/// 0x08851158
UNK_RET ArrowVis::Update(float fDeltaTime) {
    m_fGrowRate = 1.0f / g_cvArrowVisSystem.m_fAnimSpeed;
    if (m_fProgress < kMaxProgress) {
        m_fProgress += m_fGrowRate * fDeltaTime;
    }
    if (m_fProgress >= kMaxProgress) {
        m_fProgress = kMaxProgress;
        m_bAnimating = false;
    }
}

/// 0x08851232
ArrowVisSystem::ArrowVisSystem() : field_0(8), m_pCommandStream(NULL), m_pShader(NULL) {
}

/// 0x088512d4
ArrowVisSystem::~ArrowVisSystem() {
}

/// 0x08851320
void ArrowVisSystem::Startup() {
    m_aVertices.reserve(kGeometryReserve);
    m_aIndices.reserve(kGeometryReserve);
    m_pCommandStream = FireGrafix::g_pRenderer->CreateCommandStream(0x80, 2, 0x60, 0);
    m_pShader = new FireGrafix::ArrowShader();
    SubscribeToEvents();
    LoadSettingsFromXML();

    // The single arrow starts hidden until event 165 places it.
    ArrowVis kArrow;
    kArrow.m_bHidden = true;
    kArrow.CreateBuffers();
    m_aArrows.push_back(kArrow);
}

/// 0x0885145a
void ArrowVisSystem::SubscribeToEvents() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    void (ArrowVisSystem::*pfnOnSpawn)(SpawnArrowEvent&) = &ArrowVisSystem::OnSpawnArrow;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnSpawn, sizeof(pfnOnSpawn));
    Fun_088535c6(&kDelegate);
    kDelegate.m_pObject = this;
    void (ArrowVisSystem::*pfnOnRemoveAll)(RemoveAllArrowsEvent&) = &ArrowVisSystem::OnRemoveAllArrows;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnRemoveAll, sizeof(pfnOnRemoveAll));
    Fun_08853234(&kDelegate);
    kDelegate.m_pObject = this;
    void (ArrowVisSystem::*pfnOnRemove)(RemoveArrowEvent&) = &ArrowVisSystem::OnRemoveArrow;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnRemove, sizeof(pfnOnRemove));
    Fun_08852ea2(&kDelegate);
}

/// 0x088514d6
void ArrowVisSystem::LoadSettingsFromXML() {
    FXmlDocument kDocument;
    FXmlElement kRoot;
    Civ5App* pApplication = *reinterpret_cast<Civ5App* const*>(g_pkApplication);
    pApplication->SetupDirectoryFileCatalog(kArrowSettingsDirectory, -1, NULL, NULL);
    if (!kDocument.Load(kArrowSettingsPath)) {
        MessageBoxA(0, "Could not load arrow xml file", "Error", MB_ICONEXCLAMATION);
        return;
    }
    kDocument.GetRootElement(kRoot);
    kRoot.GetFirstAttributeVal(m_fMinWidth, "min_width", 9);
    kRoot.GetFirstAttributeVal(m_fMaxWidth, "max_width", 9);
    kRoot.GetFirstAttributeVal(m_fWidth2, "width2", 6);
    kRoot.GetFirstAttributeVal(m_fOutlineWidth, "outline_width", 0xd);
    kRoot.GetFirstAttributeVal(m_fOutlineWidth2, "outline_width2", 0xe);
    kRoot.GetFirstAttributeVal(m_fArcHeight, "arc_height", 10);
    kRoot.GetFirstAttributeVal(m_fTargetHeight, "target_height", 0xd);
    kRoot.GetFirstAttributeVal(m_fAnimSpeed, "anim_speed", 10);
    kRoot.GetFirstAttributeVal(m_fAlpha, "alpha", 5);
    kRoot.GetFirstAttributeVal(m_fAmbientLightLevel, "ambient_light_level", 0x13);

    FStringFixedBufferA(szColor, kColorTextLength);
    FGXVector3 vBodyColor = {0.0f, 0.0f, 0.0f};
    FGXVector3 vOutlineColor = {0.0f, 0.0f, 0.0f};
    kRoot.GetFirstAttributeVal(szColor, "body_color", 10);
    ExtractColor(szColor, vBodyColor);
    kRoot.GetFirstAttributeVal(szColor, "outline_color", 0xd);
    ExtractColor(szColor, vOutlineColor);
    // Blue is not masked to 8 bits before the shift.
    m_uBodyColor = kOpaqueAlpha | (static_cast<unsigned int>(static_cast<int>(vBodyColor.z)) << 16) |
                   ((static_cast<unsigned int>(static_cast<int>(vBodyColor.y)) << 8) & 0xffff) |
                   (static_cast<unsigned int>(static_cast<int>(vBodyColor.x)) & 0xff);
    m_uOutlineColor = kOpaqueAlpha | (static_cast<unsigned int>(static_cast<int>(vOutlineColor.z)) << 16) |
                      ((static_cast<unsigned int>(static_cast<int>(vOutlineColor.y)) << 8) & 0xffff) |
                      (static_cast<unsigned int>(static_cast<int>(vOutlineColor.x)) & 0xff);

    // Files are monitored for reload only when bit 0 of application byte 0xf4d is set.
    const unsigned char* pApplicationBytes = *reinterpret_cast<const unsigned char* const*>(g_pkApplication);
    if ((pApplicationBytes[0xf4d] & 1) != 0) {
        cvPath kPath;
        kPath.FromSystemLocale(kArrowSettingsPath);
        m_kSettingsWatch.SetFileToMonitor(kPath, kFileMonitorFlags);
    }
}

/// 0x088519f2
void ArrowVisSystem::Shutdown() {
    for (unsigned int uArrow = 0; uArrow < m_aArrows.size(); ++uArrow) {
        m_aArrows[uArrow].ReleaseBuffers();
    }
    FireGrafix::g_pRenderer->ReleaseCommandStream(m_pCommandStream);
    delete m_pShader;
    UnSubscribeToEvents();
}

/// 0x08851a66
void ArrowVisSystem::UnSubscribeToEvents() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    void (ArrowVisSystem::*pfnOnSpawn)(SpawnArrowEvent&) = &ArrowVisSystem::OnSpawnArrow;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnSpawn, sizeof(pfnOnSpawn));
    Fun_08852c76(&kDelegate);
    kDelegate.m_pObject = this;
    void (ArrowVisSystem::*pfnOnRemoveAll)(RemoveAllArrowsEvent&) = &ArrowVisSystem::OnRemoveAllArrows;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnRemoveAll, sizeof(pfnOnRemoveAll));
    Fun_08852a4a(&kDelegate);
    kDelegate.m_pObject = this;
    void (ArrowVisSystem::*pfnOnRemove)(RemoveArrowEvent&) = &ArrowVisSystem::OnRemoveArrow;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnRemove, sizeof(pfnOnRemove));
    Fun_0885281e(&kDelegate);
}

/// 0x08851ae2
void ArrowVisSystem::StartupGame() {
}

/// 0x08851ae4
void ArrowVisSystem::ShutdownGame() {
}

/// 0x08851ae6
void ArrowVisSystem::Update(float fDeltaTime) {
    if (m_kSettingsWatch.HasChanged()) {
        LoadSettingsFromXML();
        m_kSettingsWatch.ResetChange();
    }
    const unsigned int uNumArrows = m_aArrows.size();
    for (unsigned int uArrow = 0; uArrow < uNumArrows; ++uArrow) {
        ArrowVis& kArrow = m_aArrows[uArrow];
        if (kArrow.m_bHidden) {
            continue;
        }
        if (kArrow.m_bDirty) {
            kArrow.UpdateArrowBuffers(m_aVertices, m_aIndices);
        }
        // UpdateArrowBuffers clears m_bAnimating once the shaft reaches the head.
        if (kArrow.m_bAnimating) {
            kArrow.Update(fDeltaTime);
            kArrow.UpdateArrowBuffers(m_aVertices, m_aIndices);
        }
    }
}

/// 0x08851c24
void ArrowVisSystem::Render(unsigned int hRenderTarget) {
    m_pCommandStream->ClearStream();
    D3DXMATRIX mViewProj;
    D3DXMatrixMultiply(&mViewProj, &FrameGlobals::g_View[0], &FrameGlobals::g_Proj);
    float* pConsts = static_cast<float*>(FireGrafix::g_pArrowShaderConstants->LockGlobal(0));
    memcpy(pConsts, &mViewProj, sizeof(D3DXMATRIX));
    pConsts[kConstAlpha] = m_fAlpha;
    pConsts[kConstAmbient] = m_fAmbientLightLevel;
    FireGrafix::g_pArrowShaderConstants->Unlock(0);

    const unsigned int uNumArrows = m_aArrows.size();
    FGXVector4 vClear;
    vClear.x = 0.0f;
    vClear.y = 0.0f;
    vClear.z = 0.0f;
    vClear.w = 0.0f;
    // The same surfaces are submitted once per pass; only the alpha state differs.
    m_pCommandStream->BeginCommandSet(hRenderTarget, kFirstPassAlphaState, kArrowZState, 0, "Arrow Visualization", vClear, 1.0f, 0, 0, 0);
    for (unsigned int uArrow = 0; uArrow < uNumArrows; ++uArrow) {
        if (!m_aArrows[uArrow].m_bHidden) {
            m_pCommandStream->DrawSurface(m_aArrows[uArrow].m_hSurface, m_pShader->m_hInstance, 0);
        }
    }
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->BeginCommandSet(hRenderTarget, kSecondPassAlphaState, kArrowZState, 0, "Arrow Visualization", vClear, 1.0f, 0, 0, 0);
    for (unsigned int uArrow = 0; uArrow < uNumArrows; ++uArrow) {
        if (!m_aArrows[uArrow].m_bHidden) {
            m_pCommandStream->DrawSurface(m_aArrows[uArrow].m_hSurface, m_pShader->m_hInstance, 0);
        }
    }
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddAllCommandSetsToRenderQueue();
}

/// 0x08851ed2
unsigned int ArrowVisSystem::SpawnArrow(FGXVector3 const& vStart, FGXVector3 const& vEnd) {
    ArrowVis& kArrow = m_aArrows[0];
    kArrow.m_vStart = vStart;
    kArrow.m_vEnd.x = vEnd.x;
    kArrow.m_vEnd.y = vEnd.y;
    kArrow.m_vEnd.z = m_fTargetHeight;
    kArrow.m_bDirty = true;
    kArrow.m_bAnimating = true;
    kArrow.m_bHidden = false;
    kArrow.m_fProgress = 0.0f;
    kArrow.m_fGrowRate = 0.0f;
    return 0;
}

/// 0x08851f5a
void ArrowVisSystem::MoveArrow(unsigned int uArrow, FGXVector3 const& vStart, FGXVector3 const& vEnd) {
    if (uArrow < m_aArrows.size()) {
        m_aArrows[uArrow].m_vStart = vStart;
        m_aArrows[uArrow].m_vEnd = vEnd;
        m_aArrows[uArrow].m_bDirty = true;
    }
}

/// 0x08851fa8
void ArrowVisSystem::RemoveArrow(unsigned int uArrow) {
    if (uArrow < m_aArrows.size()) {
        m_aArrows[uArrow].m_bHidden = true;
    }
}

/// 0x08851fc2
void ArrowVisSystem::OnSpawnArrow(EventTemplate<Event_Int2Type<165>, SpawnArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const SpawnArrowEventData& kData = reinterpret_cast<const SpawnArrowEventData&>(kEvent);
    HEXVec kHex;
    kHex.x = static_cast<float>(kData.m_kFrom.x);
    kHex.y = static_cast<float>(kData.m_kFrom.y);
    kHex.z = static_cast<float>(kData.m_kFrom.z);
    kHex.w = 1.0f;
    const FGXVector4 vFrom = MapGlobals::g_pTerrainSampler->GetWorldLocation(kHex);
    kHex.x = static_cast<float>(kData.m_kTo.x);
    kHex.y = static_cast<float>(kData.m_kTo.y);
    kHex.z = static_cast<float>(kData.m_kTo.z);
    kHex.w = 1.0f;
    const FGXVector4 vTo = MapGlobals::g_pTerrainSampler->GetWorldLocation(kHex);
    const float fWorldWidth = WorldGlobals::g_WorldSize.x;

    // Of the three horizontally wrapped copies of the start, the last one on screen for camera 0 wins.
    float fStartX = vFrom.x;
    for (int iWrap = -1; iWrap != 2; ++iWrap) {
        const FGXVector3 vCandidate = {static_cast<float>(iWrap) * fWorldWidth + vFrom.x, vFrom.y, vFrom.z};
        if (ArrowVis::IsOnScreen(vCandidate, 0)) {
            fStartX = vCandidate.x;
        }
    }

    // The search starts with the distance of the copy one world to the left but never selects that copy.
    const float fDeltaY = vTo.y - vFrom.y;
    const float fDeltaY2 = fDeltaY * fDeltaY;
    float fCandidateX = vTo.x - fWorldWidth;
    float fBestDist2 = (fCandidateX - fStartX) * (fCandidateX - fStartX) + fDeltaY2;
    float fEndX = vTo.x;
    for (int iWrap = 0; iWrap != 2; ++iWrap) {
        fCandidateX += fWorldWidth;
        const float fDist2 = (fCandidateX - fStartX) * (fCandidateX - fStartX) + fDeltaY2;
        if (fDist2 < fBestDist2) {
            fEndX = fCandidateX;
            fBestDist2 = fDist2;
        }
    }

    ArrowVis& kArrow = m_aArrows[0];
    kArrow.m_vStart.x = fStartX;
    kArrow.m_vStart.y = vFrom.y;
    kArrow.m_vStart.z = vFrom.z;
    kArrow.m_vEnd.x = fEndX;
    kArrow.m_vEnd.y = vTo.y;
    kArrow.m_vEnd.z = m_fTargetHeight;
    kArrow.m_bDirty = true;
    kArrow.m_bAnimating = true;
    kArrow.m_bHidden = false;
    kArrow.m_fGrowRate = 0.0f;
    kArrow.m_fProgress = 0.0f;
}

/// 0x08852218
void ArrowVisSystem::OnRemoveArrow(EventTemplate<Event_Int2Type<167>, RemoveArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
}

/// 0x0885221a
void ArrowVisSystem::OnRemoveAllArrows(EventTemplate<Event_Int2Type<166>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    m_aArrows[0].m_bHidden = true;
}

/// 0x08852226
void ArrowVisSystem::ExtractColor(char const* pszText, FGXVector3& vColor) const {
    FStringFixedBufferA(szText, kColorParseLength);
    szText = pszText;
    FStringFixedBufferA(szToken, kColorParseLength);
    int iStart = 0;
    szText.TrimLeft();
    szText.TrimRight();
    if (szText.GetLength() <= 0) {
        return;
    }
    float* const apfChannel[3] = {&vColor.x, &vColor.y, &vColor.z};
    for (int iChannel = 0; iChannel < 3; ++iChannel) {
        // Only the first field is taken even when empty; later empty fields are skipped.
        do {
            szText.Extract(szToken, iStart, ",");
        } while (iChannel > 0 && szToken.GetLength() < 1);
        int iValue = szToken.ConvertToInt(10, true);
        if (iValue < 0) {
            iValue = 0;
        }
        *apfChannel[iChannel] = iValue > kColorChannelLimit ? kColorChannelMax : static_cast<float>(iValue);
    }
}
