/// origin guess
#pragma once

/// size ?
class InitWSA {
public:
    /// 0x08d5b82a
    InitWSA();
    /// 0x08d5b862
    ~InitWSA();
};

/// 0x08d5b9c0
UNK_RET hex_to_int(char const* a1, int a2, int& a3);
/// 0x08d5ba29
UNK_RET ch_to_hex(char a1, char* a2);

namespace FUrlEncoding {
/// 0x08d5ba62
UNK_RET Encode(char const* a1, unsigned int a2, char* a3, unsigned int a4, unsigned int& a5, unsigned int& a6);
/// 0x08d5bb94
UNK_RET Decode(char const* a1, unsigned int a2, char* a3, unsigned int a4, unsigned int& a5, unsigned int& a6);
/// 0x08d5bcca
UNK_RET DecodeInPlace(char* a1, unsigned int a2, unsigned int& a3);
}
