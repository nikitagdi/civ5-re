/// origin guess
#pragma once

/// size ?
class CvPlayerController {
public:
    /// 0x0875992c
    UNK_RET Initialize(int a1);
    /// 0x0875995e
    UNK_RET GetLastAICivsProcessed() const;
    /// 0x08759966
    UNK_RET SetLastAICivsProcessed(int a1);
    /// 0x08759972
    UNK_RET GetLastInitialAIProcessed() const;
    /// 0x0875997a
    UNK_RET SetLastInitialAIProcessed(int a1);
    /// 0x08759986
    UNK_RET HasSentNetLoopBarrier() const;
    /// 0x08759990
    UNK_RET SentNetLoopBarrier(bool a1);
    /// 0x0875999c
    UNK_RET HasSentNetLoopBarrierComplete() const;
    /// 0x087599a6
    UNK_RET SentNetLoopBarrierComplete(bool a1);
    /// 0x087599b2
    UNK_RET TurnChanged(int a1);
    /// 0x087599be
    UNK_RET HasSentRandomNumberGeneratorSyncCheck() const;
    /// 0x087599c8
    UNK_RET SentRandomNumberGeneratorSyncCheck(bool a1);
    /// 0x087599d4
    UNK_RET SetReceivedTurnComplete(bool a1);
    /// 0x08759ae8
    UNK_RET HasReceivedTurnComplete() const;
    /// 0x08759af2
    UNK_RET SetReceivedTurnAllComplete(bool a1);
    /// 0x08759be0
    UNK_RET HasReceivedTurnAllComplete() const;
    /// 0x08759c68
    UNK_RET Reconnected();
    /// 0x08759d98
    UNK_RET RestoreTurnCompleteStatus();
};
