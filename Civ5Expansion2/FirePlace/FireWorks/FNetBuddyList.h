/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a305458
class FNetBuddyList {
public:
    /// 0x08d78574
    FNetBuddyList();
    /// 0x08d7861a
    /// deleting 0x08d785f2
    virtual ~FNetBuddyList();
    /// 0x08d78668
    UNK_RET Clear();
    /// 0x08d786d8
    UNK_RET CreateBuddy(int a1);
    /// 0x08d787c6
    UNK_RET GetUniqueProfileID();
    /// 0x08d787d4
    UNK_RET GetBuddy(int a1);
    /// 0x08d78820
    UNK_RET RemoveBuddy(int a1);
    /// 0x08d78a1a
    UNK_RET GetNumBuddies();
    /// 0x08d78a28
    UNK_RET GetBuddyAtIndex(unsigned int a1);
    /// 0x08d78a8c
    UNK_RET IndexUsername(int a1, FStringA const& a2);
    /// 0x08d78b22
    UNK_RET FindBuddy(FStringA const& a1);
};
