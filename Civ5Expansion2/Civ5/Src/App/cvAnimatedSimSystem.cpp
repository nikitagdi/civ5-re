/// origin guess
#include <alloca.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/cvAnimatedSimSystem.h"
#include "Civ5/Src/App/cvFloodplainsSystem.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/HEXMatrix.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/Engine/cvDynamicGeometryVisSystem.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvLandmarkVis.h"
#include "Civ5/Src/App/Graphics/cvLandmarkVisSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlAttribute.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlElement.h"
#include "FirePlace/FireWorks/FXmlTraverser.h"

typedef FFastVector<unsigned short, true, 43u, 260u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator> DecalIndexVector;
typedef cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor> LandmarkVisHandle;

static const float kHalfPi = 1.5707964f;
// Stretch of the world y axis applied to every decal vertex.
static const float kDecalYScale = 1.1547344f;
// World units per texture repeat of a field decal.
static const float kFieldTextureSize = 25.6f;
// The second stencil ring lies this far outside the field outline.
static const float kStencilMargin = 5.0f;
static const unsigned char kStencilFieldValue = 1;
static const unsigned char kStencilMarginValue = 0x10;
static const unsigned char kStateHalfBuilt = 2;
static const unsigned char kStatePillaged = 8;
static const unsigned int kHalfBuiltGroup = 0;
static const unsigned int kWheatGroup = 5;
static const unsigned int kWheatHalfBuiltGroup = 6;
static const unsigned int kPillagedGroup = 7;
static const float kDefaultDecalPriority = 0.1f;
static const unsigned short kNoFarm = 0xffff;
static const unsigned int kMaxPlotsPerFarm = 50;
static const unsigned int kNoNode = 0x0fffffffu;
static const unsigned int kNoDecal = 0xffffffffu;
// Half size of the rect posted for a hex, in texture units.
static const float kHexHalfExtent = 64.0f;
// Plots smaller than this are not used as the landmark anchor.
static const float kMinAnchorPlotArea = 100.0f;
static const unsigned char kInitialFOWState = 2;
// g_EventSystemLocalMachine starts with the index of its active machine; machines are 0xc00180 bytes apart.
static const unsigned int kEventMachineStride = 0xc00180;
static const unsigned int kEventQueueOffset = 0xc80;

/// 0x08a1a894
UNK_RET cvFarmSimSystem::OnImprovementCreated(GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    const LandmarkCreatedEventRecord& kEvent = reinterpret_cast<const LandmarkCreatedEventRecord&>(a1);
    const HEXVec_i kHex = {kEvent.m_sX, kEvent.m_sY, -(kEvent.m_sX + kEvent.m_sY)};
    const HEXVec_i kCell = Fun_086616f0(m_kFarmGrid.m_mxForward, kHex);
    const int iColumn = kCell.x + (kCell.y >> 1);
    const SquareGridPolicy& kGameGrid = *reinterpret_cast<const SquareGridPolicy*>(WorldGlobals::g_GameGrid);
    if (kCell.y >= kGameGrid.m_iHeight || iColumn >= kGameGrid.m_iWidth || iColumn < 0 || kCell.y < 0) {
        return;
    }
    unsigned short& usCell = m_aausFarmIndex[kCell.y][iColumn];
    if (usCell == kNoFarm && kEvent.m_iImprovementType != m_iFarmLandmarkType) {
        return;
    }
    if (m_ucDeferFlags & 1) {
        FarmEventCallback kCallback = {this, nullptr, 0};
        UNK_RET (cvFarmSimSystem::*pfnHandler)(GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>&) = &cvFarmSimSystem::OnImprovementCreated;
        memcpy(&kCallback.m_pfnHandler, &pfnHandler, sizeof(kCallback.m_pfnHandler));
        Fun_087506be(m_aucEventQueue, &a1, &kCallback, 0);
        return;
    }
    if (usCell != kNoFarm) {
        DestroyImprovement(kHex);
        if (kEvent.m_iImprovementType != m_iFarmLandmarkType) {
            return;
        }
    }

    FarmData kFarm;
    kFarm.m_iLandmarkEntry = -1;
    kFarm.m_ucState = kEvent.m_ucState;
    kFarm.m_ucNumPlots = 0;
    kFarm.m_ucFOWState = kInitialFOWState;
    kFarm.m_ucEra = (unsigned char)kEvent.m_iEra;
    kFarm.m_ucPlayer = kEvent.m_ucPlayer;
    kFarm.m_uVisStyle = kEvent.m_usVisStyle & 0xf;
    kFarm.m_uDecalStyle = kEvent.m_usDecalStyle & 0xf;
    kFarm.m_bHasVis = 0;
    kFarm.m_bOnWheat = kEvent.m_iResourceType == m_iWheatLandmarkType;
    kFarm.m_bFree = 0;
    kFarm.m_uVariant = 0;
    kFarm.m_sX = kEvent.m_sX;
    kFarm.m_sY = kEvent.m_sY;
    kFarm.m_fAnchorX = 0.0f;
    kFarm.m_fAnchorY = 0.0f;
    kFarm.m_fAnchorZ = 0.0f;
    kFarm.m_hVis = 0xffffffffu;
    kFarm.m_pSmokeKey = 0;
    kFarm.m_uSmokeKeyHigh = 0;
    const unsigned int uEra = (unsigned char)kEvent.m_iEra;
    if (m_auNumFarmVariants[uEra] != 0) {
        kFarm.m_uVariant = rand_wincompatible() % m_auNumFarmVariants[uEra];
    }
    // Falls back to older eras until a model exists.
    for (int iEra = (int)uEra; iEra >= 0; --iEra) {
        kFarm.m_iLandmarkEntry = m_aaiFarmEntries[kFarm.m_uVariant][iEra];
        if (kFarm.m_iLandmarkEntry != -1) {
            break;
        }
    }
    const unsigned int uFarm = AllocFarm(&m_kFarms, kFarm) & 0xffff;
    usCell = (unsigned short)uFarm;
    PushBackPendingFarm(&m_kPendingFarms, uFarm);
}

/// 0x08a1b030
UNK_RET cvFarmSimSystem::OnPlotUpdateEnd(EventTemplate<Event_Int2Type<5>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    if (m_ucDeferFlags & 1) {
        FarmEventCallback kCallback = {this, nullptr, 0};
        UNK_RET (cvFarmSimSystem::*pfnHandler)(EventTemplate<Event_Int2Type<5>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>&) = &cvFarmSimSystem::OnPlotUpdateEnd;
        memcpy(&kCallback.m_pfnHandler, &pfnHandler, sizeof(kCallback.m_pfnHandler));
        Fun_08a1e17a(m_aucEventQueue, &a1, &kCallback, 0);
        return;
    }
    const short* psHex = reinterpret_cast<const short*>(&a1);
    cvLandmarkVisSystem* pVisSystem = reinterpret_cast<cvLandmarkVisSystem*>(g_cvLandmarkVisSystem);
    unsigned int uPending = 0;
    while (uPending < m_kPendingFarms.m_uSize) {
        const unsigned int uFarm = m_kPendingFarms.m_pData[uPending];
        if (uFarm < m_kFarms.m_uNumSlots && !m_kFarms.m_pData[uFarm].m_bFree) {
            FarmData& kFarm = m_kFarms.m_pData[uFarm];
            if (kFarm.m_sX != psHex[0] || kFarm.m_sY != psHex[1]) {
                ++uPending;
                continue;
            }
            if (kFarm.m_bHasVis) {
                pVisSystem->DestroyFromHandle(reinterpret_cast<LandmarkVisHandle&>(kFarm.m_hVis));
                kFarm.m_bHasVis = 0;
            }
            // Without a landmark model the farm stays pending.
            if (kFarm.m_iLandmarkEntry == -1) {
                ++uPending;
                continue;
            }
            const int iMachine = *reinterpret_cast<const int*>(g_EventSystemLocalMachine);
            FGXMathRect::Rect* pRect = QueueRectEvent(g_EventSystemLocalMachine + iMachine * kEventMachineStride + kEventQueueOffset);
            const HEXVec vHex = {(float)kFarm.m_sX, (float)kFarm.m_sY, (float)-(kFarm.m_sY + kFarm.m_sX), 1.0f};
            D3DXMATRIX kHexToTex;
            HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&kHexToTex), vHex);
            D3DXMatrixMultiply(&kHexToTex, &kHexToTex, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
            const D3DXVECTOR4 vOrigin = {0.0f, 0.0f, -0.0f, 1.0f};
            D3DXVECTOR4 vTex;
            D3DXVec4Transform(&vTex, &vOrigin, &kHexToTex);
            pRect->m_fMinX = (vTex.x - kHexHalfExtent) * WorldGlobals::g_TerrainScale.x + WorldGlobals::g_WorldMin.x;
            pRect->m_fMinY = (vTex.y - kHexHalfExtent) * WorldGlobals::g_TerrainScale.y + WorldGlobals::g_WorldMin.y;
            pRect->m_fMaxX = (vTex.x + kHexHalfExtent) * WorldGlobals::g_TerrainScale.x + WorldGlobals::g_WorldMin.x;
            pRect->m_fMaxY = (vTex.y + kHexHalfExtent) * WorldGlobals::g_TerrainScale.y + WorldGlobals::g_WorldMin.y;

            cvLandmarkVis_Init kInit;
            kInit.m_uiType = 0;
            kInit.m_uiVariantIndex = 0xffffffffu;
            kInit.m_ucFlags = 7;
            kInit.m_uiVariant = 5;
            kInit.field_450 = 0.0f;
            kInit.field_454 = 0.0f;
            kInit.m_uiElementCount = 0;
            FillLandmarkSimInfo(kFarm, kInit);
            if (kInit.m_uiElementCount != 0) {
                pVisSystem->InitHandle(reinterpret_cast<LandmarkVisHandle&>(kFarm.m_hVis), kInit);
                kFarm.m_bHasVis = 1;
            }
            UpdatePillageSmoke(this, kFarm);
        }
        m_kPendingFarms.m_pData[uPending] = m_kPendingFarms.m_pData[m_kPendingFarms.m_uSize - 1];
        --m_kPendingFarms.m_uSize;
    }
}

/// 0x08a1b402
UNK_RET cvFarmSimSystem::OnPlotPacketReceived(EventTemplate<Event_Int2Type<3>, EventPacketData<FGXVector2, PlotPacketData>, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    const PlotPacketRecord& kPacket = reinterpret_cast<const PlotPacketRecord&>(a1);
    const HEXVec_i kHex = kPacket.m_kHex;
    // The binary repeats the same validity test.
    if (!Fun_08664aae(m_kPlotGrid, kHex) || !Fun_08664aae(m_kPlotGrid, kHex)) {
        return;
    }
    const HEXVec_i kCell = Fun_086616f0(m_kFarmGrid.m_mxForward, kHex);
    const unsigned int uFarm = m_aausFarmIndex[kCell.y][kCell.x + ((unsigned int)kCell.y >> 1)];
    if (uFarm == kNoFarm) {
        return;
    }
    if (m_ucDeferFlags & 1) {
        FarmEventCallback kCallback = {this, nullptr, 0};
        UNK_RET (cvFarmSimSystem::*pfnHandler)(EventTemplate<Event_Int2Type<3>, EventPacketData<FGXVector2, PlotPacketData>, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>&) = &cvFarmSimSystem::OnPlotPacketReceived;
        memcpy(&kCallback.m_pfnHandler, &pfnHandler, sizeof(kCallback.m_pfnHandler));
        Fun_08a1e33c(m_aucEventQueue, &a1, &kCallback, 0);
        return;
    }

    for (unsigned int uNode = m_kAssemblies.m_uFirst; uNode != kNoNode; uNode = m_kAssemblies.m_pNodes[uNode].m_uNext) {
        AssemblyNode& kNode = m_kAssemblies.m_pNodes[uNode];
        if (kNode.m_kData.m_iPacketId != kPacket.m_iPacketId) {
            continue;
        }
        if (AppendPlotPacket(&kNode.m_kData.m_kState, kPacket, &m_kPlots.m_pData[kNode.m_kData.m_uPlot].m_Polygon)) {
            FarmData& kFarm = m_kFarms.m_pData[uFarm];
            if (kFarm.m_ucNumPlots < kMaxPlotsPerFarm) {
                kFarm.m_ausPlots[kFarm.m_ucNumPlots++] = (unsigned short)m_kAssemblies.m_pNodes[uNode].m_kData.m_uPlot;
                if (kFarm.m_ucNumPlots != 0) {
                    CreateFarmDecal(uFarm, kFarm.m_ucNumPlots - 1);
                }
            }
            ++m_uNumCompletedPlots;
            EraseAssemblies(&m_kAssemblies, uNode, m_kAssemblies.m_pNodes[uNode].m_uNext);
        }
        return;
    }

    FarmPlot kTemplate;
    kTemplate.m_Polygon.m_pPool = &m_PolygonNodes;
    kTemplate.m_Polygon.m_uNodeCount = 0;
    kTemplate.m_Polygon.m_uHead = kNoNode;
    kTemplate.m_Polygon.m_uTail = kNoNode;
    kTemplate.m_Polygon.m_uSize = 0;
    kTemplate.m_iPacketId = kPacket.m_iPacketId;
    kTemplate.m_usFarm = kNoFarm;
    kTemplate.m_hDecal = kNoDecal;
    kTemplate.m_hOverlayDecal = kNoDecal;
    const unsigned int uPlot = AllocPlot(&m_kPlots, kTemplate);
    FarmPlot& kPlot = m_kPlots.m_pData[uPlot];
    kPlot.m_hDecal = kNoDecal;
    kPlot.m_hOverlayDecal = kNoDecal;
    kPlot.m_usFarm = (unsigned short)uFarm;
    kPlot.m_iPacketId = kPacket.m_iPacketId;
    AssemblyState kState = {-1, -1};
    if (AppendPlotPacket(&kState, kPacket, &m_kPlots.m_pData[uPlot].m_Polygon)) {
        FarmData& kFarm = m_kFarms.m_pData[uFarm];
        if (kFarm.m_ucNumPlots < kMaxPlotsPerFarm) {
            kFarm.m_ausPlots[kFarm.m_ucNumPlots++] = (unsigned short)uPlot;
            if (kFarm.m_ucNumPlots != 0) {
                CreateFarmDecal(uFarm, kFarm.m_ucNumPlots - 1);
            }
        }
        ++m_uNumCompletedPlots;
    } else {
        const PlotAssembly kAssembly = {kPacket.m_iPacketId, uPlot, kState};
        LinkAssemblyAtEnd(&m_kAssemblies, AllocAssembly(&m_kAssemblies, kAssembly));
    }
}

/// 0x08a1bc12
UNK_RET cvFarmSimSystem::Startup(bool bStartupFlag) {
    m_bStartupFlag = bStartupFlag;
    for (unsigned int uVariant = 0; uVariant < 3; ++uVariant) {
        for (unsigned int uEra = 0; uEra < 5; ++uEra) {
            m_aaiFarmEntries[uVariant][uEra] = -1;
        }
    }
    m_pLandmarkLibrary = reinterpret_cast<cvLandmarkLibrary*>(&reinterpret_cast<cvLandmarkVisSystem*>(g_cvLandmarkVisSystem)->m_pTypeRecords);
    m_kFarmGrid.m_mxForward = m_kFarmGrid.m_kTransform.matrix();
    m_kFarmGrid.m_mxInverse = m_kFarmGrid.m_kTransform.inv_matrix();
    SubscribeToEvents();
    m_hBlankTexture = FireGrafix::g_pRenderer->LoadTexture("blank_texture.dds");
    LoadFarmDecalTextures("farm_types.xml");
}

/// 0x08a1bcf6
UNK_RET cvFarmSimSystem::LoadFarmDecalTextures(char const* pszFileName) {
    if (pszFileName == 0) {
        return;
    }
    FXmlDocument kDocument;
    kDocument.Load(pszFileName);
    FXmlTraverser kElement;
    kDocument.GetRootElement(kElement);
    kElement.GetChildCount(1);
    FXmlAttribute kAttribute;
    FStringA strText;
    FStringA::FStringAFixedBuffer<256> kTextBuffer(strText);
    for (unsigned int uGroup = 0; uGroup < 8; ++uGroup) {
        strText = s_aszFarmDecalGroupNames[uGroup];
        if (!kElement.GetFirstChild(kElement, strText.c_str(), strText.GetLength())) {
            continue;
        }
        m_aDecalGroups[uGroup].m_usFirst = (unsigned short)m_kDecalTypes.m_uSize;
        kElement.GetFirstChild(kElement);
        do {
            DecalType kType;
            kType.m_hAlbedo = 0;
            if (kElement.GetFirstAttribute(kAttribute, "albedo", 6) && kAttribute.GetVal(strText)) {
                kType.m_hAlbedo = FireGrafix::g_pRenderer->LoadTexture(strText.c_str());
            }
            kType.m_hHeightmap = 0;
            if (kElement.GetFirstAttribute(kAttribute, "heightmap", 9) && kAttribute.GetVal(strText)) {
                kType.m_hHeightmap = FireGrafix::g_pRenderer->LoadTexture(strText.c_str());
            }
            if (kElement.GetFirstAttribute(kAttribute, "spec", 4) && kAttribute.GetVal(strText)) {
                kType.m_hSpec = FireGrafix::g_pRenderer->LoadTexture(strText.c_str());
            } else {
                kType.m_hSpec = m_hBlankTexture;
            }
            float fPriority;
            if (kElement.GetFirstAttribute(kAttribute, "priority", 8) && kAttribute.GetVal(fPriority)) {
                kType.m_fPriority = fPriority;
            } else {
                kType.m_fPriority = kDefaultDecalPriority;
            }
            PushBackDecalType(&m_kDecalTypes, kType);
        } while (kElement.GetNextSibling(kElement));
        m_aDecalGroups[uGroup].m_usCount = (unsigned short)(m_kDecalTypes.m_uSize - m_aDecalGroups[uGroup].m_usFirst);
        kElement.GetParent(kElement);
        kElement.GetParent(kElement);
    }
}

/// 0x08a1c214
void cvFarmSimSystem::FillLandmarkSimInfo(cvFarmSimSystem::FarmData& kFarm, cvLandmarkVis_Init& kInit) {
    kInit.m_uiType = (unsigned int)kFarm.m_iLandmarkEntry;
    kInit.m_uiVariantIndex = 0xffffffffu;
    kInit.m_uiVariant = kFarm.m_uVisStyle;
    kInit.m_ucFlags = (unsigned char)((kInit.m_ucFlags & 0xc0) | (unsigned char)(kFarm.m_ucState << 2) | (kFarm.m_ucFOWState & 3));
    const FGXMathRect::Rect kBounds = reinterpret_cast<const LayoutFieldState*>(g_LayoutState)->m_kFieldBounds;
    const HEXVec vHex = {(float)kFarm.m_sX, (float)kFarm.m_sY, (float)-(kFarm.m_sY + kFarm.m_sX), 1.0f};
    const FGXVector4 vCentre = m_pTerrainSampler->GetWorldLocation(vHex);

    // The anchor is the centroid nearest the hex centre among plots of at least kMinAnchorPlotArea.
    FGXVector2 vAnchor = {vCentre.x, vCentre.y};
    if (kFarm.m_ucNumPlots != 0) {
        float fBestDistance = FLT_MAX;
        for (unsigned int uPlot = 0; uPlot < kFarm.m_ucNumPlots; ++uPlot) {
            FarmPolygon& kPolygon = m_kPlots.m_pData[kFarm.m_ausPlots[uPlot]].m_Polygon;
            const FGXVector2 vCentroid = POLY_Centroid(kPolygon.begin(), kPolygon.end());
            const float fArea = POLY_SignedArea(kPolygon.begin(), kPolygon.end());
            if (fabsf(fArea) >= kMinAnchorPlotArea) {
                const float fDX = vCentroid.x - vCentre.x;
                const float fDY = vCentroid.y - vCentre.y;
                const float fDistance = fDX * fDX + fDY * fDY;
                if (fDistance < fBestDistance) {
                    vAnchor = vCentroid;
                    fBestDistance = fDistance;
                }
            }
        }
    }
    const float fHeight = m_pTerrainSampler->GetWorldHeight(vAnchor);

    if (m_pLandmarkLibrary != 0) {
        const cvLandmarkLibrary::cvLandmarkLibraryEntry* pEntry = 0;
        if (kInit.m_uiType < m_pLandmarkLibrary->m_uiNumEntries) {
            pEntry = &m_pLandmarkLibrary->m_pEntries[kInit.m_uiType];
        }
        FGXVector2 vSample;
        vSample.x = fmaxf(fminf(vAnchor.x, kBounds.m_fMaxX - 1.0f), kBounds.m_fMinX + 1.0f);
        vSample.y = fmaxf(fminf(vAnchor.y, kBounds.m_fMaxY - 1.0f), kBounds.m_fMinY + 1.0f);
        const FGXVector2 vField = Fun_087bbab8(reinterpret_cast<const LayoutFieldState*>(g_LayoutState), vSample);
        const float fFieldLength = sqrtf(vField.x * vField.x + vField.y * vField.y);
        float fDirX = 0.0f;
        float fDirY = 0.0f;
        if (fFieldLength != 0.0f) {
            fDirX = vField.x / fFieldLength;
            fDirY = vField.y / fFieldLength;
        }
        const float fHeading = atan2f(fDirY, fDirX);
        D3DXMATRIX kTransform;
        D3DXMatrixRotationZ(&kTransform, fHeading + (float)((int)rand_wincompatible() % 4) * kHalfPi);
        kTransform._41 = vAnchor.x;
        kTransform._42 = vAnchor.y;
        kTransform._43 = fHeight;
        const unsigned int uNumPieces = pEntry->m_uiNumPieces;
        for (unsigned int uPiece = 0; uPiece < uNumPieces; ++uPiece) {
            memcpy(kInit.m_aElements[uPiece].m_afMatrix, &kTransform, sizeof(kInit.m_aElements[uPiece].m_afMatrix));
            kInit.m_aElements[uPiece].m_uiElementDef = uPiece;
        }
        kInit.m_uiElementCount = uNumPieces;
        kInit.field_450 = vAnchor.x;
        kInit.field_454 = vAnchor.y;
    }
    kFarm.m_fAnchorX = vAnchor.x;
    kFarm.m_fAnchorY = vAnchor.y;
    kFarm.m_fAnchorZ = fHeight;
}

/// 0x08a1c99c
UNK_RET cvFarmSimSystem::DestroyImprovement(HEXVec_i kHex) {
    const HEXVec_i kCell = Fun_086616f0(m_kFarmGrid.m_mxForward, kHex);
    const int iColumn = kCell.x + (kCell.y >> 1);
    const SquareGridPolicy& kGameGrid = *reinterpret_cast<const SquareGridPolicy*>(WorldGlobals::g_GameGrid);
    if (kCell.y >= kGameGrid.m_iHeight || iColumn >= kGameGrid.m_iWidth || iColumn < 0 || kCell.y < 0) {
        return;
    }
    unsigned short& usCell = m_aausFarmIndex[kCell.y][iColumn];
    const unsigned int uFarm = usCell;
    if (uFarm == kNoFarm) {
        return;
    }
    void* pQueue = g_EventSystemLocalMachine + *reinterpret_cast<const int*>(g_EventSystemLocalMachine) * kEventMachineStride + kEventQueueOffset;
    FarmData& kFarm = m_kFarms.m_pData[uFarm];
    if (kFarm.m_pSmokeKey != 0 || kFarm.m_uSmokeKeyHigh != 0) {
        unsigned int* puKey = Fun_087bc5a2(pQueue);
        puKey[0] = (unsigned int)(size_t)kFarm.m_pSmokeKey;
        puKey[1] = kFarm.m_uSmokeKeyHigh;
        puKey[3] = 0;
        puKey[2] = 0;
        kFarm.m_uSmokeKeyHigh = 0;
        kFarm.m_pSmokeKey = 0;
    }
    if (kFarm.m_bHasVis) {
        reinterpret_cast<cvLandmarkVisSystem*>(g_cvLandmarkVisSystem)->DestroyFromHandle(reinterpret_cast<LandmarkVisHandle&>(kFarm.m_hVis));
        kFarm.m_bHasVis = 0;
    }
    for (unsigned int uPlot = 0; uPlot < kFarm.m_ucNumPlots; ++uPlot) {
        ReleaseFarmDecal(uFarm, uPlot);
        PaintRoadNetwork(uFarm, uPlot, false);
        FreePlot(&m_kPlots, kFarm.m_ausPlots[uPlot]);
    }

    // Texture-space centre of a hex to the world rect of +-kHexHalfExtent around it.
    auto HexWorldRect = [](float fX, float fY, float fZ) {
        const HEXVec vHex = {fX, fY, fZ, 1.0f};
        D3DXMATRIX kHexToTex;
        HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&kHexToTex), vHex);
        D3DXMatrixMultiply(&kHexToTex, &kHexToTex, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
        const D3DXVECTOR4 vOrigin = {0.0f, 0.0f, -0.0f, 1.0f};
        D3DXVECTOR4 vTex;
        D3DXVec4Transform(&vTex, &vOrigin, &kHexToTex);
        FGXMathRect::Rect kRect;
        kRect.m_fMinX = (vTex.x - kHexHalfExtent) * WorldGlobals::g_TerrainScale.x + WorldGlobals::g_WorldMin.x;
        kRect.m_fMinY = (vTex.y - kHexHalfExtent) * WorldGlobals::g_TerrainScale.y + WorldGlobals::g_WorldMin.y;
        kRect.m_fMaxX = (vTex.x + kHexHalfExtent) * WorldGlobals::g_TerrainScale.x + WorldGlobals::g_WorldMin.x;
        kRect.m_fMaxY = (vTex.y + kHexHalfExtent) * WorldGlobals::g_TerrainScale.y + WorldGlobals::g_WorldMin.y;
        return kRect;
    };
    FGXMathRect::Rect kRect = HexWorldRect((float)kFarm.m_sX, (float)kFarm.m_sY, (float)-(kFarm.m_sY + kFarm.m_sX));
    // The neighbours repaint their stencil, which the removed margin rings may have cleared.
    for (unsigned int uDir = 0; uDir < 6; ++uDir) {
        const HEXVec_i kNeighbour = {kHex.x + HEXVec_i::TRANSLATIONS[uDir].x, kHex.y + HEXVec_i::TRANSLATIONS[uDir].y, kHex.z + HEXVec_i::TRANSLATIONS[uDir].z};
        if (!Fun_08664aae(m_kFarmGrid, kNeighbour)) {
            continue;
        }
        const FGXMathRect::Rect kNeighbourRect = HexWorldRect((float)kNeighbour.x, (float)kNeighbour.y, (float)kNeighbour.z);
        kRect.m_fMinX = kRect.m_fMinX < kNeighbourRect.m_fMinX ? kRect.m_fMinX : kNeighbourRect.m_fMinX;
        kRect.m_fMinY = kRect.m_fMinY < kNeighbourRect.m_fMinY ? kRect.m_fMinY : kNeighbourRect.m_fMinY;
        kRect.m_fMaxX = kRect.m_fMaxX > kNeighbourRect.m_fMaxX ? kRect.m_fMaxX : kNeighbourRect.m_fMaxX;
        kRect.m_fMaxY = kRect.m_fMaxY > kNeighbourRect.m_fMaxY ? kRect.m_fMaxY : kNeighbourRect.m_fMaxY;
        const HEXVec_i kNeighbourCell = Fun_086616f0(m_kFarmGrid.m_mxForward, kNeighbour);
        const unsigned int uNeighbour = m_aausFarmIndex[kNeighbourCell.y][kNeighbourCell.x + ((unsigned int)kNeighbourCell.y >> 1)];
        if (uNeighbour == kNoFarm) {
            continue;
        }
        for (unsigned int uPlot = 0; uPlot < m_kFarms.m_pData[uNeighbour].m_ucNumPlots; ++uPlot) {
            PaintRoadNetwork(uNeighbour, uPlot, true);
        }
    }
    *QueueRectEvent(pQueue) = kRect;
    usCell = kNoFarm;
    FreeFarm(&m_kFarms, uFarm);
}

/// 0x08a1d004
void cvFarmSimSystem::ReleaseFarmDecal(unsigned int uFarm, unsigned int uPlot) {
    FarmPlot& kPlot = m_kPlots.m_pData[m_kFarms.m_pData[uFarm].m_ausPlots[uPlot]];
    if (kPlot.m_hDecal != kNoDecal) {
        g_TerrainSystem.m_DecalLayer.DestroyDecal(kPlot.m_hDecal);
        kPlot.m_hDecal = kNoDecal;
    }
    if (kPlot.m_hOverlayDecal != kNoDecal) {
        g_TerrainSystem.m_DecalLayer.DestroyDecal(kPlot.m_hOverlayDecal);
        kPlot.m_hOverlayDecal = kNoDecal;
    }
}

/// 0x08a1d08e
void cvFarmSimSystem::PaintRoadNetwork(unsigned int uFarm, unsigned int uPlot, bool bPaint) {
    FarmPolygon kPolygon = m_kPlots.m_pData[m_kFarms.m_pData[uFarm].m_ausPlots[uPlot]].m_Polygon;
    TerrainDecalLayer& kLayer = g_TerrainSystem.m_DecalLayer;
    kLayer.m_scratchIndices.m_uiNumIndices = 0;
    const FarmPolygon& kConstPolygon = kPolygon;
    // The layer's index scratch has the FFastVector layout the triangulator fills.
    if (POLY_Triangulate<FGXVector2>(kConstPolygon.begin(), kConstPolygon.end(), reinterpret_cast<DecalIndexVector&>(kLayer.m_scratchIndices)) == 0) {
        return;
    }
    AddWorldSpaceMeshToTerrainStencil(kPolygon.begin(), kPolygon.m_uSize, kLayer.m_scratchIndices.m_pIndices, kLayer.m_scratchIndices.m_uiNumIndices, kStencilFieldValue, true);

    const unsigned int uNumPoints = kPolygon.m_uSize;
    FGXVector2* pMargin = static_cast<FGXVector2*>(alloca(uNumPoints * sizeof(FGXVector2)));
    const FGXVector2 vCentre = POLY_Centroid(kPolygon.begin(), kPolygon.end());
    FGXVector2* pOut = pMargin;
    for (FarmPolygonIterator kIt = kPolygon.begin(); kIt != kPolygon.end(); FarmPolygonIteratorNext(&kIt)) {
        const FGXVector2& vPoint = *kIt;
        const float fDX = vPoint.x - vCentre.x;
        const float fDY = vPoint.y - vCentre.y;
        const float fLength = sqrtf(fDX * fDX + fDY * fDY);
        pOut->x = fDX / fLength * kStencilMargin + vPoint.x;
        pOut->y = fDY / fLength * kStencilMargin + vPoint.y;
        ++pOut;
    }
    AddWorldSpaceMeshToTerrainStencil(pMargin, uNumPoints, kLayer.m_scratchIndices.m_pIndices, kLayer.m_scratchIndices.m_uiNumIndices, kStencilMarginValue, bPaint);
}

/// 0x08a1d4ac
void cvFarmSimSystem::CreateFarmDecal(unsigned int uFarm, unsigned int uPlot) {
    PaintRoadNetwork(uFarm, uPlot, true);
    FarmData& kFarm = m_kFarms.m_pData[uFarm];
    FarmPlot& kPlot = m_kPlots.m_pData[kFarm.m_ausPlots[uPlot]];
    FarmPolygon kPolygon = kPlot.m_Polygon;
    TerrainDecalLayer& kLayer = g_TerrainSystem.m_DecalLayer;
    kLayer.m_scratchIndices.m_uiNumIndices = 0;
    const FarmPolygon& kConstPolygon = kPolygon;
    if (POLY_Triangulate<FGXVector2>(kConstPolygon.begin(), kConstPolygon.end(), reinterpret_cast<DecalIndexVector&>(kLayer.m_scratchIndices)) == 0) {
        return;
    }
    const float fOffsetX = m_pTerrainSampler->m_fOffsetX;
    const float fOffsetY = m_pTerrainSampler->m_fOffsetY;
    const FGXVector2 vCentre = POLY_Centroid(kPolygon.begin(), kPolygon.end());
    // The bounds are computed and never read.
    POLY_ComputeBounds(kPolygon.begin(), kPolygon.m_uSize);

    // The rows of the field follow the layout heading at the centroid; one field in three is turned a quarter.
    const FGXVector2 vField = Fun_087bbab8(reinterpret_cast<const LayoutFieldState*>(g_LayoutState), vCentre);
    const float fFieldLength = sqrtf(vField.x * vField.x + vField.y * vField.y);
    float fDirX = 0.0f;
    float fDirY = 0.0f;
    if (fFieldLength != 0.0f) {
        fDirX = vField.x / fFieldLength;
        fDirY = vField.y / fFieldLength;
    }
    const float fHeading = atan2f(fDirY, fDirX);
    const float fTurn = ((int)rand_wincompatible() % 3 <= 0) ? kHalfPi : -0.0f;
    D3DXMATRIX kRotation;
    D3DXMatrixRotationZ(&kRotation, fTurn - fHeading);

    const unsigned int uNumVerts = kPolygon.m_uSize;
    DecalVertex* pVerts = static_cast<DecalVertex*>(alloca(uNumVerts * 2 * sizeof(DecalVertex)));
    DecalVertex* pPillagedVerts = pVerts + uNumVerts;
    DecalVertex* pVert = pVerts;
    for (FarmPolygon::const_iterator kIt = kConstPolygon.begin(); kIt != kConstPolygon.end(); FarmPolygonConstIteratorNext(&kIt)) {
        const FGXVector2& vPoint = *kIt;
        pVert->m_fX = vPoint.x + fOffsetX;
        pVert->m_fY = (vPoint.y + fOffsetY) * kDecalYScale;
        const D3DXVECTOR4 vUV = {(vPoint.x - vCentre.x) / kFieldTextureSize + 0.5f, (vPoint.y - vCentre.y) / kFieldTextureSize + 0.5f, 0.0f, 1.0f};
        D3DXVECTOR4 vRotated;
        D3DXVec4Transform(&vRotated, &vUV, &kRotation);
        pVert->field_8 = vRotated.x;
        pVert->field_c = vRotated.y;
        pVert[uNumVerts] = *pVert;
        ++pVert;
    }

    const FarmDecalGroup* pGroup;
    switch (kFarm.m_uDecalStyle) {
    case 1:
        pGroup = &m_aDecalGroups[2];
        break;
    case 2:
        pGroup = &m_aDecalGroups[3];
        break;
    case 3:
        pGroup = &m_aDecalGroups[1];
        break;
    case 4:
        pGroup = &m_aDecalGroups[4];
        break;
    default:
        return;
    }
    if (kFarm.m_ucEra == 0) {
        return;
    }
    if (kFarm.m_bOnWheat) {
        pGroup = &m_aDecalGroups[kFarm.m_ucState == kStateHalfBuilt ? kWheatHalfBuiltGroup : kWheatGroup];
    } else if (kFarm.m_ucState == kStateHalfBuilt) {
        pGroup = &m_aDecalGroups[kHalfBuiltGroup];
    }
    if (pGroup->m_usCount == 0) {
        return;
    }
    const DecalType kType = m_kDecalTypes.m_pData[(int)rand_wincompatible() % (int)pGroup->m_usCount + pGroup->m_usFirst];
    kPlot.m_hDecal = kLayer.CreateDecal(kType, pVerts, uNumVerts, kLayer.m_scratchIndices.m_pIndices, kLayer.m_scratchIndices.m_uiNumIndices / 3, false);
    if (kFarm.m_ucState == kStatePillaged) {
        const FarmDecalGroup& kPillaged = m_aDecalGroups[kPillagedGroup];
        const DecalType kPillagedType = m_kDecalTypes.m_pData[(int)rand_wincompatible() % (int)kPillaged.m_usCount + kPillaged.m_usFirst];
        kPlot.m_hOverlayDecal = kLayer.CreateDecal(kPillagedType, pPillagedVerts, uNumVerts, kLayer.m_scratchIndices.m_pIndices, kLayer.m_scratchIndices.m_uiNumIndices / 3, false);
    }
}
