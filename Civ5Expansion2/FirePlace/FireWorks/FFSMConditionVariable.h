/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FContext;
class FXmlElement;

/// size 0x30
/// vtable 0x0a305d20
class FFSMConditionVariable : public FIFSMCondition {
public:
    void* m_pEntry;       ///< +0x18
    float m_fValue;       ///< +0x1c
    char m_szName[0x10];  ///< +0x20
    /// absent
    virtual ~FFSMConditionVariable();
    /// 0x08dad374
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dad408
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08cee3ae
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cee3ce
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dad2ce
    void* GetEntry(FContext& a1) const;
    /// 0x08dad32a
    void set_name(char const* a1);
};
