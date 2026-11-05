/// origin guess
#pragma once

// marker-misplaced: deleting on a non-virtual destructor.
/// size 0x1
class Misplaced {
public:
    /// 0x08000000
    /// deleting 0x08000040
    ~Misplaced();
};
