/// origin guess
#pragma once

namespace FProfiler {
class TableType;
}

namespace cvProfileManager {
/// size ?
class PendingReport {
public:
    /// 0x08992738
    PendingReport(char const* a1, FProfiler::TableType a2, unsigned int a3, float a4);
};
}

namespace cvProfileManager {
/// 0x089927e4
UNK_RET Update();
/// 0x089927e6
UNK_RET SetFilterThreshold(float a1);
/// 0x089927fc
UNK_RET SetEnabled(bool a1);
/// 0x08992818
UNK_RET RequestReport(char const* a1, FProfiler::TableType a2, unsigned int a3, float a4);
}
