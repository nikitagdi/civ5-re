/// origin guess
#include "Civ5/Src/App/Engine/cvDynamicGeometryVisSystem.h"
#include <string.h>

/// 0x087d0d9c
void cvDynamicGeometryVisSystem::Update(float a1) {
}

/// 0x087d0d9e
void cvDynamicGeometryVisSystem::RenderInternal() {
}

/// 0x087d1868
void cvDynamicGeometryVisSystem::OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
}

/// 0x087d186a
void cvDynamicGeometryVisSystem::OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
}

/// 0x087d186c
void cvDynamicGeometryVisSystem::OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    memcpy(&m_kWorldGlobals, &a1, sizeof(WorldGlobalsData));
}
