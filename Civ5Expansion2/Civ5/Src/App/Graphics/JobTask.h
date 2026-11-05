/// origin guess
#pragma once
#include <tbb.h>

/// size ?
/// vtable 0x0a306640
class JobTask : public tbb::task {
public:
    /// 0x08870b52
    /// deleting 0x08dc291e
    virtual ~JobTask();
    /// 0x08dc2932
    virtual tbb::task* execute();
};
