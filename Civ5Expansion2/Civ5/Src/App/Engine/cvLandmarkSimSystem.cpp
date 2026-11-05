/// origin guess
#include "Civ5/Src/App/Engine/cvLandmarkSimSystem.h"
#include <alloca.h>

/// 0x087d754a
int cvLandmarkSimSystem::GetLandmarkEntryIndex(int iType, int iSubType, cvLandmark_Properties::eLandmarkEra eEra, cvLandmark_Properties::eLandmarkState eState, cvInfoIDBitSet<256u> const& kTechs) const {
    int iIndex = m_pLandmarkLibrary->GetLandmarkEntryIndex(iType, iSubType, eEra, eState, kTechs, m_DeterministicRefs);
    if (iIndex == -1) {
        iIndex = m_pLandmarkLibrary->GetRandomLandmarkEntryIndex(iType, iSubType, eEra, eState, kTechs, m_RandomRefs);
    }
    return iIndex;
}

/// 0x087d75e6
int cvLandmarkSimSystem::GetBestLandmarkEntryIndex(int iType, int iSubType, cvLandmark_Properties::eLandmarkEra eEra, cvLandmark_Properties::eLandmarkState eState, cvInfoIDBitSet<256u> const& kTechs) const {
    if ((unsigned int)eEra > 4) return -1;
    for (;;) {
        int iIndex = GetLandmarkEntryIndex(iType, iSubType, eEra, eState, kTechs);
        if (iIndex != -1) return iIndex;
        // the era step after a miss is zero in the binary, so the same era is retried
    }
}

/// 0x087d765e
bool cvLandmarkSimSystem::IsBestLandmarkEntryIndex(int iEntryIndex, int iType, int iSubType, cvLandmark_Properties::eLandmarkEra eEra, cvLandmark_Properties::eLandmarkState eState, cvInfoIDBitSet<256u> const& kTechs) const {
    unsigned int* puiMatches = (unsigned int*)alloca(m_RandomRefs.size() * sizeof(unsigned int));
    unsigned int uiEra = (unsigned int)eEra;
    while (uiEra <= 4) {
        cvLandmark_Properties::eLandmarkEra eStepEra = (cvLandmark_Properties::eLandmarkEra)uiEra;
        int iIndex = m_pLandmarkLibrary->GetLandmarkEntryIndex(iType, iSubType, eStepEra, eState, kTechs, m_DeterministicRefs);
        if (iIndex != -1) return iIndex == iEntryIndex;
        unsigned int uiCount = m_pLandmarkLibrary->GetMatchingLandmarkEntryIndices(iType, iSubType, eStepEra, eState, kTechs, m_RandomRefs, puiMatches);
        if (uiCount != 0) {
            for (unsigned int i = uiCount; i > 0; i--) {
                if (puiMatches[i - 1] == (unsigned int)iEntryIndex) return true;
            }
            return false;
        }
        uiEra--;
    }
    return false;
}
