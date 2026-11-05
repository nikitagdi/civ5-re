/// origin guess
#pragma once

/// size 0x4
/// vtable ?
class cvTimeBase {
public:
    /// 0x08350010
    virtual void Alpha();
    /// 0x08350020
    virtual void vf_01(UNK_PARAMS);
};

/// size 0x4
/// vtable ?
class cvOther {
public:
    /// 0x08350060
    virtual void Gamma();
};

/// size 0xc
/// vtable ?
class cvTimed : public cvOther, public cvTimeBase {
public:
    /// 0x08350030
    virtual void Beta();
    /// 0x08350040
    virtual void vf_01(UNK_PARAMS);
};

/// size 0x4
/// vtable ?
template <class T> class cvPattern {
public:
    /// 0x08350050
    virtual void vf_00(UNK_PARAMS);
};
