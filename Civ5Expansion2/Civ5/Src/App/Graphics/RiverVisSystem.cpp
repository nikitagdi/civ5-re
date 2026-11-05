/// origin binary
#include "Civ5/Src/App/Graphics/RiverVisSystem.h"

/// 0x088d9490
RiverVisSystem::RiverVisSystem()
{
    m_pLeftBanks = 0;
    m_pRightBanks = 0;
    m_pCommandStream = 0;
    m_pHeights = 0;
    m_hSurface = 0;
    m_pShader = 0;
    m_hIndexBuffer = 0;
    m_hVertexBuffer = 0;
    m_uiTriangleCount = 0;
    m_hBlankTexture = 0;
    m_bReady = false;
}

/// 0x088da586
UNK_RET RiverVisSystem::DestroyFromHandle(cvGeneralRefHandle<RiverVisSystem, unsigned int, RiverVisSystemDataDef, RiverVisSystem_Accessor>& a1)
{
    m_pHeights = 0;
    m_pLeftBanks = 0;
    m_pRightBanks = 0;
    m_bReady = false;
}

/// 0x088da5a4
UNK_RET RiverVisSystem::GetGeometry(unsigned int& hVertexBuffer, unsigned int& hIndexBuffer, unsigned int& uiTriangleCount)
{
    hVertexBuffer = m_hVertexBuffer;
    hIndexBuffer = m_hIndexBuffer;
    uiTriangleCount = m_uiTriangleCount;
}
