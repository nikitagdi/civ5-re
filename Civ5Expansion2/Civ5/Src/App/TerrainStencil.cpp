/// origin guess
#include "Civ5/Src/App/TerrainStencil.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/TextureData.h"

static const float kFloatMax = 3.4028235e+38f;
static const float kHexHeightRatio = 0.866f;
static const unsigned int kEventMachineStride = 0xc00180;
static const unsigned int kStencilEventSlot = 0xc80;


/// 0x087fd1e0
TerrainStencil::TerrainStencil() {
    m_bStarted = false;
    m_iHeight = 0;
    m_iWidth = 0;
    field_14 = 0;
    field_10 = 0;
    field_18 = 0;
    field_1c = 0xfffffff;
    field_2c = 0;
    field_28 = 0;
    field_24 = 0;
    field_20 = 0;
}

/// 0x087fd22e
void TerrainStencil::StartupGame() {
    const cvTerrainSampler* sampler = MapGlobals::g_pTerrainSampler;
    int samplerWidth = sampler->m_iHeightMapWidth;
    int samplerHeight = sampler->m_iHeightMapHeight;
    int dims[2] = {samplerWidth >> 1, samplerHeight >> 1};
    int blocksX = (dims[0] + (static_cast<unsigned int>(samplerWidth >> 31) >> 28)) >> 4;
    int blocksY = (dims[1] + (static_cast<unsigned int>(samplerHeight >> 31) >> 28)) >> 4;
    int blocks = blocksY * blocksX;
    unsigned char clearValue = 0;
    Fun_087fd914(&m_iWidth, dims, &clearValue, (blocks + (static_cast<unsigned int>(blocks >> 31) >> 30)) >> 2);
    ClearStencil();
    m_bStarted = true;
    int machine = *reinterpret_cast<int*>(g_EventSystemLocalMachine);
    Fun_087fea4a(reinterpret_cast<char*>(g_EventSystemLocalMachine) + machine * kEventMachineStride + kStencilEventSlot);
}

/// 0x087fd2e2
void TerrainStencil::ClearStencil() {
    int halfWidth = m_iWidth >> 1;
    int halfHeight = m_iHeight >> 1;
    if (halfWidth < 16) {
        return;
    }
    int blocksX = (halfWidth + (static_cast<unsigned int>(m_iWidth >> 31) >> 28)) >> 4;
    int blocksY = (halfHeight + (static_cast<unsigned int>(m_iHeight >> 31) >> 28)) >> 4;
    for (int blockX = 0; blockX < blocksX; ++blockX) {
        if (halfHeight < 16) {
            continue;
        }
        for (int blockY = 0; blockY < blocksY; ++blockY) {
            int block[2] = {blockX, blockY};
            Fun_087fda4a(&m_iWidth, 0, block);
        }
    }
}

/// 0x087fd39c
void TerrainStencil::ShutdownGame() {
    Fun_087fd9f8(&m_iWidth);
    ClearStencil();
    m_bStarted = false;
}

/// 0x087fd3cc
void TerrainStencil::AddStencil(TerrainStencilSurface surface, unsigned char ucStencilBits, bool a3) {
    float minX = kFloatMax;
    float minY = kFloatMax;
    float maxX = -kFloatMax;
    float maxY = -kFloatMax;
    for (unsigned int i = 0; i < surface.m_uiVertexCount; ++i) {
        float x = surface.m_pVertices[i].x;
        float y = surface.m_pVertices[i].y;
        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
    }
    TerrainStencilVertex* converted[3] = {nullptr, nullptr, nullptr};
    Fun_087fdaa4(this, surface.m_pVertices, surface.m_uiVertexCount, converted);
    surface.m_pVertices = converted[0];
    int dims[2] = {m_iWidth, m_iHeight};
    field_5 = ucStencilBits;
    field_4 = a3 ? -1 : 0;
    Fun_087fdb22(dims, &surface, &field_4);
    int machine = *reinterpret_cast<int*>(g_EventSystemLocalMachine);
    TerrainStencilUpdatedData* update =
        Fun_087fe8d6(reinterpret_cast<char*>(g_EventSystemLocalMachine) + machine * kEventMachineStride + kStencilEventSlot);
    update->m_fMinX = minX;
    update->m_fMinY = minY;
    update->m_fMaxX = maxX;
    update->m_fMaxY = maxY;
    update->m_uiStencilBits = ucStencilBits;
    Fun_087fe06c(converted);
}

/// 0x087fd588
void TerrainStencil::AddStencil(TerrainStencilVertex const* pVertices, unsigned int uiVertexCount, unsigned char ucStencilBits, bool a4) {
    float minX = kFloatMax;
    float minY = kFloatMax;
    float maxX = -kFloatMax;
    float maxY = -kFloatMax;
    for (unsigned int i = 0; i < uiVertexCount; ++i) {
        float x = pVertices[i].x;
        float y = pVertices[i].y;
        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
    }
    TerrainStencilVertex* converted[3] = {nullptr, nullptr, nullptr};
    Fun_087fdaa4(this, pVertices, uiVertexCount, converted);
    int dims[2] = {m_iWidth, m_iHeight};
    field_5 = ucStencilBits;
    field_4 = a4 ? -1 : 0;
    Fun_087fe098(dims, converted[0], uiVertexCount / 3, &field_4);
    int machine = *reinterpret_cast<int*>(g_EventSystemLocalMachine);
    TerrainStencilUpdatedData* update =
        Fun_087fe8d6(reinterpret_cast<char*>(g_EventSystemLocalMachine) + machine * kEventMachineStride + kStencilEventSlot);
    update->m_fMinX = minX;
    update->m_fMinY = minY;
    update->m_fMaxX = maxX;
    update->m_fMaxY = maxY;
    update->m_uiStencilBits = ucStencilBits;
    Fun_087fe06c(converted);
}

/// 0x087fd746
unsigned char TerrainStencil::SampleWorldPosition(FGXVector2 const& vPos, unsigned char ucMask) const {
    const cvTerrainSampler* sampler = MapGlobals::g_pTerrainSampler;
    if (sampler == nullptr) {
        return 0;
    }
    float worldX = vPos.x + sampler->m_fOffsetX;
    float worldY = (vPos.y + sampler->m_fOffsetY) / kHexHeightRatio;
    int tileX = static_cast<int>(worldX);
    int tileY = static_cast<int>(worldY);
    int cell[2] = {tileX >> 1, tileY >> 1};
    if (((tileX | tileY) >> 1) < 0 || cell[0] >= m_iWidth || cell[1] >= m_iHeight) {
        return 0;
    }
    return Fun_087fe5d2(&m_iWidth, cell) & ucMask;
}

/// 0x087fd7d6
void TerrainStencil::SaveToFile(char const* szPath) const {
    FireGrafix::TextureData texture(0x32, static_cast<FireGrafix::_TextureType>(0), m_iWidth, m_iHeight, 1, 1, 0, nullptr, 0);
    unsigned char* row = texture.m_aMips[0].m_apFaces[0];
    for (int y = 0; y < m_iHeight; ++y) {
        int cell[2] = {0, y};
        for (int x = 0; x < m_iWidth; ++x) {
            cell[0] = x;
            row[x] = static_cast<unsigned char>(-Fun_087fe5d2(&m_iWidth, cell));
        }
        row += texture.m_aMips[0].m_uWidth;
    }
    texture.SaveDDS(szPath);
}
