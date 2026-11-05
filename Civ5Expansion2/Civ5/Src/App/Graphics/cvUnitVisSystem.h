/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/FrameGlobalsData.h"

class cvUnitVis;
class cvUnitVisSystem;
struct cvUnitVisDataDef;
struct cvUnitVis_Init;
struct cvUnitVis_SetTransformation;
struct cvUnitVis_EnableUnitMember;
struct cvUnitVis_SetTargetAnimation;
struct cvUnitVis_State;
struct cvUnitVis_DamageState;
struct cvUnitVis_BlendTime;
struct cvUnitSetActionResultCheck;
struct cvUnitSetVarListValue;
struct cvUnitOverrideFSMState;
struct cvUnitSetFSMDebugState;
struct cvUnitStateGraphInfo;
struct cvUnitCurrentStateInfo;
struct cvUnitCurrentDebugStateInfo;
struct MapGlobalsData;
struct GlobalUnitScaleData;
struct AssetSwappedData;
struct UnitDataEditedData;
struct UnitDataRequestedData;
struct UnitDebugFSMData;

/// size ?
struct AnimationSamplingChangedData {
    enum AnimationSampleTypes : int;
};

/// size 0x148
struct cvUnitVis_DamageState {
    unsigned char m_aucFigureState[0x40];  ///< +0x0
    float m_afReviveTag[0x3f];             ///< +0x40
    unsigned char m_ucNumFigures;          ///< +0x13c
    unsigned int m_uiReviveMaskLo;         ///< +0x140
    unsigned int m_uiReviveMaskHi;         ///< +0x144
};

/// size ?
struct cvUnitSetVarListValue {
    enum VARIABLE_TYPE : int;
};

template <int N>
struct Event_Int2Type;
struct NullEventClass;
struct LinearEventDispatcher;
struct LocalMachineContainer;
struct NonSerialContainer;
struct SerialContainer;
struct cvEventSystemLocalMachineAccessor;
struct cvEventSystemNonSerialAccessor;
struct cvEventSystemSerialAccessor;
struct cvSubscriptionValidator;
template <class TAccessor, class TValidator>
struct BaseEvent;
template <class TAccessor, class TValidator>
struct cvSerialBaseEvent;
template <class TId, class TData, class TDispatcher, class TContainer, class TPre, class TBase, class TPost>
class EventTemplate;
template <class TId, class TData, class TDispatcher, class TContainer, class TPre, class TBase, class TPost>
class GameCoreEventTemplate;

template <int Id, class TData>
using LocalMachineEvent = EventTemplate<Event_Int2Type<Id>, TData, LinearEventDispatcher, LocalMachineContainer, NullEventClass,
                                        BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>;
template <int Id, class TData>
using NonSerialEvent = EventTemplate<Event_Int2Type<Id>, TData, LinearEventDispatcher, NonSerialContainer, NullEventClass,
                                     BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>;
template <int Id, class TData>
using SerialEvent = GameCoreEventTemplate<Event_Int2Type<Id>, TData, LinearEventDispatcher, SerialContainer, NullEventClass,
                                          cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>;

typedef cvGeneralRefHandle<cvUnitVisSystem, unsigned int, cvUnitVisDataDef, cvUnitVisSystemAccessor> cvUnitVisHandle;

/// 0x08935b6a
UNK_RET Fun_08935b6a(UNK_PARAMS);

/// 0x08935bf6
UNK_RET Fun_08935bf6(UNK_PARAMS);

/// 0x08935c96
UNK_RET Fun_08935c96(UNK_PARAMS);

/// 0x08935d36
UNK_RET Fun_08935d36(UNK_PARAMS);

/// 0x08935dfa
UNK_RET Fun_08935dfa(UNK_PARAMS);

/// 0x08936be2
UNK_RET Fun_08936be2(UNK_PARAMS);

/// 0x08937312
unsigned int Fun_08937312(UNK_PARAMS);

/// 0x0893737e
UNK_RET Fun_0893737e(UNK_PARAMS);

/// 0x0893b5ca
UNK_RET Fun_0893b5ca(UNK_PARAMS);

/// 0x0893b636
UNK_RET Fun_0893b636(UNK_PARAMS);

/// 0x0893b7ac
UNK_RET Fun_0893b7ac(UNK_PARAMS);

/// 0x0893b7d8
UNK_RET Fun_0893b7d8(UNK_PARAMS);

/// 0x0893b80e
UNK_RET Fun_0893b80e(UNK_PARAMS);

/// 0x0893b83a
UNK_RET Fun_0893b83a(UNK_PARAMS);

/// 0x0893bbc8
UNK_RET Fun_0893bbc8(UNK_PARAMS);

/// size 0x143d0
class cvUnitVisSystem {
public:
    /// size 0x14
    struct VisSlotArray {
        int field_0;           ///< +0x0
        int field_4;           ///< +0x4
        char* field_8;         ///< +0x8
        unsigned int field_c;  ///< +0xc
        int field_10;          ///< +0x10
    };

    /// 0x0893170c
    cvUnitVisSystem();

    /// 0x08931b0c
    ~cvUnitVisSystem();

    /// 0x08931c04
    void Startup(unsigned int a1);

    /// 0x08931f24
    void SubscribeToEvents();

    /// 0x089320ea
    void LoadUnitLibraries();

    /// 0x089321b2
    void Shutdown();

    /// 0x089322f0
    void UnSubscribeToEvents();

    /// 0x089324b6
    void StartupGame();

    /// 0x089326c4
    void SetAnimationSamplingType(AnimationSamplingChangedData::AnimationSampleTypes eType);

    /// 0x089326e6
    void UpdateUnitText();

    /// 0x089327c2
    void SetUnitCount(unsigned int a1, unsigned int a2);

    /// 0x08932892
    void ShutdownGame();

    /// 0x089328d6
    void DestroyVises();

    /// 0x089329c2
    void OnFrameGlobalsUpdated(LocalMachineEvent<34, FrameGlobalsData>& a1);

    /// 0x08932a96
    void OnMapGlobalsChanged(LocalMachineEvent<37, MapGlobalsData>& a1);

    /// 0x08932acc
    void OnAnimationSamplingChanged(NonSerialEvent<6, AnimationSamplingChangedData>& a1);

    /// 0x08932af0
    void OnTestAnimations(SerialEvent<2, NullEventClass>& a1);

    /// 0x08932bf6
    void OnToggleDisplayUnits(NonSerialEvent<13, NullEventClass>& a1);

    /// 0x08932c02
    void OnDontRecordCommandStreams(NonSerialEvent<7, NullEventClass>& a1);

    /// 0x08932c04
    void OnRecordCommandStreams(NonSerialEvent<8, NullEventClass>& a1);

    /// 0x08932c06
    void OnGlobalUnitScale(NonSerialEvent<14, GlobalUnitScaleData>& a1);

    /// 0x08932c56
    void OnLibrarySwap(NonSerialEvent<16, NullEventClass>& a1);

    /// 0x08932dda
    void OnAssetSwap(LocalMachineEvent<128, AssetSwappedData>& a1);

    /// 0x08932ef2
    void OnToggleDebugInfoDisplay(NonSerialEvent<19, NullEventClass>& a1);

    /// 0x08932f2e
    void OnUnitDataEdited(SerialEvent<86, UnitDataEditedData>& a1);

    /// 0x08933044
    void OnUnitDataRequested(SerialEvent<87, UnitDataRequestedData>& a1);

    /// 0x08933090
    void OnUnitDebugFSM(SerialEvent<88, UnitDebugFSMData>& a1);

    /// 0x089330d8
    void UpdateGlobals(int a1);

    /// 0x0893330c
    void InitializeVisualizer(cvUnitVis& a1, cvUnitVisHandle& a2);

    /// 0x089337b0
    void SetVisualizerTranslation(cvUnitVis& a1, const cvUnitVis_SetTransformation& a2);

    /// 0x08933846
    void SetVisualizerVarListValue(cvUnitVis& a1, unsigned int a2, cvUnitSetVarListValue::VARIABLE_TYPE a3, float a4);

    /// 0x089338da
    void ReleaseVisualizersOfType(int a1);

    /// 0x08933954
    void CreateVisualizersOfType(int a1);

    /// 0x08933a72
    void UpdateAnimationText();

    /// 0x08933ac2
    float GetGlobalUnitScale() const;

    /// 0x08933b04
    void UpdateUnitTextPerFrame();

    /// 0x08933b94
    void DestroyFromHandle(cvUnitVisHandle& h);

    /// 0x08933dea
    void SetVisualizerAnimation(cvUnitVis& a1);

    /// 0x08933fe2
    void Update(float fTime);

    /// 0x08934040
    void OnRenderView(unsigned int a1);

    /// 0x08934104
    void OnRenderViewZFail(unsigned int a1);

    /// 0x08934178
    void OnRenderShadow(unsigned int a1);

    /// 0x0893423c
    void OnRenderReflection(unsigned int a1);

    /// 0x08934300
    void StateSort();

    /// 0x0893432e
    void RenderInternal();

    /// 0x08934358
    void DrawUnits();

    /// 0x08934732
    void DrawUnitsMT();

    /// 0x089349e0
    UNK_RET GetInfoFromHandle(cvUnitVisHandle h, cvUnitStateGraphInfo& a2) const;

    /// 0x08934b1e
    UNK_RET GetInfoFromHandle(cvUnitVisHandle h, cvUnitCurrentStateInfo& a2) const;

    /// 0x08934bea
    UNK_RET GetInfoFromHandle(cvUnitVisHandle h, cvUnitCurrentDebugStateInfo& a2) const;

    /// 0x08934d50
    UNK_RET DestroyVis(cvUnitVisHandle h, const cvUnitVisHandle& a2);

    /// 0x08934e9a
    void InitHandle(cvUnitVisHandle& h, const cvUnitVis_Init& init);

    /// 0x08934f76
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitVis_Init& a2);

    /// 0x089350ce
    UNK_RET GetStateIDByName(cvUnitVisHandle h, const char* a2, unsigned int a3) const;

    /// 0x0893512c
    UNK_RET GetCurrentStateID(cvUnitVisHandle h, unsigned int a2) const;

    /// 0x08935180
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitVis_EnableUnitMember& a2);

    /// 0x0893522c
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitVis_SetTransformation& a2);

    /// 0x0893527a
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitVis_SetTargetAnimation& a2);

    /// 0x089352d2
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitSetVarListValue& a2);

    /// 0x08935336
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitSetActionResultCheck& a2);

    /// 0x089353be
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitOverrideFSMState& a2);

    /// 0x089353fc
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitVis_State& a2);

    /// 0x0893563e
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitVis_DamageState& a2);

    /// 0x08935846
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitVis_BlendTime& a2);

    /// 0x0893586e
    void UpdateFromHandle(cvUnitVisHandle h, const cvUnitSetFSMDebugState& a2);

    /// 0x089358ba
    bool HasAnimationStates(cvUnitVisHandle h) const;

    /// 0x08935906
    bool HasAnimation(cvUnitVisHandle h, unsigned int a2) const;

    /// 0x0c075090
    static UNK_DATA ms_kRandomPlacement[0x18];

    /// size ?
    /// vtable 0x0a2fddc4
    class UnitRenderJob : public FJob {
    public:
        /// 0x0893595a
        virtual void Execute(unsigned int a1);

        /// 0x0c0750a8
        static UNK_DATA sm_pThreadRenderer[4];
    };

    /// size ?
    /// vtable 0x0a2fddac
    class UnitRenderJob1 : public FJob {
    public:
        /// 0x08935adc
        virtual void Execute(unsigned int a1);
    };

    /// size ?
    /// vtable 0x0a2fdd94
    class UnitRenderJob2 : public FJob {
    public:
        /// 0x08935afa
        virtual void Execute(unsigned int a1);
    };

    /// size ?
    /// vtable 0x0a2fdd78
    class UnitRenderJob3 : public FJob {
    public:
        /// 0x08935b42
        virtual void Execute(unsigned int a1);
    };

private:
    int field_0;                                              ///< +0x0
    char unk_4[0xe4];                                         ///< +0x4
    int field_e8;                                             ///< +0xe8
    int field_ec;                                             ///< +0xec
    char unk_f0[0xc];                                         ///< +0xf0
    char unk_fc[0x10];                                        ///< +0xfc
    char unk_10c[0x214];                                      ///< +0x10c
    FrameGlobalsData m_kFrameGlobals;                         ///< +0x320
    int field_b6c;                                            ///< +0xb6c
    char unk_b70[0x80];                                       ///< +0xb70
    VisSlotArray field_bf0;                                   ///< +0xbf0
    char unk_c04[0xc];                                        ///< +0xc04
    VisSlotArray* field_c10;                                  ///< +0xc10
    char unk_c14[0xc];                                        ///< +0xc14
    char unk_c20[0x70];                                       ///< +0xc20
    char unk_c90[0x12e18];                                    ///< +0xc90
    float m_fTime;                                            ///< +0x13aa8
    float m_fDeltaTime;                                       ///< +0x13aac
    unsigned int m_uiGlobalUnitScale;                         ///< +0x13ab0
    float field_13ab4;                                        ///< +0x13ab4
    AnimationSamplingChangedData::AnimationSampleTypes m_eAnimationSamplingType;  ///< +0x13ab8
    bool m_bDisplayUnits;                                     ///< +0x13abc
    bool m_bDebugInfoDisplay;                                 ///< +0x13abd
    char unk_13abe[2];                                        ///< +0x13abe
    int field_13ac0;                                          ///< +0x13ac0
    char unk_13ac4[0x8d0];                                    ///< +0x13ac4
    unsigned int field_14394;                                 ///< +0x14394
    unsigned int field_14398;                                 ///< +0x14398
    char unk_1439c[0xc];                                      ///< +0x1439c
    int field_143a8;                                          ///< +0x143a8
    int field_143ac;                                          ///< +0x143ac
    char unk_143b0[0xc];                                      ///< +0x143b0
    int field_143bc;                                          ///< +0x143bc
    int field_143c0;                                          ///< +0x143c0
    int field_143c4;                                          ///< +0x143c4
    char unk_143c8[8];                                        ///< +0x143c8
};

/// 0x0c060cc0
extern "C" cvUnitVisSystem g_cvUnitVisSystem;
