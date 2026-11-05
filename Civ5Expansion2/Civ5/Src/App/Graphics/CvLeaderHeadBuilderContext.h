/// origin guess
#pragma once
#include "Civ5/Src/App/BuilderContext_Base.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include <d3d9.h>
#include <granny.h>
#include <utility>

class CvLeaderHeadModel;
class CvLeaderHeadScene;
class CvLeaderHeadTextureManager;
class DebugHexSpaceStringData;
class DebugStringClearData;
class DebugStringToggleData;
class DebugWorldSpaceStringData;
class FGXVector2;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
class FGXModelNode;
class FGXSurface;
}

/// size ?
class CvLeaderHeadBuilderContext {
public:
    /// 0x08b7a730
    CvLeaderHeadBuilderContext();
    /// 0x08b7a806
    ~CvLeaderHeadBuilderContext();
    /// 0x08b7a9ea
    UNK_RET Init(CvLeaderHeadTextureManager* a1, CvLeaderHeadModel* a2, CvLeaderHeadScene* a3, char const* a4, FFastVector<std::pair<unsigned char*, unsigned int>, true, 46u, 0u, BaseVector<std::pair<unsigned char*, unsigned int>, true>::FDefaultFastVectorAllocator>* a5);
    /// 0x08b7aa2e
    UNK_RET InitFallback(CvLeaderHeadScene* a1, CvLeaderHeadModel* a2);
    /// 0x08b7aa6c
    UNK_RET GetVertexFormat(granny_material const* a1, granny_mesh const* a2);
    /// 0x08b7ad4e
    UNK_RET NotifyIBCreated(BuilderContext_Base::IBCreationInfo const& a1);
    /// 0x08b7ae0e
    UNK_RET GetSystemSideIndexBufferStorage(granny_mesh const* a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5);
    /// 0x08b7ae8e
    UNK_RET NotifyVBCreated(BuilderContext_Base::VBCreationInfo const& a1);
    /// 0x08b7af0a
    UNK_RET GetSystemSideVertexBufferStorage(granny_mesh const* a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5);
    /// 0x08b7af8e
    UNK_RET GetIBDataForMesh(unsigned int a1) const;
    /// 0x08b7aff2
    UNK_RET GetVBDataForMesh(unsigned int a1) const;
    /// 0x08b7b056
    UNK_RET BindTextures(granny_material const* a1, FireGrafix::FGXSurface* a2);
    /// 0x08b7b10a
    UNK_RET ExtractMutableCSetData(unsigned int a1, FireGrafix::FGXModelNode* a2, granny_material const* a3);
};

namespace DebugOverlay {
/// 0x08b7ba54
UNK_RET Startup();
/// 0x08b7bb98
UNK_RET OnHexDebugString(EventTemplate<Event_Int2Type<82>, DebugHexSpaceStringData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x08b7be80
UNK_RET OnWorldSpaceDebugString(EventTemplate<Event_Int2Type<83>, DebugWorldSpaceStringData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x08b7bf4e
UNK_RET OnDebugStringToggle(EventTemplate<Event_Int2Type<84>, DebugStringToggleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x08b7bf76
UNK_RET OnDebugStringClear(EventTemplate<Event_Int2Type<85>, DebugStringClearData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x08b7bfa2
UNK_RET Shutdown();
/// 0x08b7c06c
UNK_RET StartupGame();
/// 0x08b7c144
UNK_RET ShutdownGame();
/// 0x08b7c17c
UNK_RET BeginRender(unsigned int a1, FireGrafix::CCommandStream* a2);
/// 0x08b7c2fc
UNK_RET EndRender(FireGrafix::CCommandStream* a1);
/// 0x08b7c378
UNK_RET RenderText(FireGrafix::CCommandStream* a1, FGXVector2 a2, char* a3, unsigned long a4, float a5);
/// 0x08b7c8e8
UNK_RET Render(unsigned int a1, FireGrafix::CCommandStream* a2, D3DXMATRIX* a3);
/// 0x08b7cae6
UNK_RET DeleteDebugString(int a1, FGXVector2& a2);
}
