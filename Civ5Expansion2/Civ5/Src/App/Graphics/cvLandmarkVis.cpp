/// origin binary
#include "Civ5/Src/App/Graphics/cvLandmarkVis.h"

/// 0x08a2cb86
void cvLandmarkVis::SetFOWState(unsigned char ucFOWState) {
    m_ucFlags = (ucFOWState & 0x3) | (m_ucFlags & 0xfc);
}
