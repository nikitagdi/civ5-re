/// origin guess
#pragma once
#include "FirePlace/FireWorks/FReferenceObject.h"

class FDDTRoot;
class FXmlDocument;
class ResourceType;

/// size ?
/// vtable 0x0a302730
class FResourceLoadHelper : public FReferenceObject {
public:
    /// 0x08ce7d68
    FResourceLoadHelper();
    /// 0x08ce7d96
    virtual ~FResourceLoadHelper();
    /// 0x08ce7daa
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08ce7d8c
    virtual UNK_RET CreateResource(char const* a1, ResourceType a2);
    /// 0x0d27b400
    static UNK_DATA ms_kRTTI[0x8];
};

/// 0x08ce7dc0
UNK_RET CloneDDTRoot(FDDTRoot const& a1, FDDTRoot** a2);
/// 0x08ce7f4a
UNK_RET DestroyDDTRoot(FDDTRoot** a1);
/// 0x08ce7f7c
UNK_RET CreateDDTRootFromXmlDocument(FXmlDocument const& a1, FDDTRoot** a2);
/// 0x08ce8db2
UNK_RET CreateDDTRootFromXmlFile(char const* a1, FDDTRoot** a2);
/// 0x08ce8e2c
UNK_RET GetSequenceType(char const* a1);
/// 0x08ce8e83
UNK_RET GetSequenceTypeName(int a1);
/// 0x08ce8eb2
UNK_RET GetSelectorItemType(char const* a1);
/// 0x08ce8f09
UNK_RET GetSelectorItemTypeName(int a1);
