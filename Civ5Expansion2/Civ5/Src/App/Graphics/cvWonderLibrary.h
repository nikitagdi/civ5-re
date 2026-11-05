/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <memory>
#include <string>
#include <vector>

class DecalVertex;
class FXmlTraverser;
class cvMeshDecal;
namespace FireGrafix {
enum FGX_SPIV_GENERIC : int;
}

// Wonder art comes from Assets\Buildings\Wonders\Civ5ArtDefines_Wonders.xml plus every "WonderDefines" content package file:
// WonderArtInfos/WonderArtInfo[Type]/WonderArtState[fScale, Granny = model file]; only the first WonderArtState is read.
// Construction and built states are not in the XML: they are node state bits of the Granny model, selected by cvWonderVis::SetMeshState.
/// size 0x38
class cvWonderLibrary {
public:
    class cvWonderLibraryEntry;
    // FGXGrannyCompositeContainerTemplate base at +0x0.
    /// size 0xcd0
    class cvWonderLibraryEntry {
    public:
        unsigned char unk_0[0xba];                                      ///< +0x0
        char m_szModelFile[0x80];                                       ///< +0xba
        char m_szTypeName[0x80];                                        ///< +0x13a
        unsigned char unk_1ba[0x2];                                     ///< +0x1ba
        // fScale from the XML when positive, else 1.0.
        float m_fScale;                                                 ///< +0x1bc
        unsigned char m_aModels[8][0x15c];                              ///< +0x1c0
        unsigned int m_uModelCount;                                     ///< +0xca0
        unsigned int field_ca4;                                         ///< +0xca4
        FFastVector<cvMeshDecal, true, 43u, 0u, BaseVector<cvMeshDecal, true>::FDefaultFastVectorAllocator>* m_pDecals;           ///< +0xca8
        FFastVector<DecalVertex, true, 43u, 0u, BaseVector<DecalVertex, true>::FDefaultFastVectorAllocator>* m_pDecalVertices;    ///< +0xcac
        FFastVector<unsigned short, true, 43u, 0u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator>* m_pDecalIndices; ///< +0xcb0
        // Surfaces per continent variant; continent n uses surfaces from n * m_uSurfaceCount.
        unsigned int m_uSurfaceCount;                                   ///< +0xcb4
        // Model XY bounds, padded by 3 on each side.
        float m_fBoundsMinX;                                            ///< +0xcb8
        float m_fBoundsMinY;                                            ///< +0xcbc
        float m_fBoundsMaxX;                                            ///< +0xcc0
        float m_fBoundsMaxY;                                            ///< +0xcc4
        unsigned char unk_cc8[0x8];                                     ///< +0xcc8
        /// 0x08861666
        cvWonderLibraryEntry();
        /// 0x0886170e
        ~cvWonderLibraryEntry();
        /// 0x08861318
        UNK_RET SetTypes(char const* a1);
        /// 0x08861474
        UNK_RET ReloadEntry(FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a1);
        /// 0x08861488
        UNK_RET LoadEntry(FXmlTraverser const& a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a2);
        /// 0x08861736
        UNK_RET Release();
        /// 0x08861792
        UNK_RET BindAsset(char const* a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a2);
        /// 0x08861b0e
        UNK_RET ReloadAssets(FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a1);
        /// 0x08861df6
        UNK_RET BuildContinentSurfaces(std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 4u, 29u, 128u> >& a1, unsigned int a2, unsigned int a3, bool a4, unsigned int a5);
    };
    cvWonderLibraryEntry* m_pEntries;                                   ///< +0x0
    unsigned int m_uEntryCount;                                         ///< +0x4
    FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator> m_vShaderPackageInstances; ///< +0x8
    FFastVector<cvMeshDecal, true, 43u, 0u, BaseVector<cvMeshDecal, true>::FDefaultFastVectorAllocator> m_vDecals;            ///< +0x14
    FFastVector<DecalVertex, true, 43u, 0u, BaseVector<DecalVertex, true>::FDefaultFastVectorAllocator> m_vDecalVertices;     ///< +0x20
    FFastVector<unsigned short, true, 43u, 0u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator> m_vDecalIndices; ///< +0x2c
    /// 0x08860cd0
    cvWonderLibrary();
    /// 0x08860d36
    ~cvWonderLibrary();
    /// 0x08860da6
    UNK_RET Release();
    /// 0x08860de8
    UNK_RET Init(std::vector<std::string, std::allocator<std::string> > const& a1);
    /// 0x08861174
    UNK_RET AllocateLibraryEntries(unsigned int a1);
    /// 0x0886127e
    UNK_RET InitShaderPackageInstances();
    /// 0x0886135c
    UNK_RET LoadWonderArtStateFromXml(FXmlTraverser& a1, unsigned int a2);
    /// 0x0886139a
    UNK_RET DeallocateLibraryEntries();
    /// 0x08861416
    UNK_RET ReleaseShaderPackageInstances();
    /// 0x0886143a
    UNK_RET ReloadWonder(unsigned int a1);
};
