/// origin guess
#pragma once

class FNetStatBase;
class FNetStatOp;
class StatRequestType;

/// size ?
/// vtable 0x0a305350
class FNetStatRequest {
public:
    /// 0x08d761b0
    FNetStatRequest(FNetStatBase* a1, int a2, int a3, StatRequestType a4);
    /// 0x08d762cc
    /// deleting 0x08d762a4
    virtual ~FNetStatRequest();
    /// 0x08d764d6
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08d764dc
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08d764de
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x08d7635e
    UNK_RET AddColumn(int a1, bool a2);
    /// 0x08d763c6
    UNK_RET GetColumnAt(unsigned int a1) const;
    /// 0x08d763e6
    UNK_RET GetNumColumns() const;
    /// 0x08d763f4
    UNK_RET AddFilter(int a1, FNetStatOp a2, int a3);
    /// 0x08d7646a
    UNK_RET GetFilterAt(unsigned int a1) const;
    /// 0x08d7648a
    UNK_RET GetNumFilters() const;
    /// 0x0ff8ac60
    static UNK_DATA ms_iNextID[0x4];
};
