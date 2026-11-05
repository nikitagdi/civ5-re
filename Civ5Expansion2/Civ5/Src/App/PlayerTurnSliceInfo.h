/// origin guess
#pragma once

namespace Civ5NetMessage {
/// size ?
class PlayerTurnSliceInfo {
public:
    /// 0x0870d334
    PlayerTurnSliceInfo();
    /// 0x0870d368
    UNK_RET Clear();
    /// 0x0870d39c
    UNK_RET GetAverage();
    /// 0x0870d3c0
    UNK_RET UpdateLimits();
    /// 0x0870d414
    UNK_RET Update(unsigned int a1);
};
}

namespace Civ5NetMessage {
/// 0x0870d4b4
UNK_RET GetPlayerTurnSliceInfo(int a1);
/// 0x0870d4da
UNK_RET GetLocalTurnSliceInfo();
}
