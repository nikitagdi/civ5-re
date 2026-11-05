/// origin guess
#include <memory>
#include "Civ5/Src/App/InterfaceBuddy.h"
#include "Civ5/Src/App/CvInitMgr.h"

/// 0x086ad474
void InterfaceBuddy::centerCamera(ICvUnit1* a1) {
    if (a1 == NULL) {
        return;
    }
    std::auto_ptr<ICvPlot1> pPlot(a1->GetPlot());
    lookAt(pPlot.get(), CAMERALOOKAT_NORMAL);
}

/// 0x086ada06
bool InterfaceBuddy::canEndTurn() const {
    return m_bCanEndTurn;
}

/// 0x086ade92
bool InterfaceBuddy::isHasMovedUnit() {
    return m_bHasMovedUnit;
}

/// 0x086adeaa
void InterfaceBuddy::setHasMovedUnit(bool a1) {
    m_bHasMovedUnit = a1;
}

/// 0x086b2f7c
bool InterfaceBuddy::IsSPAutoEndTurnEnabled() {
    return cvOptionsManager::g_GameOptionsCache[0x3a] != 0;
}

/// 0x086b2fac
void InterfaceBuddy::SetSPAutoEndTurnEnabled(bool a1) {
    cvOptionsManager::g_GameOptionsCache[0x3a] = a1;
}

/// 0x086b2fe0
bool InterfaceBuddy::IsMPAutoEndTurnEnabled() {
    return cvOptionsManager::g_GameOptionsCache[0x3b] != 0;
}

/// 0x086b3010
void InterfaceBuddy::SetMPAutoEndTurnEnabled(bool a1) {
    cvOptionsManager::g_GameOptionsCache[0x3b] = a1;
}
