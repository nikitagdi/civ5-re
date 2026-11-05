/// origin guess
#pragma once
#include "FirePlace/FireWorks/FSoundBase.h"
#include "FirePlace/FireWorks/FString.h"

class F3DSoundScriptData;
class FGXVector3;
class FSoundBase;
class FSoundInit;
enum EAudioPositionType : int;

/// size ?
/// vtable 0x0a3037a0
class FSound3D : public FSoundBase {
public:
    /// 0x08d31d7c
    FSound3D();
    /// 0x08d326ca
    virtual UNK_RET ApplyScript();
    /// 0x08d31f7c
    virtual UNK_RET Clear();
    /// 0x08d33028
    virtual UNK_RET ComputePriorityScoreVersus(FSoundBase const* a1) const;
    /// 0x08d321a8
    virtual UNK_RET Destroy();
    /// 0x08d3225e
    virtual UNK_RET FinishLoading();
    /// 0x08d3216c
    virtual UNK_RET Init(FSoundInit const& a1);
    /// 0x08d3269c
    virtual UNK_RET InitSoundType();
    /// 0x08d32084
    virtual UNK_RET InsertSoundIntoAudioManager();
    /// 0x08d321de
    virtual UNK_RET Load(FStringA* a1);
    /// 0x08d33138
    virtual UNK_RET NotifyAudioManagerSoundUnloaded();
    /// 0x08d31f4c
    virtual UNK_RET RemoveSoundWithLowestPriority();
    /// 0x08d320b8
    virtual UNK_RET Update(float a1);
    /// 0x08d32a2e
    virtual UNK_RET ApplyScriptUpdate(float a1);
    /// 0x08d31ebe
    /// deleting 0x08d31e96
    virtual ~FSound3D();
    /// 0x08d32180
    virtual UNK_RET Term();
    /// 0x08d3222a
    virtual UNK_RET DoLoad(FStringA* a1);
    /// 0x08d32142
    UNK_RET UpdatePosition();
    /// 0x08d3232a
    UNK_RET SetPosition(FGXVector3 const& a1);
    /// 0x08d323ea
    UNK_RET SetVelocityMagnitude(float a1);
    /// 0x08d32412
    UNK_RET SetDistances(float a1, float a2);
    /// 0x08d324c2
    UNK_RET SetCone(int a1, int a2, float a3);
    /// 0x08d3259a
    UNK_RET SetOrientation(FGXVector3 const& a1, FGXVector3 const& a2);
    /// 0x08d326b8
    UNK_RET SetScript(F3DSoundScriptData const* a1);
    /// 0x08d32712
    UNK_RET ApplyScriptPosition();
    /// 0x08d32972
    UNK_RET ApplyScriptDistances();
    /// 0x08d329c6
    UNK_RET ApplyScriptChannelLevels();
    /// 0x08d32ad0
    UNK_RET UpdateScriptPosition(float a1);
    /// 0x08d32c7a
    UNK_RET CalculatePosition(EAudioPositionType a1, float a2);
    /// 0x08d32e74
    UNK_RET GetPosition(FGXVector3& a1) const;
    /// 0x08d32e98
    UNK_RET GetVelocityMagnitude(float& a1) const;
    /// 0x08d32eb2
    UNK_RET UpdateVelocity(float a1);
    /// 0x08d32f8a
    UNK_RET GetDistances(float& a1, float& a2) const;
    /// 0x08d32fb4
    UNK_RET GetCone(int& a1, int& a2, float& a3) const;
    /// 0x08d32fea
    UNK_RET GetOrientation(FGXVector3& a1, FGXVector3& a2) const;
    /// 0x0ff85ea0
    static UNK_DATA s_ObjPool[0x18];
};
