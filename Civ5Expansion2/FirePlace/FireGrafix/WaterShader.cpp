/// origin guess
#include "FirePlace/FireGrafix/WaterShader.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/TextureData.h"

namespace FireGrafix {
/// 0x08781526
WaterShader::WaterShader() {
    m_uPackageInstance = 0;
    unk_008[0] = unk_008[1] = unk_008[2] = unk_008[3] = 0;
    unsigned int auParamSetIdsA[6] = {};
    unsigned int auParamSetIdsB[4] = {};
    unsigned int auParamSetIdsC[4] = {};
    m_pGlobalData = g_pWaterGlobalData;
    auParamSetIdsA[0] = g_pWaterGlobalData->field_34;
    unsigned int const uInstanceSet =
        FGXConstantSetManager::CreateConstantSet(static_cast<CONSTANTTYPE>(0), 0x80, 0, 4, 4, 0, 4, 0);
    m_pInstanceData = &reinterpret_cast<FGXConstantSet*>(&CRenderer9::m_kCSManager[0x2020c8])[uInstanceSet];
    m_pszName = "WaterShader";
    m_iPackageId = 0x5b;
    m_uPackageInstance = g_pRenderer->CreateShaderPackageInstance(auParamSetIdsA, auParamSetIdsB, auParamSetIdsC,
                                                                  m_pInstanceData->field_34, 1, 0, 0, 0x5b);
}
}
