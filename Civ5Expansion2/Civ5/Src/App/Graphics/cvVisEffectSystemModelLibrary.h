/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include <memory>
#include <string>
#include <vector>

class FXmlTraverser;
namespace FireGrafix {
enum FGX_SPIV_GENERIC : int;
}

/// size ?
class cvVisEffectSystemModelLibrary {
public:
    class LibraryEntry;
    /// size ?
    class LibraryEntry {
    public:
        /// 0x088d2652
        LibraryEntry();
        /// 0x088d2692
        ~LibraryEntry();
        /// 0x088d2060
        UNK_RET LoadEntry(FXmlTraverser const& a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a2);
        /// 0x088d24ee
        UNK_RET ReloadEntry(FXmlTraverser const& a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a2);
        /// 0x088d26a6
        UNK_RET BindAsset(FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a1);
    };
    /// 0x088d18ec
    cvVisEffectSystemModelLibrary();
    /// 0x088d1936
    ~cvVisEffectSystemModelLibrary();
    /// 0x088d1972
    UNK_RET Release();
    /// 0x088d199a
    UNK_RET Init(std::vector<std::string, std::allocator<std::string> > const& a1);
    /// 0x088d1e80
    UNK_RET InitShaderPackageInstances();
    /// 0x088d1f74
    UNK_RET AllocateLibraryEntries(unsigned int a1);
    /// 0x088d2140
    UNK_RET Reload(std::vector<std::string, std::allocator<std::string> > const& a1);
    /// 0x088d2516
    UNK_RET DeallocateLibraryEntries();
    /// 0x088d2580
    UNK_RET ReleaseShaderPackageInstances();
    /// 0x088d25a4
    UNK_RET GetLibraryEntry(unsigned int a1);
    /// 0x088d25f6
    UNK_RET GetModelHandleByModelFilename(char const* a1) const;
};
