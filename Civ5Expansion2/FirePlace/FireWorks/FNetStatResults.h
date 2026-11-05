/// origin guess
#pragma once

class FNetStatRecord;

/// size ?
/// vtable 0x0a305398
class FNetStatResults {
public:
    /// 0x08d76d98
    FNetStatResults();
    /// 0x08d76e08
    /// deleting 0x08d76dca
    virtual ~FNetStatResults();
    /// 0x08d76e40
    UNK_RET Clear();
    /// 0x08d76e98
    UNK_RET CreateRecord();
    /// 0x08d76f7c
    UNK_RET GetNumRecords() const;
    /// 0x08d76f8a
    UNK_RET GetRecord(int a1);
    /// 0x08d76faa
    UNK_RET SortRecords();
    /// 0x08d77000
    UNK_RET RankSortPred(FNetStatRecord const* a1, FNetStatRecord const* a2);
};
