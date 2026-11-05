/// origin binary
#pragma once

class FIMaker;
template <class T0> class FTPointer;

/// size ?
class FFactory {
public:
    /// 0x08dc48e8
    FFactory();
    /// 0x08dc4902
    UNK_RET Add(FTPointer<FIMaker> const& a1);
    /// 0x08dc493a
    UNK_RET Clear();
    /// 0x08dc4984
    UNK_RET GetMakers() const;
    /// 0x08dc498a
    UNK_RET FindMaker(char const* a1) const;
};

/// 0x08dc539a
UNK_RET GetUTF8StrLen(unsigned char* a1, unsigned char* a2, unsigned int* a3);
