/// origin guess
#include "Civ5/Src/App/Graphics/TerrainCellStatJob.h"
#include "FirePlace/FireGrafix/CRenderer9.h"

namespace {

const unsigned int kBaseVertexFormat = 0x10;
const float kBaseVertexZ = 14.0f;

// 24-byte vertex of format 0x10: position, four zero halves, packed half2 texcoord.
struct BaseTerrainVertex {
    float x;
    float y;
    float z;
    D3DXFLOAT16 zero[4];
    D3DXFLOAT16 u;
    D3DXFLOAT16 v;
};

}

/// 0x08917ad8
unsigned int CreateBaseTerrainCellVB(unsigned int uWidth, unsigned int uHeight, unsigned int uUvBorder) {
    const unsigned int hVB = FireGrafix::g_pRenderer->CreateStaticVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(kBaseVertexFormat), uWidth * uHeight, 0, 0);
    BaseTerrainVertex* pVertices = static_cast<BaseTerrainVertex*>(FireGrafix::g_pRenderer->LockStaticVB(hVB, 0));
    const float fUDivisor = static_cast<float>((uWidth - 1) + uUvBorder);
    const float fVDivisor = static_cast<float>((uHeight - 1) + uUvBorder);
    for (unsigned int y = 0; y < uHeight; ++y) {
        for (unsigned int x = 0; x < uWidth; ++x) {
            BaseTerrainVertex& kVertex = pVertices[y * uWidth + x];
            kVertex.x = static_cast<float>(x);
            kVertex.y = static_cast<float>(y);
            kVertex.z = kBaseVertexZ;
            const float fU = static_cast<float>(x + uUvBorder) / fUDivisor;
            const float fV = static_cast<float>(y + uUvBorder) / fVDivisor;
            D3DXFloat32To16Array(&kVertex.u, &fU, 1);
            D3DXFloat32To16Array(&kVertex.v, &fV, 1);
            const float fZero = 0.0f;
            for (int i = 0; i < 4; ++i) {
                D3DXFloat32To16Array(&kVertex.zero[i], &fZero, 1);
            }
        }
    }
    FireGrafix::g_pRenderer->UnlockVB(hVB, 0, uWidth * uHeight);
    return hVB;
}
