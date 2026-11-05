/// origin guess
#pragma once
#include "Civ5/Src/App/EventSystemRecorder.h"
#include "Civ5/Src/App/EventSystem_Recordable.h"

class DispatchData;
class EventStream;
class EventSystemRecorder;
class cvSequencerEventRegistrar;
template <class T0, class T1, class T2, unsigned int T3, unsigned int T4> class EventSystem;
template <class T0, class T1> class EventDispatcher;
template <class T0, class T1> class EventSystem_Recordable;
template <class T0> class EventQueue;

/// size ?
/// vtable 0x0a2fd210 EventSystem_Sequencer<EventSystem<EventQueue<EventStream>, EventDispatcher<EventQueue<EventStream>, DispatchData>, cvSequencerEventRegistrar, 16u, 1u>, EventQueue<EventStream>, EventDispatcher<EventQueue<EventStream>, DispatchData>, cvSequencerEventRegistrar, 128u, 1u>
template <class T0, class T1, class T2, class T3, unsigned int T4, unsigned int T5>
class EventSystem_Sequencer : public EventSystem_Recordable<T0, T1>, public EventSystemRecorder {
public:
    /// 0x0881e120 EventSystem_Sequencer<EventSystem<EventQueue<EventStream>, EventDispatcher<EventQueue<EventStream>, DispatchData>, cvSequencerEventRegistrar, 16u, 1u>, EventQueue<EventStream>, EventDispatcher<EventQueue<EventStream>, DispatchData>, cvSequencerEventRegistrar, 128u, 1u>
    /// deleting 0x0881e3aa EventSystem_Sequencer<EventSystem<EventQueue<EventStream>, EventDispatcher<EventQueue<EventStream>, DispatchData>, cvSequencerEventRegistrar, 16u, 1u>, EventQueue<EventStream>, EventDispatcher<EventQueue<EventStream>, DispatchData>, cvSequencerEventRegistrar, 128u, 1u>
    virtual ~EventSystem_Sequencer();
};
