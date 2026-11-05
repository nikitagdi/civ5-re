/// origin guess
#pragma once
#include <Windows.h>

namespace FireGrafix {
class FGXModelTemplate;
}

// One static building of a city: a node of the city's template plus its transform.
/// size 0x64
class cvCityVisBuilding {
public:
    unsigned char unk_0[0x14];          ///< +0x0
    // Entry of the system's random table, taken when the building was added.
    float m_fRandom;                    ///< +0x14
    // Height class 0 to 3, the key SelectBuilding filters nodes on.
    unsigned char m_ucHeight;           ///< +0x18
    unsigned char unk_19[0x3];          ///< +0x19
    D3DXMATRIX m_mWorld;                ///< +0x1c
    unsigned int field_5c;              ///< +0x5c
    // Node of the building set the mesh decals are taken from.
    unsigned int m_uNodeIndex;          ///< +0x60
};

// Wall polyline, up to 6 points.
/// size 0x4c
class cvCityWallSegment {
public:
    float m_aafPoints[6][3];            ///< +0x0
    unsigned char m_ucPointCount;       ///< +0x48
    unsigned char unk_49[0x3];          ///< +0x49
};

// One city on the map: composition inputs (era, culture, continent, wall style) and the static building list.
/// size 0x4f0
class cvCityVis {
public:
    cvCityWallSegment m_akWallSegments[10];   ///< +0x0
    unsigned char unk_2f8[0xc];               ///< +0x2f8
    float m_aafTowerPositions[20][3];         ///< +0x304
    float m_afTowerValues[20];                ///< +0x3f4
    float m_aafGatePositions[6][3];           ///< +0x444
    float m_afGateValues[6];                  ///< +0x48c
    float m_fWallScale;                       ///< +0x4a4
    unsigned char m_ucWallSegmentCount;       ///< +0x4a8
    unsigned char m_ucWallTowerCount;         ///< +0x4a9
    unsigned char m_ucWallGateCount;          ///< +0x4aa
    // Index into the DynamicGeometryLibrary wall styles; 3 when the id is not found.
    unsigned char m_ucWallStyle;              ///< +0x4ab
    unsigned int m_uWallSurface;              ///< +0x4ac
    unsigned int m_uWallVertexBuffer;         ///< +0x4b0
    unsigned int m_uWallIndexBuffer;          ///< +0x4b4
    // Bit 31 set marks a free pool slot; the low 31 bits are the EndUpdate value.
    unsigned int m_uSlotFlags;                ///< +0x4b8
    FireGrafix::FGXModelTemplate* m_pStaticTemplate; ///< +0x4bc
    cvCityVisBuilding* m_pBuildings;          ///< +0x4c0
    unsigned int m_uBuildingCount;            ///< +0x4c4
    unsigned int m_uBuildingCapacity;         ///< +0x4c8
    unsigned int m_uEra;                      ///< +0x4cc
    unsigned int m_uCulture;                  ///< +0x4d0
    unsigned int m_uContinent;                ///< +0x4d4
    // Handle of the city's wonder list in cvCityVisSystem; -1 when it has none.
    unsigned int m_uWonderGroup;              ///< +0x4d8
    unsigned char m_ucFOWState;               ///< +0x4dc
    unsigned char unk_4dd[0x3];               ///< +0x4dd
    float m_fPosX;                            ///< +0x4e0
    float m_fPosY;                            ///< +0x4e4
    float field_4e8;                          ///< +0x4e8
    float field_4ec;                          ///< +0x4ec
    /// 0x08a2b214
    cvCityVis();
    /// 0x08a2b2ce
    ~cvCityVis();
    /// 0x08a2b2f4
    bool Load();
    /// 0x08a2b3b8
    UNK_RET Destroy();
    /// 0x08a2b450
    unsigned int AddVisBuilding(cvCityVisBuilding const& a1);
    /// 0x08a2b47e
    UNK_RET CompactStaticBuildingNodesAndTransforms();
    /// 0x08a2b8ca
    UNK_RET SetFOWState(unsigned char a1, bool a2);
    /// 0x0c08b3a4
    static UNK_DATA m_TempSurfaceVector[0xc];
    /// 0x0c08b3b0
    static UNK_DATA m_TempVisBuildings[0xc];
};
