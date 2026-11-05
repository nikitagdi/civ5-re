/// origin guess
#pragma once
#include "FirePlace/FireWorks/FServerBrowserFilter.h"

/// size ?
class FServerBrowserFilters {
public:
    /// 0x08d42d2e
    UNK_RET Find(char const* a1) const;
    /// 0x08d42d82
    UNK_RET Find(char const* a1);
    /// 0x08d42dd6
    UNK_RET Add(char const* a1, char const* a2, int a3, FServerBrowserFilter::FilterApplyType a4);
    /// 0x08d42ed2
    UNK_RET Remove(char const* a1);
    /// 0x08d42f38
    UNK_RET Get(char const* a1) const;
};
