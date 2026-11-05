/// origin binary
#pragma once

/// size 0x4
/// vtable ?
class cvBase {
public:
    /// 0x0834a030
    /// deleting 0x0834a040
    virtual ~cvBase();
};

/// size 0x4
/// vtable ?
class cvAliased : public cvBase {
public:
    /// 0x0834a030
    virtual ~cvAliased();
};
