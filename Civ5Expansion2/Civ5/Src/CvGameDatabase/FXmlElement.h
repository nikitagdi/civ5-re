/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/FXmlNVPair.h"
#include <memory>
#include <string>

namespace Database {
class FXmlAttribute;
class FXmlElement;
}
namespace Database { namespace rapidxml {
template <class T0> class xml_node;
}}

namespace Database {
/// size ?
/// vtable 0x0a300bb0
class FXmlElement : public Database::FXmlNVPair {
public:
    /// 0x08c57b18
    FXmlElement();
    /// 0x08c57b3c
    FXmlElement(Database::FXmlElement const& a1);
    /// 0x08c57eec
    virtual UNK_RET InternalGetName(char const*& a1, unsigned int& a2) const;
    /// 0x08c57f36
    virtual UNK_RET InternalGetValue(char const*& a1, unsigned int& a2) const;
    /// 0x08c57b76
    /// deleting 0x08c57b62
    virtual ~FXmlElement();
    /// 0x08ba2f02 GetFirstAttributeVal<bool>
    /// 0x08ba2e7e GetFirstAttributeVal<std::string>
    template <class T0>
    bool GetFirstAttributeVal(T0& a1, char const* a2, unsigned int a3) const;
    /// 0x08c56dee
    UNK_RET GetChildCount(int a1) const;
    /// 0x08c56e1e
    UNK_RET Assign(Database::rapidxml::xml_node<char>* a1);
    /// 0x08c57b78
    UNK_RET GetAttributeCount() const;
    /// 0x08c57baa
    UNK_RET GetFirstAttribute(Database::FXmlAttribute& a1) const;
    /// 0x08c57bce
    UNK_RET GetFirstAttribute(Database::FXmlAttribute& a1, char const* a2, unsigned int a3) const;
    /// 0x08c57c22
    UNK_RET GetLastAttribute(Database::FXmlAttribute& a1) const;
    /// 0x08c57c4c
    UNK_RET GetLastAttribute(Database::FXmlAttribute& a1, char const* a2, unsigned int a3) const;
    /// 0x08c57ca0
    UNK_RET GetParent(Database::FXmlElement& a1) const;
    /// 0x08c57d0c
    UNK_RET GetFirstChild(Database::FXmlElement& a1) const;
    /// 0x08c57d30
    UNK_RET GetFirstChild(Database::FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08c57d84
    UNK_RET GetLastChild(Database::FXmlElement& a1) const;
    /// 0x08c57da8
    UNK_RET GetLastChild(Database::FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08c57dfc
    UNK_RET GetNextSibling(Database::FXmlElement& a1) const;
    /// 0x08c57e20
    UNK_RET GetNextSibling(Database::FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08c57e74
    UNK_RET GetPreviousSibling(Database::FXmlElement& a1) const;
    /// 0x08c57e98
    UNK_RET GetPreviousSibling(Database::FXmlElement& a1, char const* a2, unsigned int a3) const;
};
}
