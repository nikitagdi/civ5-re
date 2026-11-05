/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "Civ5/Src/App/Graphics/cvCoastMeshEngine.h"

class cvTerrainSampler;

// Shared by every job of one BlendAllCoastMeshes launch; copied into each job.
/// size 0x1c
class CoastMeshBlendParams {
public:
    // CoastMesh::field_18 of the batch's mesh, indexed by the triangle index within the batch.
    FGXVector3 const* m_pEdgeWeights;    ///< +0x0
    unsigned short field_4;              ///< +0x4
    char unk_6[2];                       ///< +0x6
    cvTerrainSampler* m_pSampler;        ///< +0x8
    FGXInt2 m_kMapSize;                  ///< +0xc
    // TerrainData indices; a texel left low by the blend and holding m_aucFromTerrain[i] becomes m_aucToTerrain[i].
    unsigned char m_aucFromTerrain[4];   ///< +0x14
    unsigned char m_aucToTerrain[4];     ///< +0x18
};

// One job per CoastMeshBatch; BlendAllCoastMeshes runs the even batches of every mesh, waits, then the odd ones.
/// size 0x4c
/// vtable 0x0a2fdc64
class CoastMeshBlendJob : public FJob {
public:
    /// inline
    CoastMeshBlendJob() {
        field_4 = 0;
        m_kMapSize.x = 0;
        m_kMapSize.y = 0;
    }

    /// 0x088eaed4
    virtual void Execute(unsigned int a1);

    FGXInt2 m_kMapSize;            ///< +0x10
    CoastMeshBatch m_kBatch;       ///< +0x18
    CoastMeshBlendParams m_kParams; ///< +0x2c
    // Nonzero in one random job of the even pass: Execute leaks 0x80 bytes of the coast temp heap.
    int m_iHeapTrample;            ///< +0x48
};

// Rasterises each batch triangle over the height map texels it covers and lerps their height toward the mesh height.
/// 0x088eaf32
void Fun_088eaf32(FGXInt2 const* pMapSize, CoastMeshBatch const* pBatch, CoastMeshBlendParams const* pParams);
