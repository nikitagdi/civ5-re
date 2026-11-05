/// origin binary
#pragma once

/// size ?
/// vtable ?
class FIUniqueId {
public:
    /// absent
    virtual ~FIUniqueId();
    /// pure
    virtual UNK_RET GetUniqueId() const = 0;
    /// pure
    virtual UNK_RET SetUniqueId(int a1) = 0;
};
