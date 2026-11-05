/// origin guess
#include "Civ5/Src/App/Engine/cvMarshSimSystem.h"
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireWorks/FString.h"

namespace {

typedef GameCoreEventTemplate<Event_Int2Type<53>, MarshCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass> MarshCreatedEvent;
typedef GameCoreEventTemplate<Event_Int2Type<54>, MarshRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass> MarshRemovedEvent;

// A serial game-core event is a 4-byte header followed by its payload.
struct MarshRemovedMessage {
    unsigned int m_uHeader;
    MarshRemovedData m_kData;
};

// The stamp is 128 texels wide, centred on the hex's texel position.
const int kStampSize = 128;
const int kStampHalfSize = 64;
// Terrain types whose ID is restored when a marsh goes away: grass, plains, tundra.
const int kTerrainGrass = 0;
const int kTerrainPlains = 1;
const int kTerrainTundra = 3;

// The helper below is inlined in the binary.
const auto MakeStampSurface = [](cvMarshSimSystem const& kSystem, HEXVec_i const& kHex) -> BlitMaskedIDSurface {
    const D3DXVECTOR4 kHexPosition = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(kHex.z), 1.0f};
    D3DXVECTOR4 kTexel;
    D3DXVec4Transform(&kTexel, &kHexPosition, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
    BlitMaskedIDSurface kSurface;
    // The surface starts at the record, one byte before the copied texels.
    kSurface.m_pucMask = reinterpret_cast<unsigned char const*>(kSystem.m_kStamps.begin());
    kSurface.m_iWidth = kStampSize;
    kSurface.m_iHeight = kStampSize;
    kSurface.m_iX = static_cast<int>(kTexel.x) - kStampHalfSize;
    kSurface.m_iY = static_cast<int>(kTexel.y) - kStampHalfSize;
    return kSurface;
};

}

/// 0x087e6a30
cvMarshSimSystem::cvMarshSimSystem() {
    field_0 = 4;
    m_bGameStarted = false;
}

/// 0x087e6aa2
cvMarshSimSystem::~cvMarshSimSystem() {
}

/// 0x087e6af2
UNK_RET cvMarshSimSystem::Startup() {
    FStringA kPath;
    FStringA::FStringAFixedBuffer<256> kPathBuffer(kPath);
    kPath.Format("marsh_stamp.dds");
    FileLoadBuffer kBuffer;
    Fun_08669c48(&kBuffer, kPath);
    {
        FireGrafix::TextureData kTexture(kBuffer.m_pData, Fun_08669d48(&kBuffer), 0x15);
        if (kTexture.m_nMips == 0) {
            MessageBoxA(0, "Marsh stamp texture missing", "Marsh Error", MB_OK);
            exit(0);
        }
        if (kTexture.m_ulFormat != 0x32) {
            MessageBoxA(0, "Incorrect marsh stamp texture format (L8 required)", "Marsh Error", MB_OK);
            exit(0);
        }
        m_kStamps.setsize(m_kStamps.size() + 1);
        FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
        unsigned char* pDst = m_kStamps.back().m_aucMask;
        for (unsigned int uiRow = 0; uiRow < kMip.m_uWidth; ++uiRow) {
            for (unsigned int uiCol = 0; uiCol < kMip.m_uHeight; ++uiCol) {
                pDst[1 + uiCol] = kMip.m_apFaces[0][uiCol + kMip.m_uPitch * uiRow];
            }
            pDst += kMip.m_uHeight;
        }
        m_kPendingMarshes.clear();
        SubscribeToEvents();
    }
    Fun_08669d90(&kBuffer);
}

/// 0x087e6e16
UNK_RET cvMarshSimSystem::SubscribeToEvents() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    UNK_RET (cvMarshSimSystem::*pfnOnMarshAdded)(MarshCreatedEvent&) = &cvMarshSimSystem::OnMarshAdded;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnMarshAdded, sizeof(pfnOnMarshAdded));
    Fun_087e7eb2(&kDelegate);
    kDelegate.m_pObject = this;
    UNK_RET (cvMarshSimSystem::*pfnOnMarshRemoved)(MarshRemovedEvent&) = &cvMarshSimSystem::OnMarshRemoved;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnMarshRemoved, sizeof(pfnOnMarshRemoved));
    Fun_087e7b20(&kDelegate);
}

/// 0x087e6e74
UNK_RET cvMarshSimSystem::Shutdown() {
    UnsubscribeToEvents();
}

/// 0x087e6ed2
UNK_RET cvMarshSimSystem::UnsubscribeToEvents() {
    TerrainEventDelegate kDelegate;
    kDelegate.m_pObject = this;
    UNK_RET (cvMarshSimSystem::*pfnOnMarshAdded)(MarshCreatedEvent&) = &cvMarshSimSystem::OnMarshAdded;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnMarshAdded, sizeof(pfnOnMarshAdded));
    Fun_087e78f4(&kDelegate);
    kDelegate.m_pObject = this;
    UNK_RET (cvMarshSimSystem::*pfnOnMarshRemoved)(MarshRemovedEvent&) = &cvMarshSimSystem::OnMarshRemoved;
    memcpy(&kDelegate.m_pfnMethod, &pfnOnMarshRemoved, sizeof(pfnOnMarshRemoved));
    Fun_087e76c8(&kDelegate);
}

/// 0x087e6f30
UNK_RET cvMarshSimSystem::StartupGame() {
    m_bGameStarted = true;
    for (unsigned int i = 0; i < m_kPendingMarshes.size(); ++i) {
        OnMarshAdded(reinterpret_cast<MarshCreatedEvent&>(m_kPendingMarshes[i]));
    }
    m_kPendingMarshes.clear();
}

/// 0x087e6f7e
UNK_RET cvMarshSimSystem::OnMarshAdded(GameCoreEventTemplate<Event_Int2Type<53>, MarshCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    MarshCreatedEventRecord const& kRecord = reinterpret_cast<MarshCreatedEventRecord const&>(kEvent);
    if (!m_bGameStarted) {
        m_kPendingMarshes.push_back(kRecord);
        return;
    }
    const BlitMaskedIDSurface kSurface = MakeStampSurface(*this, kRecord.m_kData.m_kHex);
    BlitMaskedIDs(kSurface, g_aContinentPrefix[kRecord.m_kData.m_iContinent]);
    m_kMarshes.push_back(kRecord.m_kData);
}

/// 0x087e709a
UNK_RET cvMarshSimSystem::ShutdownGame() {
    m_bGameStarted = false;
}

/// 0x087e70a4
UNK_RET cvMarshSimSystem::Update(float fDeltaTime) {
}

/// 0x087e70a6
UNK_RET cvMarshSimSystem::BlitMaskedIDs(BlitMaskedIDSurface const& kSurface, unsigned int uiHexCode) {
    unsigned char ucTerrainIndex = 0xff;
    TerrainData* pTerrainData = g_TerrainSystem.field_27720;
    if (pTerrainData != 0) {
        for (unsigned int i = 0; i < pTerrainData->m_uiNumTerrains; ++i) {
            if (pTerrainData->m_aHexCode[i] == uiHexCode) {
                ucTerrainIndex = static_cast<unsigned char>(i);
                break;
            }
        }
    }
    MaskedIDPaint kPaint;
    kPaint.m_pSampler = MapGlobals::g_pTerrainSampler;
    kPaint.m_ucTerrainIndex = ucTerrainIndex;
    Fun_087e735c(&g_TerrainSystem.field_27718, kSurface, kPaint);
}

// Repaints the stamp with the hex's own terrain; other terrain types keep the marsh texels.
/// 0x087e7122
UNK_RET cvMarshSimSystem::OnMarshRemoved(GameCoreEventTemplate<Event_Int2Type<54>, MarshRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    HEXVec_i const& kHex = reinterpret_cast<MarshRemovedMessage const&>(kEvent).m_kData.m_kHex;
    for (FFastList<MarshCreatedData>::iterator it = m_kMarshes.begin(); it != m_kMarshes.end(); ++it) {
        if (it->m_kHex.x != kHex.x || it->m_kHex.y != kHex.y || it->m_kHex.z != kHex.z) {
            continue;
        }
        const int eTerrain = it->m_eTerrain;
        const int iContinent = it->m_iContinent;
        m_kMarshes.erase(it);
        const BlitMaskedIDSurface kSurface = MakeStampSurface(*this, kHex);
        if (eTerrain != kTerrainTundra && eTerrain != kTerrainPlains && eTerrain != kTerrainGrass) {
            return;
        }
        BlitMaskedIDs(kSurface, TerrainData::GetTerrainHexCodeFromType(eTerrain, iContinent));
        return;
    }
}
