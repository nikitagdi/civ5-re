/// origin guess
#include "Civ5/Src/App/Engine/FGrannyStateMachineInstance.h"
#include <cstdint>

/// 0x08cb70b6
granny_dag2_instance* FGrannyStateMachineInstance::FGrannyContext::GetCurrentDAG2Instance() const {
    return m_pDagInstance;
}

/// 0x08cb759a
void FGrannyStateMachineInstance::Init(FGrannyModelInstanceData* a1, FGrannyStateMachine& a2, unsigned int a3) {
    m_pStateMachine = &a2;
    // The random stream is seeded with the instance address, so it differs per instance and per run.
    unsigned int seed = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(this));
    m_rand.m_uiState = seed;
    m_rand.m_uiCallCount = 0;
    m_rand.m_uiSeed = seed;
    vf_05(4, 0);
    m_context.init(a1);
}

/// 0x08cb763a
void FGrannyStateMachineInstance::on_leave(int a1, FFiniteStateMachineReferenceState<FGrannyState>& a2, int a3) {
    if (m_eActionResultCheck == ACTION_RESULT_CHECK_ON_LEAVE && m_iActionResult == m_iActionResultTarget) {
        ++m_iActionResultHits;
    }
    m_context.MakeDAG2Current(a3);
    SetContinuationOffset(a2, a3);
}

/// 0x08cb78d4
float FGrannyStateMachineInstance::GetAccumulatedTime() const {
    return m_context.m_fAccumulatedTime;
}

/// 0x08cb78e0
granny_dag2_instance* FGrannyStateMachineInstance::GetDAG2InstanceFromCurrentState() {
    return m_context.m_pDagInstance;
}

/// 0x08cb7a2e
void FGrannyStateMachineInstance::ClearActionResultCheck() {
    m_eActionResultCheck = ACTION_RESULT_CHECK_NONE;
    m_iActionResultTarget = 0;
    m_iActionResultHits = 0;
}

/// 0x08cb7a52
void FGrannyStateMachineInstance::SetActionResultCheck(int a1, FGrannyStateMachineInstance::ACTION_RESULT_CHECK_TYPE a2) {
    m_eActionResultCheck = a2;
    m_iActionResultTarget = a1;
    m_iActionResultHits = 0;
    if (a2 == ACTION_RESULT_CHECK_ON_ENTER && m_iActionResult == a1) {
        m_iActionResultHits = 1;
    }
}

/// 0x08cb7a8c
bool FGrannyStateMachineInstance::HasAnimation(unsigned int a1) const {
    return m_context.HasAnimation(static_cast<unsigned short>(a1));
}
