/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/FXmlNVPair.h"

namespace Database {
class FXmlAttribute;
class FXmlElement;
}
namespace Database { namespace rapidxml {
template <class T0> class xml_attribute;
}}

namespace Database {
/// size ?
/// vtable 0x0a300b90
class FXmlAttribute : public Database::FXmlNVPair {
public:
    /// 0x08c57930
    FXmlAttribute();
    /// 0x08c57a82
    virtual UNK_RET InternalGetName(char const*& a1, unsigned int& a2) const;
    /// 0x08c57acc
    virtual UNK_RET InternalGetValue(char const*& a1, unsigned int& a2) const;
    /// 0x08c57968
    /// deleting 0x08c57954
    virtual ~FXmlAttribute();
    /// 0x08c5796a
    UNK_RET GetElement(Database::FXmlElement& a1) const;
    /// 0x08c5798a
    UNK_RET Assign(Database::rapidxml::xml_attribute<char>* a1);
    /// 0x08c57996
    UNK_RET GetNextAttribute(Database::FXmlAttribute& a1) const;
    /// 0x08c579bc
    UNK_RET GetNextAttribute(Database::FXmlAttribute& a1, char const* a2, unsigned int a3) const;
    /// 0x08c57a0c
    UNK_RET GetPreviousAttribute(Database::FXmlAttribute& a1) const;
    /// 0x08c57a32
    UNK_RET GetPreviousAttribute(Database::FXmlAttribute& a1, char const* a2, unsigned int a3) const;
};
}

namespace Database { namespace rapidxml {
/// 0x08c57f81
UNK_RET parse_error_handler(char const* a1, void* a2);
}}
