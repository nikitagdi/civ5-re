/// origin guess
#pragma once

class FNetMessage;

/// size ?
/// vtable 0x0a304cc8
class FNetMessageSync {
public:
    /// 0x08d64ec4
    FNetMessageSync(int a1);
    /// 0x08d64fb0
    /// deleting 0x08d64f76
    virtual ~FNetMessageSync();
    /// 0x08d64fea
    UNK_RET Update(float a1);
    /// 0x08d650b0
    UNK_RET GetSession() const;
    /// 0x08d650e0
    UNK_RET Get();
    /// 0x08d651c2
    UNK_RET SetMessageData(FNetMessage& a1);
    /// 0x08d651e2
    UNK_RET Add(FNetMessage& a1);
    /// 0x08d6520e
    UNK_RET HasTurnElapsed();
    /// 0x08d6526a
    UNK_RET RecordTurnStartTime();
    /// 0x08d6527a
    UNK_RET GetCurrentTimeStep() const;
    /// 0x08d652b8
    UNK_RET GetElapsedTurnTime() const;
    /// 0x08d652d2
    UNK_RET ForceCommTurn(unsigned int a1);
    /// 0x08d652f2
    UNK_RET ResetSyncingTime();
    /// 0x08d65338
    UNK_RET SetSyncPaused(bool a1);
};
