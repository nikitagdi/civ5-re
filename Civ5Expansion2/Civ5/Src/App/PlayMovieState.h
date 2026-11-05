/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"
#include <Windows.h>

class IBaseFilter;
class IGraphBuilder;
class IPin;
class IVMRWindowlessControl;
class LinearEventDispatcher;
class LocalMachineContainer;
class MatchPinDirectionAndConnection;
class NullEventClass;
class _PinDirection;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fd4a0
class PlayMovieState : public Civ5GameState {
public:
    /// 0x0884b474
    PlayMovieState();
    /// 0x0884b512
    /// deleting 0x0884b4ea
    virtual ~PlayMovieState();
    /// 0x0884be58
    virtual UNK_RET Begin();
    /// 0x0884c556
    virtual UNK_RET End();
    /// 0x0884c65c
    virtual UNK_RET Update(float a1);
    /// 0x0884c6a0
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0884bdac
    virtual UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x0884b54a
    UNK_RET IsDone();
    /// 0x0884b606
    UNK_RET RenderStreams(IBaseFilter* a1, IBaseFilter* a2);
    /// 0x0884bcb8
    UNK_RET OnSizeChanged(HWND__* a1);
    /// 0x0884bd06
    UNK_RET OnDisplayChanged(HWND__* a1);
    /// 0x0884bd3a
    UNK_RET OnPaint(HWND__* a1);
    /// 0x0884bd90
    UNK_RET OnUserRequestClose(EventTemplate<Event_Int2Type<104>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0884c6a2
    UNK_RET Mute(bool a1);
};

/// 0x0884ba6e
UNK_RET AddFilterByCLSID(IGraphBuilder* a1, _GUID const& a2, IBaseFilter** a3, wchar_t const* a4);
/// 0x0884bb2f
UNK_RET InitWindowlessVMR(IBaseFilter* a1, HWND__* a2, IVMRWindowlessControl** a3);
/// 0x0884bc24
UNK_RET RemoveUnconnectedRenderer(IGraphBuilder* a1, IBaseFilter* a2, int* a3);
/// 0x0884c711
UNK_RET IsPinConnected(IPin* a1, int* a2);
/// 0x0884c78f
UNK_RET FindConnectedPin(IBaseFilter* a1, _PinDirection a2, IPin** a3);
/// 0x0884c7d4
UNK_RET IsPinDirection(IPin* a1, _PinDirection a2, int* a3);
/// 0x0884c824 FindMatchingPin<MatchPinDirectionAndConnection>
template <class T0>
long FindMatchingPin(IBaseFilter* a1, T0 a2, IPin** a3);
