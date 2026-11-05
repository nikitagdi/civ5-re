/// origin binary
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMapTypeGroup.h"
#include <string.h>

/// 0x08cac9ee
bool CvWorldBuilderMapTypeGroup::Contains(const char *szType) const {
    if (szType == nullptr || *szType == '\0') {
        return false;
    }
    if (m_szBuffer == nullptr || m_uiBufferSize == 0) {
        return false;
    }
    // Entries are packed as [length byte][string][NUL]; the next entry follows the NUL.
    uint uiOffset = 0;
    do {
        if (strcmp(m_szBuffer + uiOffset + 1, szType) == 0) {
            return true;
        }
        uiOffset += 2 + m_szBuffer[uiOffset];
    } while (uiOffset < m_uiBufferSize);
    return false;
}
