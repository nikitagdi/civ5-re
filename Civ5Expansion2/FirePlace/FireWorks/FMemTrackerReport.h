/// origin guess
#pragma once

/// size ?
class FMemTrackerReport {
public:
    class DataOrientation;
    class TableType;
    /// 0x08dbadd0
    FMemTrackerReport(unsigned int a1);
    /// 0x08dbaddc
    UNK_RET WriteTable(char const* a1, unsigned int a2, FMemTrackerReport::TableType a3, FMemTrackerReport::DataOrientation a4);
    /// 0x08dbae7c
    UNK_RET WriteFrameColumnTable(char const* a1, unsigned int a2, FMemTrackerReport::TableType a3);
    /// 0x08dbbbea
    UNK_RET WritePoolColumnTable(char const* a1, FMemTrackerReport::TableType a2);
    /// 0x08dbc892
    UNK_RET WriteFrameRowTable(char const* a1, unsigned int a2, FMemTrackerReport::TableType a3);
    /// 0x08dbd096
    UNK_RET WritePoolRowTable(char const* a1, FMemTrackerReport::TableType a2);
};

/// 0x08dbdd58
UNK_RET PoolIndex(unsigned int a1, unsigned int a2);
