/// origin guess
#pragma once
#include <memory>
#include <string>

namespace Database {
/// size ?
class FXmlNVPair {
public:
    /// 0x08c57090
    UNK_RET GetNameA(char const*& a1, unsigned int& a2) const;
    /// 0x08c570f0
    UNK_RET GetNameUtf8(char const*& a1, unsigned int& a2) const;
    /// 0x08c57128
    UNK_RET GetName(std::string& a1) const;
    /// 0x08c57182
    UNK_RET GetName(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& a1) const;
    /// 0x08c5729a
    UNK_RET GetVal(bool& a1) const;
    /// 0x08c57368
    UNK_RET GetVal(std::string& a1) const;
    /// 0x08c573c2
    UNK_RET GetVal(double& a1) const;
    /// 0x08c5745a
    UNK_RET GetVal(float& a1) const;
    /// 0x08c574f2
    UNK_RET GetVal(int& a1) const;
    /// 0x08c5758a
    UNK_RET GetVal(short& a1) const;
    /// 0x08c57622
    UNK_RET GetVal(unsigned int& a1) const;
    /// 0x08c576ba
    UNK_RET GetValA(char const*& a1, unsigned int& a2) const;
    /// 0x08c5771c
    UNK_RET GetVal(std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >& a1) const;
    /// 0x08c57834
    UNK_RET GetValUtf8(char const*& a1, unsigned int& a2) const;
    /// 0x08c5786c
    UNK_RET NameMatches(char const* a1, unsigned int const& a2) const;
    /// 0x08c578ce
    UNK_RET ValMatches(char const* a1, unsigned int const& a2) const;
    /// 0x08c5822c Lexical_Cast<bool>
    /// 0x08c58490 Lexical_Cast<double>
    /// 0x08c586f4 Lexical_Cast<float>
    /// 0x08c58958 Lexical_Cast<int>
    /// 0x08c58bbc Lexical_Cast<short>
    /// 0x08c58e20 Lexical_Cast<unsigned int>
    template <class T0>
    bool Lexical_Cast(T0& a1, std::string const& a2) const;
};
}
