/// origin guess
#pragma once
#include "FirePlace/FireEngine2/DefaultBuilderContext.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <granny.h>
#include <vector>

class FGrannyAsset;
namespace FireGrafix {
class FGXModelTemplate;
}

/// size ?
class CompositeContainerBuilderContext : public DefaultBuilderContext {
public:
    char unk_0[0x24];   ///< +0x0
    /// 0x08cd69ba
    CompositeContainerBuilderContext();
    /// 0x08cd6a36
    ~CompositeContainerBuilderContext();
    /// 0x08cd6b1e
    UNK_RET AddSourceGrannyAsset(FGrannyAsset* a1, FireGrafix::FGXModelTemplate const* a2);
    /// 0x08cd6c00
    UNK_RET AddGrannyModel(granny_model* a1, unsigned int a2);
    /// 0x08cd6caa
    UNK_RET GrannyAssetExistsForModel(granny_model const* a1);
    /// 0x08cd6d10
    UNK_RET GetFGXSurfaceHandle(granny_model const* a1, unsigned int a2) const;
    /// 0x08cd6dbe
    UNK_RET GetAssetIndexAndModelIndexFromModel(granny_model const* a1, unsigned int& a2, unsigned int& a3) const;
    /// 0x08cd6e36
    UNK_RET GetGrannyModels() const;
    /// 0x08cd6e4c
    UNK_RET CopySourceGrannyAssetsTo(std::vector<FGrannyAsset*, FSTL_Tagged_Allocator<FGrannyAsset*, 4u, 29u, 4u> >& a1) const;
};
