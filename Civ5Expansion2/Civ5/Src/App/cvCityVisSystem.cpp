/// origin guess
#include "Civ5/Src/App/cvCityVisSystem.h"
#include "Civ5/Src/App/Graphics/cvCityVis.h"
#include <string.h>

/// 0x08854bb2
void cvCityVisSystem::OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    memcpy(&m_kFrameGlobals, &a1, sizeof(FrameGlobalsData));
    for (unsigned int uPass = 0; uPass < 3; uPass++) {
        for (unsigned int uCamera = 0; uCamera < 2; uCamera++) {
            UpdateGlobals(uCamera, uPass);
        }
    }
}

/// 0x0885587e
void cvCityVisSystem::OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    memcpy(&m_kWorldGlobals, &a1, sizeof(WorldGlobalsData));
}

/// 0x08855d76
void cvCityVisSystem::Update(float a1) {
    m_fPrevTime = m_fTime;
    m_fTime = a1;
}

/// 0x088574cc
void cvCityVisSystem::UpdateVisFOWState(cvCityVis& a1, unsigned char a2) {
    a1.SetFOWState(a2, false);
}
