/// origin guess
#pragma once
#include "FirePlace/FireWorks/FContext.h"
#include "FirePlace/FireWorks/FFiniteStateMachineInstance.h"
#include "FirePlace/FireWorks/FGrannyAssetInstanceProvider.h"
#include "FirePlace/FireWorks/FIFiniteStateMachineInstanceProvider.h"
#include "FirePlace/FireWorks/FIFiniteStateMachineState.h"
#include "FirePlace/FireWorks/FIFiniteStateMachineTime.h"
#include "FirePlace/FireWorks/FILogFileProvider.h"
#include "FirePlace/FireWorks/FRandom.h"
#include <granny.h>

class FClassId;
class FGrannyModelInstanceData;
class FGrannyState;
class FGrannyStateMachine;
template <class T0> class FFiniteStateMachineReferenceState;

/// size 0xb0
/// vtable 0x0a301530
class FGrannyStateMachineInstance : public FFiniteStateMachineInstance<FGrannyState> {
public:
    class FGrannyContext;
    enum ACTION_RESULT_CHECK_TYPE : int {
        ACTION_RESULT_CHECK_NONE = 0,
        ACTION_RESULT_CHECK_ON_ENTER = 1,
        ACTION_RESULT_CHECK_ON_LEAVE = 2,
    };
    /// size 0x34
    /// vtable 0x0a301420
    class FGrannyContext : public FContext, public FIFiniteStateMachineInstanceProvider, public FIFiniteStateMachineTime, public FGrannyAssetInstanceProvider, public FIFiniteStateMachineState, public FILogFileProvider {
    public:
        /// 0x08cb6c30
        FGrannyContext();
        /// 0x08cb7abc
        /// deleting 0x08cb7bf4
        virtual ~FGrannyContext();
        /// 0x08cb7c08
        virtual UNK_RET vf_01(UNK_PARAMS);
        /// 0x08cb7c28
        virtual UNK_RET vf_03(UNK_PARAMS);
        /// 0x08cb7c5e
        virtual UNK_RET vf_04(UNK_PARAMS);
        /// 0x08cb7c6e
        virtual UNK_RET vf_05(UNK_PARAMS);
        /// 0x08cb7c7e
        virtual UNK_RET vf_06(UNK_PARAMS);
        /// 0x08cb7b26
        virtual UNK_RET vf_07(UNK_PARAMS);
        /// 0x08cb6e9a
        /// thunk 0x08cb6ec6
        virtual UNK_RET GetCurrentState() const;
        /// 0x08cb6ef2
        /// thunk 0x08cb6f2e
        virtual UNK_RET GetMinimumDuration(int a1) const;
        /// 0x08cb6f5a
        /// thunk 0x08cb6f8c
        virtual UNK_RET GetCurrentStateMinimumDuration() const;
        /// 0x08cb7c86
        virtual UNK_RET vf_11(UNK_PARAMS);
        /// 0x08cb6fc2
        /// thunk 0x08cb6fe6
        virtual UNK_RET get_fsm_instance(unsigned int a1) const;
        /// 0x08cb700a
        /// thunk 0x08cb7036
        virtual UNK_RET GetLogFile() const;
        /// 0x08cb7062
        /// thunk 0x08cb706e
        virtual UNK_RET GetAssetInstance();
        /// 0x08cb6d88
        /// thunk 0x08cb6e6e
        virtual UNK_RET QueryInterface(FClassId const& a1);
        /// 0x08cb6ca6
        void init(FGrannyModelInstanceData* a1);
        /// 0x08cb707a
        granny_dag2* GetDAG2Source(int a1) const;
        /// 0x08cb70b6
        granny_dag2_instance* GetCurrentDAG2Instance() const;
        /// 0x08cb70be
        bool MakeDAG2Current(int a1);
        /// 0x08cb721e
        bool HasAnimation(int a1, unsigned short a2) const;
        /// 0x08cb727c
        bool HasAnimation(unsigned short a1) const;
        /// 0x08cb7316
        void release();
        /// 0x08cb732a
        void Reset();
        /// 0x08cb7376
        bool is_initialized() const;

        FGrannyModelInstanceData* m_pModelInstance;     ///< +0x20
        granny_dag2_instance* m_pDagInstance;           ///< +0x24
        void* m_pDagInstanceBuffer;                     ///< +0x28
        unsigned int field_2c;                          ///< +0x2c
        float m_fAccumulatedTime;                       ///< +0x30
    };
    /// 0x08cb7390
    FGrannyStateMachineInstance();
    /// 0x08cb74f0
    /// deleting 0x08cb74c8
    virtual ~FGrannyStateMachineInstance();
    /// 0x08cb78f4
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08cb78ec
    virtual UNK_RET get_context();
    /// 0x08cb75f6
    virtual void on_enter(int a1, FFiniteStateMachineReferenceState<FGrannyState>& a2, int a3);
    /// 0x08cb763a
    virtual void on_leave(int a1, FFiniteStateMachineReferenceState<FGrannyState>& a2, int a3);
    /// 0x08cb7778
    virtual UNK_RET start_log(char const* a1, unsigned int a2);
    /// 0x087fbb18
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x087fbb38
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cb759a
    virtual void Init(FGrannyModelInstanceData* a1, FGrannyStateMachine& a2, unsigned int a3);
    /// 0x08cb769e
    void SetContinuationOffset(FFiniteStateMachineReferenceState<FGrannyState>& a1, int a2);
    /// 0x08cb78d4
    float GetAccumulatedTime() const;
    /// 0x08cb78e0
    granny_dag2_instance* GetDAG2InstanceFromCurrentState();
    /// 0x08cb7932
    void CheckAnyComplete(FGrannyModelInstanceData& a1, granny_sample_context* a2);
    /// 0x08cb796e
    void SetContinuationOffset(int a1, float a2);
    /// 0x08cb79d2
    void UpdateWatchedClocks(granny_animation const* a1, float a2);
    /// 0x08cb7a2e
    void ClearActionResultCheck();
    /// 0x08cb7a52
    void SetActionResultCheck(int a1, FGrannyStateMachineInstance::ACTION_RESULT_CHECK_TYPE a2);
    /// 0x08cb7a8c
    bool HasAnimation(unsigned int a1) const;

    char unk_4[0x34];                               ///< +0x4
    FRandom m_rand;                                 ///< +0x38
    FGrannyStateMachine* m_pStateMachine;           ///< +0x50
    char unk_54[0x8];                               ///< +0x54
    FGrannyContext m_context;                       ///< +0x5c
    bool m_bAnyComplete;                            ///< +0x90
    unsigned int m_continueClocks[3];               ///< +0x94
    int m_iActionResult;                            ///< +0xa0
    ACTION_RESULT_CHECK_TYPE m_eActionResultCheck;  ///< +0xa4
    int m_iActionResultTarget;                      ///< +0xa8
    int m_iActionResultHits;                        ///< +0xac
};
