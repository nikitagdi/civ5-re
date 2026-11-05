/// origin guess
#pragma once
#include "FirePlace/FireWorks/FXmlNVPair.h"
#include <rapidxml.h>

class FXmlAttribute;
class FXmlElement;

/// size ?
/// vtable 0x0a3059f0
class FXmlAttribute : public FXmlNVPair {
public:
    /// 0x08d8fc12
    FXmlAttribute();
    /// 0x08d8feb8
    virtual UNK_RET InternalGetName(char const*& a1, unsigned int& a2) const;
    /// 0x08d8ff02
    virtual UNK_RET InternalGetValue(char const*& a1, unsigned int& a2) const;
    /// 0x08d8fc4a
    /// deleting 0x08d8fc36
    virtual ~FXmlAttribute();
    /// 0x08d8fc4c
    UNK_RET GetElement(FXmlElement& a1) const;
    /// 0x08d8fc6c
    UNK_RET Assign(rapidxml::xml_attribute<char>* a1);
    /// 0x08d8fc78
    UNK_RET GetNextAttribute(FXmlAttribute& a1) const;
    /// 0x08d8fc9e
    UNK_RET GetNextAttribute(FXmlAttribute& a1, char const* a2, unsigned int a3) const;
    /// 0x08d8fcee
    UNK_RET GetPreviousAttribute(FXmlAttribute& a1) const;
    /// 0x08d8fd14
    UNK_RET GetPreviousAttribute(FXmlAttribute& a1, char const* a2, unsigned int a3) const;
    /// 0x08d8fd64
    UNK_RET SetName(char const* a1);
    /// 0x08d8fdc0
    UNK_RET SetName(char const* a1, unsigned int a2);
    /// 0x08d8fe0e
    UNK_RET SetValue(char const* a1);
    /// 0x08d8fe6a
    UNK_RET SetValue(char const* a1, unsigned int a2);
    /// 0x08d8ff4e
    UNK_RET InternalSetValue(char const* a1, unsigned int a2);
};
