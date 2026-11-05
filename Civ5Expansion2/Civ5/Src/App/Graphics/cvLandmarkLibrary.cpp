/// origin binary
#include "Civ5/Src/App/Graphics/cvLandmarkLibrary.h"
#include "Civ5/Src/App/cvDllUnitList.h"
#include <alloca.h>
#include <memory>
#include <stdlib.h>
#include <string.h>

/// 0x08876a66
cvLandmarkLibrary::eLandmarkLayoutHandler cvLandmarkLibrary::cvLandmarkLibraryEntry::StringToLandmarkLayoutHandler(char const* a1) const {
    if (strcmp(a1, "FARM") == 0) return LANDMARK_LAYOUT_FARM;
    if (strcmp(a1, "PASTURE") == 0) return LANDMARK_LAYOUT_PASTURE;
    if (strcmp(a1, "SPECIAL") == 0) return LANDMARK_LAYOUT_SPECIAL;
    if (strcmp(a1, "SPRING") == 0) return LANDMARK_LAYOUT_SPRING;
    if (strcmp(a1, "ANIMATED") == 0) return LANDMARK_LAYOUT_ANIMATED;
    if (strcmp(a1, "ANIMATED_ROTATED") == 0) return LANDMARK_LAYOUT_ANIMATED_ROTATED;
    if (strcmp(a1, "RANDOM") == 0) return LANDMARK_LAYOUT_RANDOM;
    return LANDMARK_LAYOUT_NONE;
}

/// 0x08875e1c
int cvLandmarkLibrary::cvLandmarkLibraryReference::IsValidForTech(cvInfoIDBitSet<256u> const& a1) const {
    unsigned int uiHash = m_pEntry->m_uiTechHash;
    if (uiHash == 0) return 1;
    std::auto_ptr<ICvGameContext1> pContext = GameCore::GetContext();
    if (pContext.get() == NULL) return 0;
    int iTech = pContext->GetInfoTypeForHash(uiHash, true);
    unsigned char const* pBits = reinterpret_cast<unsigned char const*>(&a1);
    if (iTech >= 0 && iTech <= 255 && ((pBits[iTech >> 3] >> (iTech & 7)) & 1) != 0) return 2;
    return 0;
}

/// 0x08875caa
int cvLandmarkLibrary::GetLandmarkEntryIndex(int iType, int iSubType, cvLandmark_Properties::eLandmarkEra a3, cvLandmark_Properties::eLandmarkState a4, cvInfoIDBitSet<256u> const& a5, FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> const& a6) const {
    int iEra = (int)a3;
    int iState = (int)a4;
    int iBest = -1;
    int iBestScore = 0;
    if (iType < 0) {
        // iSubType holds the landmark type; the last registered entry of that type wins
        for (int i = (int)a6.size() - 1; i >= 0; i--) {
            cvLandmarkLibraryEntry const& entry = *a6[i].m_pEntry;
            if (entry.m_iType != iSubType) continue;
            if (!(entry.m_iEra <= iEra || entry.m_iEra == 0 || iEra == 0)) continue;
            iBest = i;
            break;
        }
    } else {
        for (int i = (int)a6.size() - 1; i >= 0; i--) {
            cvLandmarkLibraryReference const& ref = a6[i];
            cvLandmarkLibraryEntry const& entry = *ref.m_pEntry;
            if (entry.m_iType != iType) continue;
            if (entry.m_iSubType != iSubType && entry.m_iSubType != -2) continue;
            if (!(entry.m_iEra <= iEra || entry.m_iEra == 0 || iEra == 0)) continue;
            if (!(entry.m_iState == iState || entry.m_iState == 1 || iState == 1)) continue;
            int iScore = ref.IsValidForTech(a5);
            if (iScore <= 0) continue;
            if (iBest == -1 || iBestScore < iScore) {
                iBest = i;
                iBestScore = iScore;
            }
        }
    }
    if (iBest == -1) return -1;
    char const* szPath = a6[iBest].m_pEntry->m_szAssetPath;
    char const* szName = strrchr(szPath, '\\');
    if (szName == NULL) szName = strrchr(szPath, '/');
    szName = (szName != NULL) ? szName + 1 : szPath;
    if (*szName == '\0') return -2;
    return (int)a6[iBest].m_uiEntryIndex;
}

/// 0x08875ede
unsigned int cvLandmarkLibrary::GetMatchingLandmarkEntryIndices(int a1, int a2, cvLandmark_Properties::eLandmarkEra a3, cvLandmark_Properties::eLandmarkState a4, cvInfoIDBitSet<256u> const& a5, FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> const& a6, unsigned int* a7) const {
    unsigned int uiCount = 0;
    bool bTechGatedFound = false;
    int iEra = (int)a3;
    int iState = (int)a4;
    for (unsigned int i = 0; i < a6.size(); i++) {
        cvLandmarkLibraryReference const& ref = a6[i];
        cvLandmarkLibraryEntry const& entry = *ref.m_pEntry;
        bool bEraOk = entry.m_iEra <= iEra || entry.m_iEra == 0 || iEra == 0;
        if (a1 < 0 && entry.m_iType == a2 && bEraOk) {
            a7[uiCount++] = ref.m_uiEntryIndex;
        } else if (entry.m_iType == a1 && (entry.m_iSubType == a2 || entry.m_iSubType == -2) && bEraOk &&
                   (entry.m_iState == iState || entry.m_iState == 1 || iState == 1) && ref.IsValidForTech(a5) != 0) {
            unsigned int uiTechHash = entry.m_uiTechHash;
            if (!bTechGatedFound && uiTechHash != 0) {
                uiCount = 0;
                bTechGatedFound = true;
            }
            if (!bTechGatedFound || uiTechHash != 0) {
                a7[uiCount++] = ref.m_uiEntryIndex;
            }
        }
    }
    return uiCount;
}

/// 0x08876050
int cvLandmarkLibrary::GetRandomLandmarkEntryIndex(int a1, int a2, cvLandmark_Properties::eLandmarkEra a3, cvLandmark_Properties::eLandmarkState a4, cvInfoIDBitSet<256u> const& a5, FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> const& a6) const {
    unsigned int* puiMatches = (unsigned int*)alloca(a6.size() * sizeof(unsigned int));
    unsigned int uiCount = GetMatchingLandmarkEntryIndices(a1, a2, a3, a4, a5, a6, puiMatches);
    if (uiCount == 0) return -1;
    return (int)puiMatches[(unsigned int)rand_wincompatible() % uiCount];
}
