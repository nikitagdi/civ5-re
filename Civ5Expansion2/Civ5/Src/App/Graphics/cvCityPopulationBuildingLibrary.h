/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include "FirePlace/FireWorks/FString.h"
#include <vector>

class DecalVertex;
class FXmlTraverser;
class cvMeshDecal;
enum eCityPopulationBuildingHeights : int;
namespace FireGrafix {
class FGXModelTemplate;
enum FGX_SPIV_GENERIC : int;
}

// Building sets are loaded from Assets\Buildings\City\Civ5ArtDefines_CityBuildings.xml plus every "CityDefines" content package file:
// BuildingArtInfos/Culture[name, lighting]/Era[name]/PopulationBuildings[Granny = model file], four Era children per Culture.
// A set is addressed as cultureOrdinal * m_uErasPerCulture + era; the key culture-name + era-name goes into g_cvCityBuildingTypeMap.
/// size 0x58
class cvCityPopulationBuildingLibrary {
public:
    class cvCityBuildingSet;
    // One loaded city building model (FGXModelTemplate base at +0x0): its nodes are the candidate buildings, picked by height class.
    /// size 0x2260
    class cvCityBuildingSet {
    public:
        // One FGXModelNode of the set; only the field SelectBuilding filters on is named.
        /// size 0x40
        struct cvBuildingNode {
            unsigned char unk_0[0x3];       ///< +0x0
            unsigned char m_ucHeight;       ///< +0x3
            unsigned char unk_4[0x3c];      ///< +0x4
        };
        unsigned int* m_pSurfacesBegin;                                       ///< +0x0
        unsigned int* m_pSurfacesEnd;                                         ///< +0x4
        unsigned char unk_8[0x10];                                            ///< +0x8
        cvBuildingNode* m_pNodesBegin;                                        ///< +0x18
        cvBuildingNode* m_pNodesEnd;                                          ///< +0x1c
        unsigned char unk_20[0x30];                                           ///< +0x20
        // Surface count per continent variant; continent n uses surfaces from n * m_uSurfaceCount.
        unsigned int m_uSurfaceCount;                                         ///< +0x50
        unsigned char unk_54[0x4c];                                           ///< +0x54
        unsigned char m_aModels[0x18][0x15c];                                 ///< +0xa0
        unsigned int m_uModelCount;                                           ///< +0x2140
        unsigned int field_2144;                                              ///< +0x2144
        FFastVector<cvMeshDecal, true, 43u, 0u, BaseVector<cvMeshDecal, true>::FDefaultFastVectorAllocator>* m_pDecals;           ///< +0x2148
        FFastVector<DecalVertex, true, 43u, 0u, BaseVector<DecalVertex, true>::FDefaultFastVectorAllocator>* m_pDecalVertices;    ///< +0x214c
        FFastVector<unsigned short, true, 43u, 0u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator>* m_pDecalIndices; ///< +0x2150
        char m_szModelFile[0x104];                                            ///< +0x2154
        unsigned int m_uLightingCulture;                                      ///< +0x2258
        unsigned int m_uEra;                                                  ///< +0x225c
        /// 0x088ad0f6
        cvCityBuildingSet();
        /// 0x088ad1ea
        ~cvCityBuildingSet();
        /// 0x088acaf2
        UNK_RET Init(char const* a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a2, unsigned int a3, unsigned int a4);
        /// 0x088aceb8
        UNK_RET Reload(FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a1);
        /// 0x088ad226
        UNK_RET Release();
        /// 0x088ad290
        UNK_RET BuildContinentSurfaces(std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 4u, 29u, 128u> >& a1, unsigned int a2, unsigned int a3, bool a4, unsigned int a5);
    };
    FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator> m_vShaderPackageInstances; ///< +0x0
    // Always 4: the Era children read per Culture.
    unsigned int m_uErasPerCulture;                                           ///< +0xc
    cvCityBuildingSet* m_pBuildingSets;                                       ///< +0x10
    unsigned int m_uBuildingSetCount;                                         ///< +0x14
    // Indexed by culture type (America, Asia, Africa, Europe, Mediterranean, Polynesian, Ruins); value is the Culture element ordinal.
    unsigned int m_auCultureOrdinal[7];                                       ///< +0x18
    FFastVector<cvMeshDecal, true, 43u, 0u, BaseVector<cvMeshDecal, true>::FDefaultFastVectorAllocator> m_vDecals;            ///< +0x34
    FFastVector<DecalVertex, true, 43u, 0u, BaseVector<DecalVertex, true>::FDefaultFastVectorAllocator> m_vDecalVertices;     ///< +0x40
    FFastVector<unsigned short, true, 43u, 0u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator> m_vDecalIndices; ///< +0x4c
    /// 0x088abc4c
    cvCityPopulationBuildingLibrary();
    /// 0x088abcba
    ~cvCityPopulationBuildingLibrary();
    /// 0x088abd28
    void Release();
    /// 0x088abd6e
    void Init();
    /// 0x088ac874
    UNK_RET InitShaderPackageInstances();
    /// 0x088ac902
    void AllocateBuildingSets(unsigned int a1);
    /// 0x088aca04
    bool LoadPopBldgDataFromXml(FXmlTraverser& a1, FStringA& a2, FStringA& a3);
    /// 0x088acd50
    UNK_RET ReleaseShaderPackageInstances();
    /// 0x088acd56
    void DeleteBuildingSets();
    /// 0x088acdc0
    bool SelectBuilding(unsigned int a1, eCityPopulationBuildingHeights a2, FireGrafix::FGXModelTemplate const*& a3, unsigned int& a4, unsigned int& a5) const;
    /// 0x088ace86
    void ReloadAsset(unsigned int a1);
};
