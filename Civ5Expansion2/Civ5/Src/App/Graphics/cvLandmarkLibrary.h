/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <vector>

class CSurfaceTexturePackage;
class FXmlTraverser;
template <unsigned int T0> class cvInfoIDBitSet;
namespace FireGrafix {
enum FGX_SPIV_GENERIC : int;
}
namespace cvLandmark_Properties {
enum eLandmarkEra : int;
enum eLandmarkState : int;
}

/// size ?
class cvLandmarkLibrary {
public:
    enum eLandmarkLayoutHandler : int {
        LANDMARK_LAYOUT_NONE = 0,
        LANDMARK_LAYOUT_FARM = 1,
        LANDMARK_LAYOUT_SPECIAL = 2,
        LANDMARK_LAYOUT_PASTURE = 3,
        LANDMARK_LAYOUT_SPRING = 4,
        LANDMARK_LAYOUT_ANIMATED = 5,
        LANDMARK_LAYOUT_ANIMATED_ROTATED = 6,
        LANDMARK_LAYOUT_RANDOM = 7
    };
    class cvLandmarkLibraryEntry;
    class cvLandmarkLibraryReference;
    // Array stride 0x17f0 in cvFarmSimSystem::StartupGame and FillLandmarkSimInfo.
    /// size 0x17f0
    class cvLandmarkLibraryEntry {
    public:
        char unk_0[0x15c0];         ///< +0x0
        char m_szAssetPath[0x80];   ///< +0x15c0
        char unk_1640[0x100];       ///< +0x1640
        // Number of model pieces; cvFarmSimSystem::FillLandmarkSimInfo fills one vis element per piece.
        unsigned int m_uiNumPieces; ///< +0x1740
        char unk_1744[0x10];        ///< +0x1744
        int m_iType;                ///< +0x1754
        int m_iSubType;             ///< +0x1758
        int m_iEra;                 ///< +0x175c
        int m_iState;               ///< +0x1760
        unsigned int m_uiTechHash;  ///< +0x1764
        char unk_1768[0x88];        ///< +0x1768
        /// 0x088760d0
        cvLandmarkLibraryEntry();
        /// 0x088761c4
        ~cvLandmarkLibraryEntry();
        /// 0x08874fe6
        UNK_RET LoadEntry(FXmlTraverser const& a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator> const& a2, CSurfaceTexturePackage& a3);
        /// 0x08875940
        UNK_RET LoadEntry(Database::Results& a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator> const& a2, CSurfaceTexturePackage& a3);
        /// 0x08876210
        UNK_RET Release();
        /// 0x08876288
        UNK_RET StringToLandmarkEra(char const* a1) const;
        /// 0x08876312
        UNK_RET StringToLandmarkState(char const* a1) const;
        /// 0x08876384
        UNK_RET BindAsset(char const* a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator> const& a2, CSurfaceTexturePackage& a3);
        /// 0x08876a66
        cvLandmarkLibrary::eLandmarkLayoutHandler StringToLandmarkLayoutHandler(char const* a1) const;
        /// 0x08876b4a
        UNK_RET BuildContinentSurfaces(std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 4u, 29u, 128u> >& a1, unsigned int a2, unsigned int a3, bool a4, unsigned int a5);
    };
    /// size ?
    class cvLandmarkLibraryReference {
    public:
        cvLandmarkLibraryEntry* m_pEntry; ///< +0x0
        unsigned int m_uiEntryIndex;      ///< +0x4
        /// 0x08875e1c
        int IsValidForTech(cvInfoIDBitSet<256u> const& a1) const;
    };
    cvLandmarkLibraryEntry* m_pEntries;   ///< +0x0
    char unk_4[0x7c];                     ///< +0x4
    unsigned int m_uiNumEntries;          ///< +0x80
    /// 0x08874618
    cvLandmarkLibrary();
    /// 0x088746d8
    ~cvLandmarkLibrary();
    /// 0x08874750
    UNK_RET Release();
    /// 0x08874792
    UNK_RET Init(char const* a1);
    /// 0x08874d2e
    UNK_RET AllocateLibraryEntries(unsigned int a1);
    /// 0x08874e32
    UNK_RET InitTexturePackage();
    /// 0x08874f3c
    UNK_RET InitShaderPackageInstances();
    /// 0x08875504
    UNK_RET Init(Database::Connection& a1);
    /// 0x08875ba8
    UNK_RET DeallocateLibraryEntries();
    /// 0x08875c12
    UNK_RET ReleaseShaderPackageInstances();
    /// 0x08875c36
    UNK_RET PopulateLandmarkEntryReferences(FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator>& a1, cvLandmarkLibrary::eLandmarkLayoutHandler a2) const;
    /// 0x08875caa
    int GetLandmarkEntryIndex(int iType, int iSubType, cvLandmark_Properties::eLandmarkEra a3, cvLandmark_Properties::eLandmarkState a4, cvInfoIDBitSet<256u> const& a5, FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> const& a6) const;
    /// 0x08875ede
    unsigned int GetMatchingLandmarkEntryIndices(int a1, int a2, cvLandmark_Properties::eLandmarkEra a3, cvLandmark_Properties::eLandmarkState a4, cvInfoIDBitSet<256u> const& a5, FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> const& a6, unsigned int* a7) const;
    /// 0x08876050
    int GetRandomLandmarkEntryIndex(int a1, int a2, cvLandmark_Properties::eLandmarkEra a3, cvLandmark_Properties::eLandmarkState a4, cvInfoIDBitSet<256u> const& a5, FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> const& a6) const;
};
