/// origin guess
#pragma once

class FCamera;
class FGXVector3;

/// size ?
class FGameListener {
public:
    enum EListenerType : int;
    /// 0x08cc0098
    FGameListener();
    /// 0x08cc00a4
    ~FGameListener();
    /// 0x08cc0034
    UNK_RET Init();
    /// 0x08cc008c
    UNK_RET Shutdown();
    /// 0x08cc00a6
    UNK_RET Update();
    /// 0x08cc0104
    UNK_RET UpdateListener_FirstPersonCamera();
    /// 0x08cc0136
    UNK_RET UpdateListener_WorldCamera();
    /// 0x08cc01ce
    UNK_RET UpdateListener_CustomCamera();
    /// 0x08cc01d0
    UNK_RET DoUpdateListenerFromCamera(FCamera* a1);
    /// 0x08cc027c
    UNK_RET GetListenerPosition(FGXVector3& a1) const;
    /// 0x08cc02c6
    UNK_RET GetListenerOrientation(FGXVector3& a1, FGXVector3& a2) const;
    /// 0x08cc031a
    UNK_RET UpdateDebugListenerEntity();
    /// 0x08cc031c
    UNK_RET SetCustomPositionOrientation(FGXVector3 const& a1, FGXVector3 const& a2, FGXVector3 const& a3);
};
