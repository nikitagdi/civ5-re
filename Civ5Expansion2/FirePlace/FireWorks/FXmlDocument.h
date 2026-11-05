/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"
#include <ostream>
#include <string>

class FXmlDocument;
class FXmlElement;

/// size ?
/// vtable 0x0a3059e0
class FXmlDocument {
public:
    /// 0x08d8e292
    FXmlDocument();
    /// 0x08d8e328
    FXmlDocument(FXmlDocument const& a1);
    /// 0x08d8e3e6
    /// deleting 0x08d8e3be
    virtual ~FXmlDocument();
    /// 0x08d8e434
    UNK_RET Clear();
    /// 0x08d8e46e
    bool Load(char const* a1);
    /// 0x08d8e58a
    UNK_RET Parse(char* a1);
    /// 0x08d8e60e
    UNK_RET Load(wchar_t const* a1);
    /// 0x08d8e7a4
    UNK_RET GetElementCount() const;
    /// 0x08d8e7fe
    UNK_RET GetRootElement(FXmlElement& a1) const;
    /// 0x08d8e88c
    UNK_RET GetFirstChild(FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08d8e8e0
    UNK_RET LocateElement(FXmlElement& a1, char const* a2);
    /// 0x08d8eb46
    UNK_RET Save(char const* a1);
    /// 0x08d8ed4c
    UNK_RET Print(FStringA& a1);
    /// 0x08d8f09a
    UNK_RET Print(std::basic_ostream<char, std::char_traits<char> >& a1);
    /// 0x08d8f202
    UNK_RET LinkEndChild(FXmlElement& a1);
    /// 0x08d8f2b6
    UNK_RET LinkDeclaration(FXmlElement& a1, char const* a2, char const* a3, char const* a4);
};

/// 0x08d8e72a
UNK_RET GetTextLocation(char const* a1, char const* a2, int& a3, int& a4);
