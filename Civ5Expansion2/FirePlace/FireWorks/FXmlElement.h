/// origin guess
#pragma once
#include "FirePlace/FireWorks/FCompareValues.h"
#include "FirePlace/FireWorks/FIFiniteStateMachineInstance.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlNVPair.h"
#include <memory>
#include <rapidxml.h>
#include <string>

class FXmlAttribute;
class FXmlElement;

/// size ?
/// vtable 0x0a305a10
class FXmlElement : public FXmlNVPair {
public:
    /// 0x08d8ff9c
    FXmlElement();
    /// 0x08d8ffc0
    FXmlElement(FXmlElement const& a1);
    /// 0x08d906e0
    virtual UNK_RET InternalGetName(char const*& a1, unsigned int& a2) const;
    /// 0x08d9072a
    virtual UNK_RET InternalGetValue(char const*& a1, unsigned int& a2) const;
    /// 0x08d8fffa
    /// deleting 0x08d8ffe6
    virtual ~FXmlElement();
    /// 0x0869904a GetFirstAttributeVal<FStringA>
    /// 0x08c6ecf8 GetFirstAttributeVal<bool>
    /// 0x0866b702 GetFirstAttributeVal<float>
    /// 0x088ef9ca GetFirstAttributeVal<int>
    /// 0x089d0172 GetFirstAttributeVal<short>
    /// 0x08c6ed7c GetFirstAttributeVal<std::string>
    /// 0x0866b786 GetFirstAttributeVal<unsigned int>
    template <class T0>
    bool GetFirstAttributeVal(T0& a1, char const* a2, unsigned int a3) const;
    /// 0x08a2a698 SetChildValue<char const*>
    /// 0x08a29c9c SetChildValue<float>
    /// 0x08a29950 SetChildValue<int>
    /// 0x08a2a1a2 SetChildValue<unsigned int>
    template <class T0>
    bool SetChildValue(char const* a1, unsigned int a2, T0 a3);
    /// 0x08cba57e SetAttributeVal<char const>
    template <class T0>
    bool SetAttributeVal(char const* a1, unsigned int a2, T0* a3);
    /// 0x08db11da SetAttributeVal<FCompareValues::Method>
    /// 0x08db1986 SetAttributeVal<FIFiniteStateMachineInstance::path_option>
    /// 0x08ce4bb0 SetAttributeVal<FStringA>
    /// 0x08db1528 SetAttributeVal<bool>
    /// 0x08ce26e6 SetAttributeVal<float>
    /// 0x08cba91c SetAttributeVal<int>
    /// 0x08cf15cc SetAttributeVal<unsigned short>
    template <class T0>
    bool SetAttributeVal(char const* a1, unsigned int a2, T0 a3);
    /// 0x08d8e82e
    UNK_RET GetChildCount(int a1) const;
    /// 0x08d8e85c
    bool GetNextSibling(FXmlElement& a1) const;
    /// 0x08d8e880
    UNK_RET Assign(rapidxml::xml_node<char>* a1);
    /// 0x08d8fffc
    UNK_RET Empty();
    /// 0x08d90008
    unsigned int GetAttributeCount() const;
    /// 0x08d90034
    UNK_RET GetFirstAttribute(FXmlAttribute& a1) const;
    /// 0x08d90058
    bool GetFirstAttribute(FXmlAttribute& a1, char const* a2, unsigned int a3) const;
    /// 0x08d900ac
    UNK_RET GetLastAttribute(FXmlAttribute& a1) const;
    /// 0x08d900d6
    UNK_RET GetLastAttribute(FXmlAttribute& a1, char const* a2, unsigned int a3) const;
    /// 0x08d9012a
    UNK_RET GetParent(FXmlElement& a1) const;
    /// 0x08d90196
    UNK_RET GetFirstChild(FXmlElement& a1) const;
    /// 0x08d901ba
    bool GetFirstChild(FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08d9020e
    UNK_RET GetLastChild(FXmlElement& a1) const;
    /// 0x08d90232
    UNK_RET GetLastChild(FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08d90286
    bool GetNextSibling(FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08d902da
    UNK_RET GetPreviousSibling(FXmlElement& a1) const;
    /// 0x08d902fe
    UNK_RET GetPreviousSibling(FXmlElement& a1, char const* a2, unsigned int a3) const;
    /// 0x08d90352
    UNK_RET SetName(char const* a1);
    /// 0x08d90394
    UNK_RET SetName(char const* a1, unsigned int a2);
    /// 0x08d903f8
    UNK_RET SetValue(char const* a1);
    /// 0x08d9043a
    UNK_RET SetValue(char const* a1, unsigned int a2);
    /// 0x08d9049e
    UNK_RET RemoveAttribute(FXmlAttribute& a1);
    /// 0x08d904e4
    UNK_RET RemoveFirstAttribute(char const* a1, unsigned int a2);
    /// 0x08d90544
    UNK_RET LinkEndChild(FXmlElement& a1);
    /// 0x08d905fe
    UNK_RET RemoveChild(FXmlElement const& a1);
    /// 0x08d90644
    UNK_RET LinkEndAttribute(FXmlAttribute& a1);
};
