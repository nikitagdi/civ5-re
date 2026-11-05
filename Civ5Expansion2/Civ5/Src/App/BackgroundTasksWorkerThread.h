/// origin guess
#pragma once

namespace Modding {
class System;
}

namespace Modding {
/// size ?
class BackgroundTasksWorkerThread {
public:
    /// 0x08c75d34
    BackgroundTasksWorkerThread(Modding::System* a1);
    /// 0x08c75d40
    UNK_RET operator()();
};
}
