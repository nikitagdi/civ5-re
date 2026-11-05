/// origin guess
#pragma once

class DispatchData;
class EventStream;
class EventStreamIterator;
template <class T0> class GameCoreEventQueue;

/// size ?
template <class T0, class T1>
class GameCoreEventDispatcher {
public:
    /// 0x086ef766 GameCoreEventDispatcher<GameCoreEventQueue<EventStream>, DispatchData>
    UNK_RET DispatchEvents(EventStream* a1, T1* a2);
    /// 0x086efe00 GameCoreEventDispatcher<GameCoreEventQueue<EventStream>, DispatchData>
    UNK_RET DispatchEvents(EventStream* a1, EventStreamIterator a2, T1* a3);
};
