/// origin binary
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"

namespace FireWorks {

/// 0x0a300000
int cvUnitVisSystem::s_iInstances = 0;

/// 0x0834a000
cvUnitVisSystem::cvUnitVisSystem() : m_iCount(0) {}

/// 0x0834a1c0
void cvUnitVisSystem::OnEvent(int e) { m_iCount += e; }

}  // namespace FireWorks

/// 0x08100000 FFastVector<int>
template <class T>
void FFastVector<T>::push_back(const T& v) { m_pData[m_iCount++] = v; }
