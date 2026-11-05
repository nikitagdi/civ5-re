/// origin guess
#pragma once
#include "Civ5/Src/App/Engine/FGrannyStateMachineInstance.h"
#include "FirePlace/FireWorks/FContext.h"
#include "FirePlace/FireWorks/FIFiniteStateMachineInstanceProvider.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <granny.h>
#include <vector>

class FAnimationControlEntry;
class FClassId;
class FFSMConditionECComplete;
class FGrannyAsset;
class FGrannyModelInstanceData;
class FGrannyTimedTriggerInstancer;
class FIFSMCondition;

/// size ?
/// vtable 0x0a301a20
class FGrannyAssetInstance {
public:
    class FGrannyStateMachineVariableContext;
    /// size ?
    /// vtable 0x0a301a60
    class FGrannyStateMachineVariableContext : public FContext, public FIFiniteStateMachineInstanceProvider {
    public:
        /// 0x08cce4a4
        /// deleting 0x08cce6ec
        virtual ~FGrannyStateMachineVariableContext();
        /// 0x08cce72e
        virtual UNK_RET vf_01(UNK_PARAMS);
        /// 0x08cce74e
        virtual UNK_RET vf_03(UNK_PARAMS);
        /// 0x08cce784
        virtual UNK_RET QueryInterface(FClassId const& a1);
        /// 0x08cce7e2
        virtual UNK_RET vf_05(UNK_PARAMS);
        /// 0x08cce31a
        /// thunk 0x08cce32a
        virtual UNK_RET get_fsm_instance(unsigned int a1) const;
    };
    /// 0x08ccb4b0
    FGrannyAssetInstance(FGrannyAsset* a1, unsigned char a2);
    /// 0x08ccb63c
    /// deleting 0x08ccb614
    virtual ~FGrannyAssetInstance();
    /// 0x08cccac0
    virtual UNK_RET FindAnimationLayer(granny_blend_dag_node* a1, unsigned char a2) const;
    /// 0x08ccd212
    virtual UNK_RET Update(float a1);
    /// 0x08ccb9a0
    virtual UNK_RET GetEntData() const;
    /// 0x08ccb9a4
    virtual UNK_RET SetEntData(void* a1);
    /// 0x08ccdb6e
    virtual UNK_RET InstantiateDAGTree(granny_model* a1, unsigned char a2);
    /// 0x08ccdcaa
    virtual UNK_RET OnControlledAnimation(unsigned int a1, granny_animation const* a2, granny_sample_context* a3);
    /// 0x08ccde0c
    virtual UNK_RET OnTrackMaskBinding(FGrannyModelInstanceData const& a1, char const* a2, granny_track_mask** a3);
    /// 0x08ccde92
    virtual UNK_RET CreateVisibilityCacheForControl(FAnimationControlEntry& a1, unsigned int a2);
    /// 0x08ccde94
    virtual UNK_RET ProcessVisibilityTrackForControl(FAnimationControlEntry& a1);
    /// 0x08ccde98
    virtual UNK_RET CreateUVWTransformCacheForControl(FAnimationControlEntry& a1);
    /// 0x08ccde9a
    virtual UNK_RET ProcessUVWTransformForControl(FAnimationControlEntry& a1);
    /// 0x08ccde96
    virtual UNK_RET CreateTextCacheForControl(FAnimationControlEntry& a1);
    /// 0x08ccb5f4
    UNK_RET GetParentAsset(unsigned int a1) const;
    /// 0x08ccb604
    UNK_RET GetParentAsset(unsigned int a1);
    /// 0x08ccb736
    UNK_RET RemoveAllAnimations();
    /// 0x08ccb788
    UNK_RET RemoveAllStateMachines();
    /// 0x08ccb7e0
    UNK_RET RemoveAllTriggerSystems();
    /// 0x08ccb824
    UNK_RET ReleaseBlendDAGNodes();
    /// 0x08ccb8da
    UNK_RET ReleaseDSETs();
    /// 0x08ccb9a6
    UNK_RET GetBlendDAGNode(unsigned int a1) const;
    /// 0x08ccb9c6
    UNK_RET GetModelInstanceData(unsigned int a1) const;
    /// 0x08ccb9d4
    UNK_RET GetModelInstanceData(unsigned int a1);
    /// 0x08ccb9e2
    UNK_RET GetStateIDByName(char const* a1, unsigned int a2) const;
    /// 0x08ccba36
    UNK_RET GetCurrentStateID(unsigned int a1) const;
    /// 0x08ccba78
    UNK_RET GetStateTime(unsigned int a1) const;
    /// 0x08ccbab8
    UNK_RET ResetState(unsigned int a1, int a2, float a3);
    /// 0x08ccbb08
    UNK_RET HasAnimation(unsigned int a1, unsigned int a2) const;
    /// 0x08ccbb52
    UNK_RET SetStatePathOptionValue(unsigned int a1, int a2, float a3);
    /// 0x08ccbba2
    UNK_RET SetStatePathActionValue(unsigned int a1, float a2);
    /// 0x08ccbbf2
    UNK_RET GetStatePathOptionValue(unsigned int a1, int a2) const;
    /// 0x08ccbc3a
    UNK_RET GetStatePathActionValue(unsigned int a1) const;
    /// 0x08ccbc82
    UNK_RET GetStatePathActionResult(unsigned int a1) const;
    /// 0x08ccbcac
    UNK_RET ClearStatePathActionResultCheck(unsigned int a1);
    /// 0x08ccbce8
    UNK_RET SetStatePathActionResultCheck(unsigned int a1, FGrannyStateMachineInstance::ACTION_RESULT_CHECK_TYPE a2, int a3);
    /// 0x08ccbd34
    UNK_RET GetStatePathActionResultCheckHitCount(unsigned int a1) const;
    /// 0x08ccbd5e
    UNK_RET GetStateAnimationComplete(unsigned int a1) const;
    /// 0x08ccbd8c
    UNK_RET GetCurrentMaxGroupID() const;
    /// 0x08ccbdba
    UNK_RET GetUserIndexHandle() const;
    /// 0x08ccbdc2
    UNK_RET SetUserIndexHandle(unsigned int a1);
    /// 0x08ccbdce
    UNK_RET GetAnimationLayerCount() const;
    /// 0x08ccbdda
    UNK_RET AddAnimationLayer();
    /// 0x08ccbde6
    UNK_RET ApplyDummyTrackMasks();
    /// 0x08ccbe24
    UNK_RET ApplyDummyTrackMask(unsigned int a1);
    /// 0x08ccbec6
    UNK_RET GetAnimation(unsigned short a1, unsigned int a2) const;
    /// 0x08ccbefc
    UNK_RET GetAnimationName(unsigned short a1, unsigned int a2) const;
    /// 0x08ccbf32
    UNK_RET PlayAnimation(unsigned short a1, unsigned int a2, float a3, std::vector<granny_control*, FSTL_Tagged_Allocator<granny_control*, 1u, 27u, 4u> >* a4, float a5, unsigned int a6);
    /// 0x08ccbfdc
    void OnPlayAnimation(unsigned int a1, granny_animation const* a2, unsigned int a3, float a4, std::vector<granny_control*, FSTL_Tagged_Allocator<granny_control*, 1u, 27u, 4u> >* a5, float a6, unsigned int a7, unsigned int a8);
    /// 0x08ccc1f0
    UNK_RET PlayAnimationFromEventCode(unsigned int a1, unsigned int a2, float a3);
    /// 0x08ccc246
    UNK_RET PlayAnimation(granny_animation const* a1, unsigned int a2, float a3, std::vector<granny_control*, FSTL_Tagged_Allocator<granny_control*, 1u, 27u, 4u> >* a4, float a5, unsigned int a6, unsigned int a7);
    /// 0x08ccc2c8
    UNK_RET PlayAnimationByGroupID(unsigned int a1, unsigned short a2, unsigned int a3, float a4, std::vector<granny_control*, FSTL_Tagged_Allocator<granny_control*, 1u, 27u, 4u> >* a5, float a6, unsigned int a7);
    /// 0x08ccc382
    UNK_RET PlayAnimationByDAGNodeIndex(unsigned int a1, unsigned short a2, unsigned int a3, float a4, std::vector<granny_control*, FSTL_Tagged_Allocator<granny_control*, 1u, 27u, 4u> >* a5, float a6, unsigned int a7);
    /// 0x08ccc40c
    UNK_RET ControlledAnimationCallback(granny_animation const* a1, granny_sample_context* a2, void* a3, unsigned int a4);
    /// 0x08ccc44a
    UNK_RET StopAnimation(granny_control* a1, float a2);
    /// 0x08ccc568
    UNK_RET RemoveAnimation(granny_control* a1);
    /// 0x08ccc66e
    UNK_RET GetAnimationList() const;
    /// 0x08ccc676
    UNK_RET ForceStateMachineStateByID(int a1, unsigned int a2);
    /// 0x08ccc702
    UNK_RET ForceStateMachineStateByIndex(int a1, unsigned int a2);
    /// 0x08ccc7c4
    UNK_RET ForceStateMachineStateByName(int a1, char const* a2);
    /// 0x08ccc892
    UNK_RET StartFSMDump(int a1, unsigned int a2);
    /// 0x08ccc9a6
    UNK_RET CheckFSMDump(unsigned int a1) const;
    /// 0x08ccc9ee
    UNK_RET AddControl(granny_control* a1, unsigned int a2);
    /// 0x08cccb36
    UNK_RET StopAllAnimations(unsigned char a1, float a2);
    /// 0x08cccb98
    void StopAnimation(granny_blend_dag_node* a1, float a2);
    /// 0x08cccd28
    UNK_RET StopAllAnimationsByDAGNodeIndex(unsigned int a1, float a2);
    /// 0x08cccda2
    UNK_RET StopAllAnimations(float a1);
    /// 0x08ccce00
    void RemoveAllAnimations(unsigned char a1);
    /// 0x08cccfc4
    UNK_RET RemoveAllAnimationsByLayer(unsigned int a1);
    /// 0x08cccfee
    UNK_RET ProcessActiveControls();
    /// 0x08ccd05c
    UNK_RET GetGlobalProcessState(unsigned int a1);
    /// 0x08ccd07c
    UNK_RET GetProcessState(unsigned int a1) const;
    /// 0x08ccd092
    UNK_RET ReleaseCompletedControls();
    /// 0x08ccd1bc
    UNK_RET SetGlobalProcessState(unsigned int a1, bool a2);
    /// 0x08ccd1f2
    UNK_RET SetProcessState(unsigned int a1, bool a2);
    /// 0x08ccd316
    UNK_RET SetAccumulationMatrix(float const (&a1)[4][4]);
    /// 0x08ccd32e
    UNK_RET GetAccumulationMatrix() const;
    /// 0x08ccd336
    UNK_RET GetAccumulationMatrix();
    /// 0x08ccd33e
    UNK_RET ResetAccumulationMatrix();
    /// 0x08ccd362
    bool AddStateMachineInstance(FGrannyModelInstanceData& a1);
    /// 0x08ccd514
    UNK_RET AddTimedTriggerInstance(unsigned int a1, FGrannyTimedTriggerInstancer* a2);
    /// 0x08ccd6b6
    UNK_RET InitializeModelInstances(unsigned int a1);
    /// 0x08ccd708
    UNK_RET AddModelInstance(FGrannyAsset* a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08ccd764
    UNK_RET AddModelInstance(FGrannyAsset* a1, unsigned int a2, unsigned int a3, unsigned int a4, FGrannyTimedTriggerInstancer* a5);
    /// 0x08ccd846
    UNK_RET AddModelInstanceDAG2(FGrannyAsset* a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08ccd8a2
    UNK_RET AddModelInstanceDAG2(FGrannyAsset* a1, unsigned int a2, unsigned int a3, unsigned int a4, FGrannyTimedTriggerInstancer* a5);
    /// 0x08ccd986
    UNK_RET GetRootBoneTransform(unsigned int a1) const;
    /// 0x08ccda2c
    UNK_RET GetBoneTransformByName(unsigned int a1, unsigned int a2) const;
    /// 0x08ccddbe
    UNK_RET TrackMaskBindingCallback(char const* a1, void* a2, unsigned int a3, granny_track_mask** a4);
    /// 0x08ccde9c
    UNK_RET HasActiveControls(bool a1) const;
    /// 0x08ccdf38
    UNK_RET ReleaseCurrentControls();
    /// 0x08ccdf9e
    UNK_RET GetDSET(unsigned int a1);
    /// 0x08ccdfba
    UNK_RET GetDSETModelInstanceData(unsigned int a1);
    /// 0x08ccdfd6
    UNK_RET IsInitializedDSETs() const;
    /// 0x08ccdfe6
    UNK_RET InitializeDSETs(unsigned int a1);
    /// 0x08cce06a
    UNK_RET AddModelInstanceDSET(FGrannyAsset* a1, unsigned int a2, unsigned int a3, unsigned int a4, FGrannyTimedTriggerInstancer* a5);
    /// 0x08cce2f8
    UNK_RET GetDSETInitializedMemBegin() const;
    /// 0x0d276b18
    static UNK_DATA ms_kGlobalProcessFlags[0x1];

    char unk_4[0x24];                                   ///< +0x4
    unsigned int m_uiUserIndexHandle;                   ///< +0x28
    char unk_2c[0x10];                                  ///< +0x2c
    FGrannyStateMachineInstance** m_ppStateMachinesBegin; ///< +0x3c
    FGrannyStateMachineInstance** m_ppStateMachinesEnd;   ///< +0x40
    FGrannyStateMachineInstance** m_ppStateMachinesCap;   ///< +0x44
    char unk_48[0x4];                                   ///< +0x48
    FAnimationControlEntry* m_pControlsBegin;           ///< +0x4c
    FAnimationControlEntry* m_pControlsEnd;             ///< +0x50
    FAnimationControlEntry* m_pControlsCap;             ///< +0x54
    granny_blend_dag_node** m_ppDagNodesBegin;          ///< +0x58
    granny_blend_dag_node** m_ppDagNodesEnd;            ///< +0x5c
};

/// 0x08cd0233 exact_cast<FFSMConditionECComplete, FIFSMCondition>
template <class T0, class T1>
T0* exact_cast(T1* a1);
