/// origin guess
#pragma once
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>
#include <lua.h>

class AlphaAnim;
class BarControl;
class ButtonControl;
class CheckBoxControl;
class ColorBoxButtonControl;
class ColorBoxControl;
class ColorSet;
class ControlBase;
class ControlChildList;
class EditBoxControl;
class FGXFont;
class FGXVector4;
class FXmlElement;
class FlipAnim;
class GridButtonControl;
class GridControl;
class IUIObjectCache;
class ImageControl;
class LineControl;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class MeterControl;
class NullEventClass;
class PullDownControl;
class RadioButtonControl;
class ReplayMapControl;
class ScrollAnim;
class ScrollPanelControl;
class SlideAnim;
class SliderControl;
class StackControl;
class SystemUpdateUIData;
class TextButtonControl;
class TextControl;
class TextureBarControl;
class UIQuad;
class UIRenderBuffer;
class UIString1024;
class UIString128;
class UIString16384;
class UIString2048;
class UIString256;
class UIString32;
class UIString4096;
class UIString512;
class UIString64;
class UIString8192;
class UITextureInfo;
class WorldAnchorControl;
class WorldMouseOverData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
enum UIAABB : int;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
template <int T0> class VStruct;
}

/// size ?
/// vtable 0x0a2f99d0
class UIManager : public cvTunerListener::ITreeNode {
public:
    /// 0x086c971c
    UIManager();
    /// 0x086c9f32
    /// deleting 0x086c9f0a
    virtual ~UIManager();
    /// 0x086d0a72
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x086d09d8
    virtual UNK_RET GetNodes(FFastVector<cvTunerListener::ITreeNode*, false, 0u, 0u, BaseVector<cvTunerListener::ITreeNode*, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x086d09ac
    virtual UNK_RET GetText(cvTunerListener::ITreeNode::TextWriter& a1);
    /// 0x086d55ec AllocateControl<AlphaAnim>
    /// 0x086d5b34 AllocateControl<BarControl>
    /// 0x086d5654 AllocateControl<ButtonControl>
    /// 0x086d585c AllocateControl<CheckBoxControl>
    /// 0x086d5c04 AllocateControl<ColorBoxButtonControl>
    /// 0x086d51da AllocateControl<ColorBoxControl>
    /// 0x086d5cd4 AllocateControl<ControlBase>
    /// 0x08696626 AllocateControl<ControlChildList>
    /// 0x086d5da4 AllocateControl<EditBoxControl>
    /// 0x086d551c AllocateControl<FlipAnim>
    /// 0x086d58c4 AllocateControl<GridButtonControl>
    /// 0x086d59fc AllocateControl<GridControl>
    /// 0x086d5724 AllocateControl<ImageControl>
    /// 0x086d592c AllocateControl<LineControl>
    /// 0x086c5314 AllocateControl<LuaContext>
    /// 0x086d5acc AllocateControl<MeterControl>
    /// 0x086d5e0c AllocateControl<PullDownControl>
    /// 0x086d5c6c AllocateControl<RadioButtonControl>
    /// 0x086d5994 AllocateControl<ReplayMapControl>
    /// 0x086d5456 AllocateControl<ScrollAnim>
    /// 0x086d5a64 AllocateControl<ScrollPanelControl>
    /// 0x086d5584 AllocateControl<SlideAnim>
    /// 0x086d5d3c AllocateControl<SliderControl>
    /// 0x086d57f4 AllocateControl<StackControl>
    /// 0x086d56bc AllocateControl<TextButtonControl>
    /// 0x086d578c AllocateControl<TextControl>
    /// 0x086d5b9c AllocateControl<TextureBarControl>
    /// 0x089a1e8e AllocateControl<UIQuad>
    /// 0x086d64c8 AllocateControl<UIString1024>
    /// 0x086d637e AllocateControl<UIString128>
    /// 0x086d6680 AllocateControl<UIString16384>
    /// 0x086d6536 AllocateControl<UIString2048>
    /// 0x086d63ec AllocateControl<UIString256>
    /// 0x08696540 AllocateControl<UIString32>
    /// 0x086d65a4 AllocateControl<UIString4096>
    /// 0x086d645a AllocateControl<UIString512>
    /// 0x086d6310 AllocateControl<UIString64>
    /// 0x086d6612 AllocateControl<UIString8192>
    /// 0x086d5f4c AllocateControl<UITextureInfo>
    /// 0x086d5242 AllocateControl<WorldAnchorControl>
    template <class T0>
    T0* AllocateControl();
    /// 0x086ca0ee
    UNK_RET StartupRenderer();
    /// 0x086ca510
    UNK_RET BuildUIQuad();
    /// 0x086ca720
    UNK_RET Startup();
    /// 0x086caa54
    UNK_RET LoadColorAtlas();
    /// 0x086caa6e
    UNK_RET OnSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086cac6c
    UNK_RET StartupGame();
    /// 0x086cad2a
    UNK_RET Set3DCursorCurrentAnim();
    /// 0x086caf30
    UNK_RET WorldMouse(EventTemplate<Event_Int2Type<103>, WorldMouseOverData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086caf42
    UNK_RET ShutdownGame();
    /// 0x086caff2
    UNK_RET ApplyResolution(bool a1);
    /// 0x086cb688
    UNK_RET UpdateScreenMatrix();
    /// 0x086cb6f2
    UNK_RET RequestReprocessAnchoring();
    /// 0x086cb728
    UNK_RET Shutdown();
    /// 0x086cb8f6
    UNK_RET AddRootControl(ControlBase* a1);
    /// 0x086cb988
    UNK_RET RemoveRootControl(ControlBase* a1);
    /// 0x086cb9e4
    UNK_RET AddCiv5Colors();
    /// 0x086cbbba
    UNK_RET ExtractColor(FStringA const& a1, FGXVector4& a2);
    /// 0x086cbf00
    UNK_RET GetColorByName(char const* a1, FGXVector4& a2);
    /// 0x086cbf5e
    UNK_RET GetColorSetByName(char const* a1, ColorSet& a2);
    /// 0x086cbf92
    UNK_RET GetColorByHash(unsigned int a1, FGXVector4& a2);
    /// 0x086cc45c
    UNK_RET GetColorSetByHash(unsigned int a1, ColorSet& a2);
    /// 0x086cc490
    UNK_RET GetGameplayColorByName(char const* a1, unsigned int a2, unsigned long& a3);
    /// 0x086cc5c2
    UNK_RET AcquirGameplayIconFontAndIndexByName(char const* a1, unsigned int a2, FGXFont*& a3, int& a4);
    /// 0x086cc670
    UNK_RET AcquireIconFont(char const* a1);
    /// 0x086cc9b4
    UNK_RET ReleaseIconFont(FGXFont* a1);
    /// 0x086cca4c
    UNK_RET BuildControl(FXmlElement const& a1, LuaContext* a2);
    /// 0x086cd20a
    UNK_RET UpdateIsExtendedFont();
    /// 0x086cd2d2
    UNK_RET SetImeMode(bool a1);
    /// 0x086cd32e
    UNK_RET SetImeEnable(bool a1);
    /// 0x086cd374
    UNK_RET SetIme();
    /// 0x086cd3b6
    UNK_RET ProcessIme(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x086cd65e
    UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x086cd672
    UNK_RET ProcessNormalInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x086cddc8
    UNK_RET ProcessTouchInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x086cddce
    bool ProcessCameraDrag(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, long lParam);
    /// 0x086ce08e
    UNK_RET RegisterDragHandler(ScrollPanelControl* a1);
    /// 0x086ce09e
    UNK_RET UnRegisterDragHandler(ScrollPanelControl* a1);
    /// 0x086ce0ba
    UNK_RET RemoveMouseOver(ControlBase* a1);
    /// 0x086ce138
    UNK_RET RequestMouseOver(ControlBase* a1);
    /// 0x086ce184
    UNK_RET ImmediateReloadUI();
    /// 0x086ce1e6
    UNK_RET RequestReloadUI();
    /// 0x086ce26e
    UNK_RET SetKeyboardFocus(EditBoxControl* a1);
    /// 0x086ce2a8
    UNK_RET Process3DCursor();
    /// 0x086ce4a2
    UNK_RET BufferDraws(UIRenderBuffer& a1);
    /// 0x086ce74a
    UNK_RET Render(unsigned int a1);
    /// 0x086cf48e
    UNK_RET GetSpotToWriteFontVertices(FireGrafix::VStruct<9>** a1, unsigned int* a2, unsigned int a3);
    /// 0x086cf4e4
    UNK_RET ClearRenderTarget(unsigned int a1);
    /// 0x086cf590
    UNK_RET Update(float a1);
    /// 0x086cfb42
    UNK_RET CheckStylesReload();
    /// 0x086cfc22
    UNK_RET GetFont(char const* a1);
    /// 0x086cfd96
    UNK_RET ReleaseTexture(UITextureInfo* a1);
    /// 0x086cfdde
    UNK_RET ReleaseAndUnloadTextureByName(char const* a1);
    /// 0x086cfeb2
    UNK_RET ReleaseTextureByName(char const* a1);
    /// 0x086cff3c
    UNK_RET ReleaseAndUnloadTexture(UITextureInfo* a1);
    /// 0x086cff6c
    UNK_RET GetErrorTexture();
    /// 0x086cff76
    UNK_RET GetTexture(char const* a1);
    /// 0x086d058c
    UNK_RET AllocateString(unsigned int a1);
    /// 0x086d0652
    UNK_RET PrintTextureInfo();
    /// 0x086d0764
    UNK_RET SetUICursor(unsigned int a1);
    /// 0x086d0812
    UNK_RET LoadCursorA(char const* a1, unsigned int a2);
    /// 0x086d0880
    UNK_RET LookUpControl(char const* a1);
    /// 0x086d0a74
    UNK_RET GetGameGridTopLeft(int a1);
    /// 0x086d0aae
    UNK_RET GetGameGridBottomRight(int a1);
    /// 0x086d0ae8
    UNK_RET QueuePopup(LuaContext* a1, unsigned int a2);
    /// 0x086d0ce0
    UNK_RET DequeuePopup(LuaContext* a1);
    /// 0x086d0dd0
    UNK_RET PushModal(ControlBase* a1, bool a2);
    /// 0x086d0e8a
    UNK_RET PopModal(ControlBase* a1);
    /// 0x086d0f44
    UNK_RET IsModal(ControlBase* a1);
    /// 0x086d0f72
    UNK_RET IsTopModal(ControlBase* a1);
    /// 0x086d0f98
    UNK_RET FindBlock(unsigned int a1, unsigned int a2);
    /// 0x086d1022
    UNK_RET AllocateBlock(unsigned int a1);
    /// 0x086d109a
    UNK_RET PopAllInterrupts();
    /// 0x086d136a
    UNK_RET PrintAllocationInfo();
    /// 0x086d18ca
    /// lua ?
    static int lPushModal(lua_State* a1);
    /// 0x086d1986
    /// lua ?
    static int lGetMousePos(lua_State* a1);
    /// 0x086d19de
    /// lua ?
    static int lGetMouseDelta(lua_State* a1);
    /// 0x086d1a36
    /// lua ?
    static int lGetResInfo(lua_State* a1);
    /// 0x086d1b04
    /// lua ?
    static int lGetResCount(lua_State* a1);
    /// 0x086d1b36
    /// lua ?
    static int lGetScreenSizeVal(lua_State* a1);
    /// 0x086d1b86
    UNK_RET PrintWindowInfo(char const* a1);
    /// 0x086d1c90
    UNK_RET RenderScratchBox(unsigned int a1, UIAABB a2);
    /// 0x086d1ec4
    UNK_RET FlushInputMessages();
    /// 0x086d1eea
    UNK_RET GetStyleElement(FXmlElement& a1, FStringA const& a2);
    /// 0x086d1f4a
    UNK_RET GetStyleElement(FXmlElement& a1, char const* a2);
    /// 0x086d1fb8
    UNK_RET GetStyleElement(FXmlElement& a1, char const* a2, unsigned int a3);
    /// 0x086d2028
    UNK_RET RegisterObjectCache(IUIObjectCache* a1);
    /// 0x086d208e
    UNK_RET UnloadUnreferencedTextures();
    /// 0x086d21b0
    UNK_RET SetForceUnloadTexture(bool a1);
    /// 0x086d21c6
    UNK_RET GetVisibleNamedContext(char const* a1);
    /// 0x086d220a
    UNK_RET GetNamedContextByIndex(char const* a1, unsigned int a2);
    /// 0x086d231e
    /// lua ?
    static int lGetVisibleNamedContext(lua_State* a1);
    /// 0x086d2398
    UNK_RET GetNamedContextCount(char const* a1);
    /// 0x086d2764
    /// lua ?
    static int lGetNamedContextByIndex(lua_State* a1);
    unsigned char unk_4[0xa5c];                       ///< +0x4
    int m_iScreenWidth;                               ///< +0xa60
    int m_iScreenHeight;                              ///< +0xa64
    int m_lMouseParam;                                ///< +0xa68
    unsigned int m_uMouseKeyState;                    ///< +0xa6c
    float m_fMouseX;                                  ///< +0xa70
    float m_fMouseY;                                  ///< +0xa74
    float m_fMouseDownX;                              ///< +0xa78
    float m_fMouseDownY;                              ///< +0xa7c
    float m_fMouseDeltaX;                             ///< +0xa80
    float m_fMouseDeltaY;                             ///< +0xa84
    float m_fMouseNormX;                              ///< +0xa88
    float m_fMouseNormY;                              ///< +0xa8c
    float m_fMouseNormDeltaX;                         ///< +0xa90
    float m_fMouseNormDeltaY;                         ///< +0xa94
    unsigned char unk_a98[0x50];                      ///< +0xa98
    int field_ae8;                                    ///< +0xae8
    unsigned char unk_aec[0x4];                       ///< +0xaec
    unsigned short m_uKeyModifiers;                   ///< +0xaf0
    unsigned char unk_af2[0x2];                       ///< +0xaf2
    float field_af4;                                  ///< +0xaf4
    unsigned char unk_af8[0x8];                       ///< +0xaf8
    unsigned char field_b00;                          ///< +0xb00
    unsigned char unk_b01[0x7];                       ///< +0xb01
    // bit 0x4 left button down, 0x8 drag captured, 0x40 drag event this frame (cleared at end of Update)
    unsigned int m_uMouseDragFlags;                   ///< +0xb08
    /// 0x0a3d1810
    static UNK_DATA m_MemBlocks[0x50];
    /// 0x0a3d1808
    static UNK_DATA m_iAllocatedSize[0x4];
    /// 0x0a3d1804
    static UNK_DATA m_iNumControls[0x4];
    /// 0x0a3d180c
    static UNK_DATA m_iNumMemBlocks[0x4];
    /// 0x0a3d1860
    static UNK_DATA ms_CacheObjects[0xc];
};

/// 0x086d0638
UNK_RET TextureSort(UITextureInfo* a1, UITextureInfo* a2);
