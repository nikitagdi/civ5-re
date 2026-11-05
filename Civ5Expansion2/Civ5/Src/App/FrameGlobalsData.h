/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include <Windows.h>

// Payload of local-machine event 34, published by GameViewState::PublishFrameGlobals; matrices and regions have one slot per camera.
/// size 0x84c
class FrameGlobalsData {
public:
    float m_fMaxZoom;                                   ///< +0x0
    float m_fMinZoom;                                   ///< +0x4
    float m_fCurrZoom;                                  ///< +0x8
    float m_fCurrZoomNorm;                              ///< +0xc
    D3DXMATRIX m_mProj;                                 ///< +0x10
    D3DXMATRIX m_amView[4];                             ///< +0x50
    D3DXMATRIX m_amInvView[4];                          ///< +0x150
    FGXVector4 m_aavViewClipPlanes[4][6];               ///< +0x250
    D3DXMATRIX m_amOverlayProjection[4];                ///< +0x3d0
    D3DXMATRIX m_amShadowViewProj[4];                   ///< +0x4d0
    D3DXMATRIX m_amReflectionViewProj[4];               ///< +0x5d0
    FGXVector3 m_avWorldTopLeftVisible[4];              ///< +0x6d0
    FGXVector3 m_avWorldBottomRightVisible[4];          ///< +0x700
    FGXVector3 m_avWorldTopLeftVisibleUnclamped[4];     ///< +0x730
    FGXVector3 m_avWorldBottomRightVisibleUnclamped[4]; ///< +0x760
    FGXInt2 m_akGameGridTopLeftVisible[4];              ///< +0x790
    FGXInt2 m_akGameGridBottomRightVisible[4];          ///< +0x7b0
    FGXInt2 m_akGameGridTopLeftVisibleUnclamped[4];     ///< +0x7d0
    FGXInt2 m_akGameGridBottomRightVisibleUnclamped[4]; ///< +0x7f0
    unsigned int m_uCameras;                            ///< +0x810
    unsigned int m_uFrame;                              ///< +0x814
    // Device id with its high and low 16 bits or-ed together.
    unsigned int m_uDeviceIdHash;                       ///< +0x818
    float m_afFOWBaseScale[4];                          ///< +0x81c
    float m_afFogColor[4];                              ///< +0x82c
    float m_afFogControl[4];                            ///< +0x83c
};

// Payload of local-machine event 35: the game-grid hexes in view, republished when either corner set changes.
/// size 0x40
class VisibilityUpdatedData {
public:
    FGXInt2 m_akGameGridTopLeftVisible[4];     ///< +0x0
    FGXInt2 m_akGameGridBottomRightVisible[4]; ///< +0x20
};
