/// origin guess
#pragma once
#include <tbb.h>

/// size ?
/// vtable 0x0a3066c0
class ThreadIdObserver : public tbb::internal::task_scheduler_observer_v3 {
public:
    /// 0x08dc29be
    virtual ~ThreadIdObserver();
    /// 0x08dc2a0a
    virtual void on_scheduler_exit(bool a1);
};
