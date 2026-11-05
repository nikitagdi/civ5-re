/// origin guess
#pragma once

class FNetStatOp;
class StatRequestType;

/// size ?
/// vtable 0x0a3052d0
class FNetStatBase {
public:
    /// 0x08d75acc
    FNetStatBase(int a1);
    /// 0x08d75b3a
    /// deleting 0x08d75b12
    virtual ~FNetStatBase();
    /// 0x08d75ef2
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08d75b96
    virtual UNK_RET Update();
    /// 0x08d75ef8
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x08d75efa
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x08d75f00
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08d75f06
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08d75f0c
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08d75f12
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x08d75f18
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x08d75f1c
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x08d75f20
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x08d75f24
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x08d75f28
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x08d75f2c
    virtual UNK_RET vf_15(UNK_PARAMS);
    /// 0x08d75f30
    virtual UNK_RET vf_16(UNK_PARAMS);
    /// 0x08d75c44
    virtual UNK_RET CreateLocalRequest(int a1, int a2);
    /// 0x08d75c4e
    virtual UNK_RET CreatePlayerRequest(int a1, int a2, int a3, int a4, int a5);
    /// 0x08d75c54
    virtual UNK_RET CreateRankRequest(int a1, int a2, int a3);
    /// 0x08d75c5a
    virtual UNK_RET CreateFriendsRequest(int a1);
    /// 0x08d75f34
    virtual UNK_RET vf_21(UNK_PARAMS);
    /// 0x08d75ca8
    virtual UNK_RET AddRequestColumn(int a1, int a2, bool a3);
    /// 0x08d75d64
    virtual UNK_RET AddRequestFilter(int a1, int a2, FNetStatOp a3, int a4);
    /// 0x08d75ddc
    virtual UNK_RET SubmitRequest(int a1);
    /// 0x08d75e3a
    virtual UNK_RET FreeRequest(int a1);
    /// 0x08d75f36
    virtual UNK_RET vf_26(UNK_PARAMS);
    /// 0x08d75f3a
    virtual UNK_RET vf_27(UNK_PARAMS);
    /// 0x08d75c10
    UNK_RET GetProfile() const;
    /// 0x08d75c4a
    UNK_RET CreateRequest(int a1, int a2, StatRequestType a3);
    /// 0x08d75d18
    UNK_RET GetRequest(int a1);
    /// 0x08d75e88
    UNK_RET RequestComplete(int a1, bool a2);
    /// 0x08d75ed8
    UNK_RET GetStatKey(int a1, int a2);
    /// 0x08d75ee4
    UNK_RET GetStatID(int a1);
    /// 0x08d75eea
    UNK_RET GetBoardID(int a1);
};

namespace FStatFactory {
/// 0x08d75c40
UNK_RET CreateStatManager(int a1);
}
