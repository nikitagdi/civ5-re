/// origin guess
#pragma once
class _TAL_ID;

// Derived jobs keep their own fields from +0x10; their array constructors clear only +0x4.
/// size 0x10
/// vtable ?
class FJob {
public:
    /// pure
    virtual void Execute(unsigned int a1) = 0;

    int field_4;     ///< +0x4
    char unk_8[0x8]; ///< +0x8
};

/// size 0x4
class FJobSet {
public:
    int field_0;  ///< +0x0
    /// 0x08dc270c
    FJobSet();

    /// 0x08dc2718
    ~FJobSet();
    class JobSetSignal;
    /// size ?
    class JobSetSignal {
    public:
        /// 0x08dc2606
        JobSetSignal();
        /// 0x08dc2648
        ~JobSetSignal();
        /// 0x08dc2674
        UNK_RET Operator();
    };
    /// 0x08dc24bc
    UNK_RET Wait();
    /// 0x08dc2698
    UNK_RET WaitWithSignal(FJobSet::JobSetSignal& a1);
    /// 0x08dc2764
    UNK_RET AddJob(FJob* a1, char const* a2, _TAL_ID* a3);
};
