/// origin guess
#include "Civ5/Src/App/Graphics/DecalContainerBuilderContext.h"
#include "Civ5/Src/App/Graphics/TerrainDecalLayer.h"
#include "FirePlace/FireEngine2/FGrFGXMaterialInfo.h"
#include "FirePlace/FireEngine2/FTextureResource.h"

namespace {

const int kGrannyReal32Member = 10;
const unsigned int kTextureLoadFlags = 0x20000;
const int kIndexBytes = 2;

}

/// 0x088be424
UNK_RET DecalContainerBuilderContext::InitMeshDecal(granny_mesh const* a1, granny_material const* a2, unsigned char a3) {
    cvMeshDecal& kDecal = m_pDecals->begin()[m_pModels[m_uModelIndex].m_usFirstDecal + m_uDecalCursor];

    granny_data_type_definition aVertexType[3] = {};
    aVertexType[0].Type = kGrannyReal32Member;
    aVertexType[0].Name = "Position";
    aVertexType[0].ArrayWidth = 2;
    aVertexType[1].Type = kGrannyReal32Member;
    aVertexType[1].Name = "TextureCoordinates0";
    aVertexType[1].ArrayWidth = 2;

    const int iIndexCount = GrannyGetMeshIndexCount(a1);
    if (iIndexCount == kDecal.m_usIndexCount) {
        const int iSourceBytes = GrannyGetMeshBytesPerIndex(a1);
        void* pSource = GrannyGetMeshIndices(a1);
        GrannyConvertIndices(iIndexCount, iSourceBytes, pSource, kIndexBytes, m_pDecalIndices->begin() + kDecal.m_uFirstIndex);
    }

    if (GrannyGetMeshVertexCount(a1) == kDecal.m_usVertexCount) {
        GrannyCopyMeshVertices(a1, aVertexType, m_pDecalVertices->begin() + kDecal.m_uFirstVertex);
        const float fZ = static_cast<const float*>(GrannyGetMeshVertices(a1))[2];
        float fPriority = fZ;
        if (fZ > 1.0f) {
            fPriority = 1.0f;
        } else if (!(fZ > 0.0f)) {
            fPriority = 0.0f;
        }
        kDecal.m_fPriority = fPriority;
    }

    if (GrannyGetMeshTriangleGroups(a1) != nullptr && GrannyGetMeshTriangleGroupCount(a1) == 1 && a2 != nullptr && IsDecalMaterial(a2) != 0) {
        const BuilderMaterialSlot* pSlot = FindMaterialInfo(a2);
        const FGrFGXMaterialInfo* pInfo = reinterpret_cast<const FGrFGXMaterialInfo*>(pSlot->m_info);
        if (pInfo->HasParameters() && m_pModels != nullptr && m_pModelCount != nullptr && m_uModelIndex < *m_pModelCount &&
            m_uDecalCursor < m_pModels[m_uModelIndex].m_usDecalCount) {
            kDecal.m_hAlbedo = FTextureResourceMgr::LoadTextureHandle(pInfo->GetTextureFilename(0), kTextureLoadFlags, nullptr);
            unsigned int hLast;
            if (kDecal.m_ucKind == 2) {
                kDecal.m_hHeightmap = FTextureResourceMgr::LoadTextureHandle("blank_texture.dds", kTextureLoadFlags, nullptr);
                hLast = kDecal.m_hHeightmap;
            } else {
                // granny_material::MapCount
                const int iMapCount = reinterpret_cast<const int*>(a2)[1];
                const char* szSpec = iMapCount < 2 ? "blank_texture_spec_decal.dds" : pInfo->GetTextureFilename(1);
                kDecal.m_hHeightmap = FTextureResourceMgr::LoadTextureHandle(szSpec, kTextureLoadFlags, nullptr);
                hLast = FTextureResourceMgr::LoadTextureHandle("blank_texture.dds", kTextureLoadFlags, nullptr);
            }
            kDecal.m_hSpec = hLast;
        }
    }

    kDecal.m_ucMask = a3;
    ++m_uDecalCursor;
}
