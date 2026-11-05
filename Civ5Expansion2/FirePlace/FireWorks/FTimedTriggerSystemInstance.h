/// origin binary
#pragma once
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FITimedTriggerSystemInstance.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>

class FClassId;
class FITimedTriggerSystem;
namespace FOutputLocation {
class FOutputItem;
enum ERootType : int;
}

/// size ?
/// vtable 0x0a3069e0
class FTimedTriggerSystemInstance : public FITimedTriggerSystemInstance {
public:
    /// 0x08dcc3d8
    FTimedTriggerSystemInstance(FITimedTriggerSystem& a1, float a2);
    /// 0x08dcc43c
    /// deleting 0x08dcc428
    virtual ~FTimedTriggerSystemInstance();
    /// 0x08dcc66a
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08dcc788
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dcc7a8
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dcc43e
    virtual UNK_RET get_system() const;
    /// 0x08dcc446
    virtual UNK_RET get_end_time() const;
    /// 0x08dcc44e
    virtual UNK_RET get_current_time() const;
    /// 0x08dcc480
    virtual UNK_RET process(float a1, float a2);
    /// 0x08dcc456
    virtual UNK_RET reset();
    /// 0x08dcc46a
    virtual UNK_RET loop();
};

/// 0x08dccd69
UNK_RET operator<<(FDataStream& a1, _GUID const& a2);
/// 0x08dccdcd
UNK_RET operator>>(FDataStream& a1, _GUID& a2);

namespace FGUID {
/// 0x08dcc818
UNK_RET Read(char const* a1, unsigned int a2, _GUID& a3);
/// 0x08dcca6e
UNK_RET Read(char const* a1, _GUID& a2);
/// 0x08dccab2
UNK_RET Read(FStringA const& a1, _GUID& a2);
/// 0x08dccae7
UNK_RET Write(_GUID const& a1, FStringA& a2, bool a3);
/// 0x08dccd1d
UNK_RET Clear(_GUID& a1);
/// 0x08dccd3d
UNK_RET IsEmpty(_GUID const& a1);
}

namespace FOutputLocation {
/// 0x08dcd1b4
UNK_RET Init();
/// 0x08dcd2ca
UNK_RET SetRoot(FOutputLocation::ERootType a1, wchar_t const* a2);
/// 0x08dcd4d2
UNK_RET Create();
/// 0x08dcd6d8
UNK_RET AddItem(FArray<FOutputLocation::FOutputItem, (eMPoolType)35>& a1, wchar_t const* a2, FOutputLocation::ERootType a3, bool a4, wchar_t const* a5);
/// 0x08dcd8d8
UNK_RET MoveRoot(FOutputLocation::ERootType a1, wchar_t const* a2);
}
