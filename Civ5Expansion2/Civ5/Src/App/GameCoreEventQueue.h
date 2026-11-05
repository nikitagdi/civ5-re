/// origin guess
#pragma once

class EventStream;
class GameCoreEventEntryHeader;

/// size ?
template <class T0>
class GameCoreEventQueue {
public:
    /// 0x086ef8de GameCoreEventQueue<EventStream>
    UNK_RET PublishNonConflictingEvents();
    /// 0x086efac2 GameCoreEventQueue<EventStream>
    UNK_RET IsConflictingEvent(GameCoreEventEntryHeader const& a1);
    /// 0x088126fe GameCoreEventQueue<EventStream>
    UNK_RET FindPublishedEntry(unsigned int a1);
};
