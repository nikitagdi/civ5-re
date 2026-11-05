/// origin guess
#pragma once

class FGXVector3;
class FListener;
class FListenerInit;
class FListenerProp;
class FTransformNode;
template <class T0> class FTPointer;

/// size ?
class FListener {
public:
    /// 0x08d2eb24
    FListener();
    /// 0x08d2ebbe
    ~FListener();
    /// 0x08d2ebb8
    UNK_RET Clear();
    /// 0x08d2ec36
    UNK_RET Term();
    /// 0x08d2ec62
    UNK_RET Init(FListenerInit const& a1);
    /// 0x08d2ed14
    UNK_RET Set3DObject(FTPointer<FTransformNode> a1);
    /// 0x08d2ed64
    UNK_RET Get3DObject();
    /// 0x08d2ed7c
    UNK_RET Update();
    /// 0x08d2ede8
    UNK_RET SetOrientation(FGXVector3 const& a1, FGXVector3 const& a2);
    /// 0x08d2ee50
    UNK_RET SetProperties(FListenerProp const& a1);
    /// 0x08d2eec4
    UNK_RET SetPosition(FGXVector3 const& a1);
    /// 0x08d2ef30
    UNK_RET GetProperties(FListenerProp& a1) const;
    /// 0x08d2ef44
    UNK_RET GetPosition(FGXVector3& a1) const;
    /// 0x08d2ef5c
    UNK_RET GetOrientation(FGXVector3& a1, FGXVector3& a2) const;
    /// 0x08d2ef8a
    UNK_RET CreateObject();
    /// 0x08d2f026
    UNK_RET DestroyObject(FListener* a1);
};
