/// origin guess
#include "Civ5/Src/App/cvFileChangeNotification.h"

/// 0x08767274
cvFileChangeNotification::cvFileChangeNotification() {
    m_hNotification = -1;
    field_4 = -1;
    m_bChanged = false;
    m_ftLastWrite.dwLowDateTime = 0;
    m_ftLastWrite.dwHighDateTime = 0;
}

/// 0x087674f6
bool cvFileChangeNotification::HasChanged() const {
    return m_bChanged;
}
