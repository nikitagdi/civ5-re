/// origin guess
#pragma once
#include "FirePlace/FireWorks/FReferenceObject.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <granny.h>
#include <utility>
#include <vector>

class FAnimationTextKeyProcessor;
class FCSVFile;
class FGrannyFileRefObject;
class FStringArray;
template <class T0> class FTPointer;

/// size ?
/// vtable 0x0a301970
class FGrannyAsset : public FReferenceObject {
public:
    enum eGrannyAsset : int;
    /// 0x08cc36b0
    FGrannyAsset();
    /// 0x08cc38bc
    /// deleting 0x08cc3894
    virtual ~FGrannyAsset();
    /// 0x08cc8996
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08cc4b3e
    virtual UNK_RET AddFileAsset(FTPointer<FGrannyFileRefObject> a1, char const* a2, unsigned int a3);
    /// 0x08cc4e84
    virtual UNK_RET PreprocessFileForAsset(FTPointer<FGrannyFileRefObject> a1) const;
    /// 0x08cc4990
    virtual UNK_RET RemoveFileAsset(int a1);
    /// 0x08cc577c
    virtual UNK_RET AddModelAsset(granny_model* a1, unsigned int a2);
    /// 0x08cc5a9c
    virtual UNK_RET AddAnimationAsset(granny_animation* a1, unsigned int a2, char const* a3);
    /// 0x08cc5ba0
    virtual UNK_RET AddAnimationAsset(unsigned short a1, granny_animation* a2, unsigned int a3, char const* a4);
    /// 0x08cc5cd4
    virtual UNK_RET AddTrackMaskAsset(granny_track_mask* a1, unsigned int a2, char const* a3, granny_model const* a4);
    /// 0x08cc67c8
    virtual UNK_RET AddDAG2Asset(granny_dag2* a1, unsigned int a2, char const* a3);
    /// 0x08cc5dfe
    virtual UNK_RET AddMiscAsset(void* a1, unsigned int a2, unsigned int* a3);
    /// 0x08cc699e
    virtual UNK_RET AddMiscAssetEntry(char const* a1, void* a2, unsigned int a3);
    /// 0x08cc6852
    virtual UNK_RET FindMiscAssetEntry(char const* a1) const;
    /// 0x08cc5120
    virtual UNK_RET RemoveGrannyModelData();
    /// 0x08cc538c
    virtual UNK_RET RestoreGrannyModelData(bool a1);
    /// 0x08cc6b9a
    virtual UNK_RET ClearAssets(unsigned int a1);
    /// 0x08cc6f84
    virtual UNK_RET ClearMiscAsset(std::pair<void*, unsigned int> const* a1);
    /// 0x08cc6dd4
    virtual UNK_RET ClearAnimGraph(char const* a1);
    /// 0x08cc716a
    virtual UNK_RET GetAssetCount(FGrannyAsset::eGrannyAsset a1) const;
    /// 0x08cc7eb8
    virtual UNK_RET GetUsageMask(unsigned int a1) const;
    /// 0x08cc7116
    virtual UNK_RET CreateInstance();
    /// 0x08cc577a
    virtual UNK_RET RemoveModelAsset(granny_model const* a1);
    /// 0x08cc5850
    virtual UNK_RET RemoveAnimationAsset(granny_animation const* a1, char const* a2);
    /// 0x08cc4f10
    virtual UNK_RET AddModelsFromFile(granny_file_info* a1, FTPointer<FGrannyFileRefObject> a2, char const* a3, unsigned int a4);
    /// 0x08cc4f6c
    virtual UNK_RET AddAnimationsFromFile(granny_file_info* a1, FTPointer<FGrannyFileRefObject> a2, char const* a3, unsigned int a4);
    /// 0x08cc4fd6
    virtual UNK_RET AddTrackMasksFromModel(granny_model const* a1);
    /// 0x08cc89aa
    virtual UNK_RET vf_27(UNK_PARAMS);
    /// 0x08cc3816
    UNK_RET SharedMemoryPush(unsigned int a1);
    /// 0x08cc3a52
    UNK_RET ReleaseModelAnimationSets();
    /// 0x08cc3b02
    UNK_RET CacheModelAssets(bool a1);
    /// 0x08cc3b32
    UNK_RET AppendAnimationExtraTextTrack(granny_animation const* a1, FCSVFile* a2, char const* a3);
    /// 0x08cc4522
    UNK_RET CreateNewTextTrackFromEntries(std::vector<granny_text_track_entry, FSTL_Tagged_Allocator<granny_text_track_entry, 1u, 27u, 4u> > const& a1);
    /// 0x08cc45a6
    UNK_RET AppendAnimationExtraTextTrack(granny_animation const* a1, granny_text_track* a2);
    /// 0x08cc4706
    UNK_RET NormalizeRotationsForFile(granny_file_info* a1, int a2) const;
    /// 0x08cc486c
    UNK_RET NormalizeRotationsForAnimation(granny_animation* a1, granny_file_info* a2) const;
    /// 0x08cc4920
    UNK_RET HasFileAsset(char const* a1) const;
    /// 0x08cc4af8
    UNK_RET GetFileAssetName(unsigned int a1) const;
    /// 0x08cc4cf0
    UNK_RET CollectAssetsFromDAG2Info(granny_dag2_info* a1, FTPointer<FGrannyFileRefObject> a2, char const* a3, unsigned int a4);
    /// 0x08cc4d5a
    UNK_RET CollectAssetsFromFileInfo(granny_file_info* a1, FTPointer<FGrannyFileRefObject> a2, char const* a3, unsigned int a4);
    /// 0x08cc5014
    UNK_RET GrannyBoneIsCamera(granny_bone const* a1);
    /// 0x08cc506a
    UNK_RET GrannyBoneIsLight(granny_bone const* a1);
    /// 0x08cc5310
    UNK_RET RemoveGrannyModelData(granny_file* a1);
    /// 0x08cc5d64
    UNK_RET HasMiscAsset(unsigned int a1) const;
    /// 0x08cc5d8c
    UNK_RET AddModelMiscAssetEntry(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08cc5e7a
    UNK_RET CreateOffsetConnectionTables();
    /// 0x08cc63b6
    UNK_RET FindMiscAssetByDataID(unsigned int a1) const;
    /// 0x08cc6408
    UNK_RET GetAnimationGraphByName(char const* a1) const;
    /// 0x08cc654a
    UNK_RET GetAnimationFromEventCode(unsigned short a1) const;
    /// 0x08cc659e
    UNK_RET GetAssociatedScriptIDs(FStringArray& a1) const;
    /// 0x08cc6dc4
    UNK_RET GetMiscAsset(unsigned int a1) const;
    /// 0x08cc71e6
    UNK_RET GetFileAsset(unsigned int a1) const;
    /// 0x08cc720c
    UNK_RET GetFileAsset(unsigned int a1);
    /// 0x08cc7232
    UNK_RET GetFileAssetRefObjectPtr(unsigned int a1);
    /// 0x08cc7266
    UNK_RET GetModelAsset(unsigned int a1) const;
    /// 0x08cc7286
    UNK_RET GetModelAssetIndexByName(char const* a1) const;
    /// 0x08cc72e8
    UNK_RET GetGrannyBone(char const* a1, int* a2, int* a3, bool a4) const;
    /// 0x08cc739c
    UNK_RET GetFGrannyAnimationAsset(unsigned int a1);
    /// 0x08cc73c8
    UNK_RET GetAnimationAsset(unsigned int a1) const;
    /// 0x08cc73f4
    UNK_RET GetAnimationAsset(unsigned int a1);
    /// 0x08cc7420
    UNK_RET GetAnimationName(unsigned int a1) const;
    /// 0x08cc7450
    UNK_RET GetAnimationEventCodeByName(char const* a1) const;
    /// 0x08cc74cc
    UNK_RET GetAnimationGraph(unsigned int a1);
    /// 0x08cc74fc
    UNK_RET GetAnimationGraphList() const;
    /// 0x08cc7506
    UNK_RET GetTrackMaskList() const;
    /// 0x08cc750e
    UNK_RET GetTrackMaskAsset(unsigned int a1) const;
    /// 0x08cc7538
    UNK_RET FindModelMiscAssetByDataID(unsigned int a1, unsigned int a2, unsigned int a3) const;
    /// 0x08cc75ca
    UNK_RET GetAnimationEventCode(granny_control const* a1) const;
    /// 0x08cc7606
    UNK_RET GetAnimationEventCode(granny_animation const* a1) const;
    /// 0x08cc764c
    UNK_RET GetAnimationNameFromEventCode(unsigned short a1) const;
    /// 0x08cc76a6
    UNK_RET SetAnimationEventCode(granny_animation* a1, unsigned short a2, char const* a3, bool a4);
    /// 0x08cc7a24
    UNK_RET CreateDefaultBindingCallback(granny_dag2_instantiate_callbacks* a1) const;
    /// 0x08cc7a54
    UNK_RET SetAnimationLayerName(granny_animation const* a1, char const* a2);
    /// 0x08cc7a9c
    UNK_RET SetFileScaleFactor(float a1);
    /// 0x08cc7ab6
    UNK_RET SetFileOptions(unsigned int a1);
    /// 0x08cc7acc
    UNK_RET GetAnimationLayerName(granny_animation const* a1);
    /// 0x08cc7b08
    UNK_RET GetAnimationLayerTrackMask(granny_animation const* a1);
    /// 0x08cc7b0c
    UNK_RET GetTextKeyProcessor() const;
    /// 0x08cc7b2e
    UNK_RET GetDefaultTextKeyProcessor();
    /// 0x08cc7b44
    UNK_RET SetTextKeyProcessor(FAnimationTextKeyProcessor* a1);
    /// 0x08cc7b54
    UNK_RET SetDefaultTextKeyProcessor(FAnimationTextKeyProcessor* a1);
    /// 0x08cc7b6e
    UNK_RET GetAnimationHasAccumulation(granny_animation const* a1);
    /// 0x08cc7bcc
    UNK_RET GetAnimationHasAccumulation(unsigned int a1) const;
    /// 0x08cc7c14
    UNK_RET GetAnyAnimationsHaveAccumulation() const;
    /// 0x08cc7c98
    UNK_RET GetTextKeysForAnimation(unsigned short a1, FStringArray& a2) const;
    /// 0x08cc7ed8
    UNK_RET IsInitializedModelAnimationSets();
    /// 0x08cc7ee8
    UNK_RET InitializeModelAnimationSets();
    /// 0x08cc81a4
    UNK_RET GetModelAnimationSet(unsigned int a1);
    /// 0x08cc81ba
    UNK_RET GetDefaultFileOptions();
    /// 0x08cc81d0
    UNK_RET SetDefaultFileOptions(unsigned int a1);
    /// 0x0d276a50
    static UNK_DATA m_sfLODPixelError[0x4];
    /// 0x0d276a4c
    static UNK_DATA ms_bCacheModelAssets[0x1];
    /// 0x0d276a48
    static UNK_DATA ms_kDefaultTextKeyProcessor[0x4];
    /// 0x0d276a40
    static UNK_DATA ms_kRTTI[0x8];
    /// 0x0d276a54
    static UNK_DATA ms_uiDefaultFileOptions[0x4];
};

/// 0x08cc3b1b
UNK_RET TextTrackEntryCompare(granny_text_track_entry const& a1, granny_text_track_entry const& a2);
/// 0x08cc50c0
UNK_RET EncodeSectionPointer(granny_file* a1, void* a2);
/// 0x08cc50fe
UNK_RET DecodeSectionPointer(granny_file* a1, void* a2);
/// 0x08cc5a47
UNK_RET AddInfLODErrorToAnimation(granny_animation* a1);
/// 0x08cc75b7
UNK_RET AnimationsAreEqual(std::pair<int, granny_animation const*> const& a1, granny_animation const* a2);
/// 0x08cc7969
UNK_RET FGrannyAssetTrackMaskBindingCallback(char const* a1, granny_model const* a2, granny_model const* a3, void* a4);
/// 0x08cc79c9
UNK_RET FGrannyAssetSourceBindingCallback(int a1, void* a2);
