/// origin binary
#include "Civ5/Src/App/UI/UIManager.h"
#include "Civ5/Src/App/DragCameraData.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"

static const unsigned int kLocalMachineStride = 0x600;
// Slot of g_EventSystemLocalMachine: dispatcher, its context at +4.
static const unsigned int kDragCameraEventSlot = 0x328;

// Bits of UIManager::m_uMouseDragFlags.
static const unsigned int kDragLeftButtonDown = 0x4;
static const unsigned int kDragCaptured = 0x8;
static const unsigned int kDragCameraEvent = 0x40;

// Press-to-move distance in pixels before a drag starts.
static const float kDragStartPixels = 4.0f;

typedef void (*PfnDispatchEvent)(const void* pEvent, void* pContext);

/// 0x086cddce
bool UIManager::ProcessCameraDrag(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, long lParam) {
    // An event system is an array of instances; its first int selects the current one.
    const auto dispatchDrag = [this]() {
        DragCameraData kDrag;
        kDrag.m_vTo.x = m_fMouseNormX;
        kDrag.m_vTo.y = m_fMouseNormY;
        kDrag.m_vFrom.x = m_fMouseNormX - m_fMouseNormDeltaX;
        kDrag.m_vFrom.y = m_fMouseNormY - m_fMouseNormDeltaY;
        UNK_DATA* pSystem = g_EventSystemLocalMachine;
        const unsigned int uOffset = *reinterpret_cast<int*>(pSystem) * kLocalMachineStride;
        PfnDispatchEvent pfnDispatch = *reinterpret_cast<PfnDispatchEvent*>(pSystem + uOffset + kDragCameraEventSlot);
        void* pContext = *reinterpret_cast<void**>(pSystem + uOffset + kDragCameraEventSlot + 4);
        pfnDispatch(&kDrag, pContext);
    };
    if (uMsg == WM_LBUTTONUP) {
        const unsigned int uFlags = m_uMouseDragFlags;
        m_uMouseDragFlags = uFlags & ~kDragLeftButtonDown;
        if (!(uFlags & kDragCaptured)) {
            return false;
        }
        ReleaseCapture();
        m_uMouseDragFlags &= ~kDragCaptured;
        return true;
    }
    if (uMsg != WM_MOUSEMOVE) {
        return false;
    }
    if (m_uMouseDragFlags & kDragCaptured) {
        m_uMouseDragFlags |= kDragCameraEvent;
        dispatchDrag();
        return true;
    }
    const float fDX = fabsf(m_fMouseDownX - static_cast<float>(static_cast<short>(lParam)));
    const float fDY = fabsf(m_fMouseDownY - static_cast<float>(lParam >> 16));
    if (!(fDX > kDragStartPixels || fDY > kDragStartPixels)) {
        return false;
    }
    m_uMouseDragFlags |= kDragCaptured;
    SetCapture(hWnd);
    m_uMouseDragFlags |= kDragCameraEvent;
    dispatchDrag();
    return true;
}
