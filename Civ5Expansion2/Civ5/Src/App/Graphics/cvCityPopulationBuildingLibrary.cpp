/// origin guess
#include "Civ5/Src/App/Graphics/cvCityPopulationBuildingLibrary.h"
#include <Windows.h>
#include <alloca.h>

/// 0x088ac902
void cvCityPopulationBuildingLibrary::AllocateBuildingSets(unsigned int a1) {
    if (m_pBuildingSets != NULL) DeleteBuildingSets();
    m_pBuildingSets = new cvCityBuildingSet[a1];
    m_uBuildingSetCount = a1;
    for (unsigned int i = 0; i < a1; i++) {
        m_pBuildingSets[i].m_pDecals = &m_vDecals;
        m_pBuildingSets[i].m_pDecalVertices = &m_vDecalVertices;
        m_pBuildingSets[i].m_pDecalIndices = &m_vDecalIndices;
    }
}

/// 0x088acd56
void cvCityPopulationBuildingLibrary::DeleteBuildingSets() {
    delete[] m_pBuildingSets;
    m_pBuildingSets = NULL;
    m_uBuildingSetCount = 0;
}

/// 0x088acdc0
bool cvCityPopulationBuildingLibrary::SelectBuilding(unsigned int a1, eCityPopulationBuildingHeights a2, FireGrafix::FGXModelTemplate const*& a3, unsigned int& a4, unsigned int& a5) const {
    if (m_pBuildingSets == NULL || a1 >= m_uBuildingSetCount) return false;
    cvCityBuildingSet const& kSet = m_pBuildingSets[a1];
    a3 = reinterpret_cast<FireGrafix::FGXModelTemplate const*>(&kSet);
    a5 = kSet.m_uSurfaceCount;
    unsigned int uNodes = (unsigned int)(kSet.m_pNodesEnd - kSet.m_pNodesBegin);
    unsigned int* puMatches = (unsigned int*)alloca(uNodes * sizeof(unsigned int));
    unsigned int uMatches = 0;
    for (unsigned int i = 0; i < uNodes; i++) {
        if (kSet.m_pNodesBegin[i].m_ucHeight == (int)a2) puMatches[uMatches++] = i;
    }
    if (uMatches == 0) {
        a3 = NULL;
        return false;
    }
    a4 = puMatches[rand_wincompatible() % uMatches];
    return true;
}

/// 0x088ace86
void cvCityPopulationBuildingLibrary::ReloadAsset(unsigned int a1) {
    m_pBuildingSets[a1].Reload(m_vShaderPackageInstances);
}
