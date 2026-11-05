/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"
#include <memory>
#include <string>

/// size ?
class FXmlNVPair {
public:
    /// 0x08d8f3cc
    UNK_RET GetNameA(char const*& a1, unsigned int& a2) const;
    /// 0x08d8f438
    UNK_RET GetNameUtf8(char const*& a1, unsigned int& a2) const;
    /// 0x08d8f470
    UNK_RET GetName(std::string& a1) const;
    /// 0x08d8f4ca
    UNK_RET GetName(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& a1) const;
    /// 0x08d8f5e2
    bool GetName(FStringA& a1) const;
    /// 0x08d8f644
    UNK_RET GetVal(bool& a1) const;
    /// 0x08d8f6de
    UNK_RET GetValA(char const*& a1, unsigned int& a2) const;
    /// 0x08d8f744
    UNK_RET GetVal(double& a1) const;
    /// 0x08d8f7b2
    bool GetVal(float& a1) const;
    /// 0x08d8f820
    UNK_RET GetVal(int& a1) const;
    /// 0x08d8f88e
    UNK_RET GetVal(short& a1) const;
    /// 0x08d8f8fc
    UNK_RET GetVal(unsigned int& a1) const;
    /// 0x08d8f96a
    UNK_RET GetVal(std::string& a1) const;
    /// 0x08d8f9c4
    UNK_RET GetVal(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& a1) const;
    /// 0x08d8fadc
    bool GetVal(FStringA& a1) const;
    /// 0x08d8fb3e
    UNK_RET GetValUtf8(char const*& a1, unsigned int& a2) const;
    /// 0x08d8fb4a
    UNK_RET NameMatches(char const* a1, unsigned int const& a2) const;
    /// 0x08d8fbae
    UNK_RET ValMatches(char const* a1, unsigned int const& a2) const;
    /// 0x08d90b58 Lexical_Cast<bool>
    /// 0x08d90ddc Lexical_Cast<double>
    /// 0x08d91060 Lexical_Cast<float>
    /// 0x08d912e4 Lexical_Cast<int>
    /// 0x08d91568 Lexical_Cast<short>
    /// 0x08d917ec Lexical_Cast<unsigned int>
    template <class T0>
    bool Lexical_Cast(T0& a1, char const* a2, unsigned int a3) const;
};
