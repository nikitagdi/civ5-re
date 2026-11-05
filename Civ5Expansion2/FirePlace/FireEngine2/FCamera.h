/// origin guess
#pragma once
#include "FirePlace/FireWorks/FReferenceObject.h"
#include <d3d9.h>
#include <Windows.h>

class FCamera;
class FFrustumPlanes;
class FGXVector3;
class FGXVector4;
template <class T0> class FRect;

/// size 0xd8
/// vtable 0x0a301900
class FCamera : public FReferenceObject {
public:
    class Frustum;
    /// size 0x18
    class Frustum {
    public:
        float m_fLeft;    ///< +0x0
        float m_fRight;   ///< +0x4
        float m_fBottom;  ///< +0x8
        float m_fTop;     ///< +0xc
        float m_fNear;    ///< +0x10
        float m_fFar;     ///< +0x14
    };
    /// 0x08cbeeec
    FCamera(float fViewAngle);
    /// 0x08cbf1e2
    /// deleting 0x08cbf1ce
    virtual ~FCamera();
    /// 0x08cbfa1a
    virtual void Update(float fDeltaTime);
    /// 0x08cbfce4
    virtual void GetViewProj(D3DXMATRIX* pView, D3DXMATRIX* pProj) const;
    /// 0x08cbf0de
    void Reset();
    /// 0x08cbf1b2
    void SetViewAngle(float fViewAngle, float fAspectRatio);
    /// 0x08cbf1fe
    void SetViewRange(float fNear, float fFar);
    /// 0x08cbf230
    void SetPosition(FGXVector3 const* pPosition);
    /// 0x08cbf256
    void SetRotation(D3DXMATRIX const& mRotation);
    /// 0x08cbf2b8
    D3DXMATRIX const* GetRotation() const;
    /// 0x08cbf2c0
    D3DXMATRIX const* GetWorldTransform() const;
    /// 0x08cbf2c8
    void SetTransform(D3DXMATRIX const& mTransform);
    /// 0x08cbf2e0
    float GetViewAngle() const;
    /// 0x08cbf374
    float GetAspectRatio() const;
    /// 0x08cbf37c
    void Zoom(float fFactor);
    /// 0x08cbf39e
    void SetWorldLocation(FGXVector3 const& vLocation);
    /// 0x08cbf3c4
    void SetWorldDirection(FGXVector3 const& vDirection);
    /// 0x08cbf3ea
    void SetLookAt(FGXVector3 const& vEye, FGXVector3 const& vAt, FGXVector3 const& vUp);
    /// 0x08cbf442
    void GetWorldLocation(FGXVector3* pOut) const;
    /// 0x08cbf468
    FGXVector3 GetWorldLocation() const;
    /// 0x08cbf490
    void GetWorldDirection(FGXVector3* pOut) const;
    /// 0x08cbf4b6
    FGXVector3 GetWorldDirection() const;
    /// 0x08cbf4de
    void GetViewingDirection(FGXVector3* pOut) const;
    /// 0x08cbf522
    FGXVector3 GetViewingDirection() const;
    /// 0x08cbf568
    void GetRow(FGXVector3* pOut, unsigned int uRow) const;
    /// 0x08cbf598
    void Move(float fX, float fY, float fZ);
    /// 0x08cbf616
    void Move(FGXVector3 const* pOffset);
    /// 0x08cbf68e
    void Pan(float fAngle);
    /// 0x08cbf6ee
    void Yaw(float fAngle);
    /// 0x08cbf74e
    void Tilt(float fAngle);
    /// 0x08cbf7ae
    void Roll(float fAngle);
    /// 0x08cbf80e
    void Pan(float a1, float a2, float a3);
    /// 0x08cbf810
    void Tilt(float a1, float a2, float a3);
    /// 0x08cbf812
    void Pan(float fAngle, float fRate);
    /// 0x08cbf890
    void Tilt(float fAngle, float fRate);
    /// 0x08cbf90e
    void Roll(float fAngle, float fRate);
    /// 0x08cbf98c
    void Move(float fDistance, float fRate);
    /// 0x08cbfc7e
    static FCamera* Create();
    /// 0x08cbfcc8
    static FCamera* (*SetCreator(FCamera* (*pfCreator)()))();
    /// 0x08cbfd66
    void GetClipPlanesInWorldSpace(FGXVector4* pPlanes) const;
    /// 0x08cbfe3a
    void GetClipPlanes(FFrustumPlanes* pPlanes) const;
    /// 0x08cbff0e
    D3DXMATRIX* GetWorldToCameraMatrix();
    /// 0x08cbff16
    UNK_RET SetViewPort(FRect<float> const& rViewPort);
    /// 0x08cbff38
    void SetViewFrustum(FCamera::Frustum const& frustum);
    /// 0x08cbff68
    void LookAtWorldPoint(FGXVector3 const& vAt, FGXVector3 const& vUp);
    /// 0x08cbffe2
    FGXVector3 GetWorldUpVector() const;
    /// 0x08cc000a
    FGXVector3 GetWorldRightVector() const;
    float m_fViewAngle;       ///< +0x8
    float m_fAspectRatio;     ///< +0xc
    float m_fPanRemaining;    ///< +0x10
    float m_fPanRate;         ///< +0x14
    float m_fTiltRemaining;   ///< +0x18
    float m_fTiltRate;        ///< +0x1c
    float m_fRollRemaining;   ///< +0x20
    float m_fRollRate;        ///< +0x24
    float m_fMoveRemaining;   ///< +0x28
    float m_fMoveRate;        ///< +0x2c
    D3DXMATRIX m_mWorld;      ///< +0x30
    D3DXMATRIX m_mWorldToCamera;  ///< +0x70
    float m_fViewPort[4];     ///< +0xb0
    Frustum m_frustum;        ///< +0xc0
    /// 0x0d267f10
    static FCamera* (*ms_pfCreator)();
};

namespace FGranny {
/// 0x0d268138
extern UNK_DATA m_bInitted[0x1];
/// 0x0d268120
extern UNK_DATA m_kLocalDagPoseCache[0x4];
/// 0x0d26812c
extern UNK_DATA m_kLocalDagPoseCache_Array[0x4];
/// 0x0d268124
extern UNK_DATA m_kLocalLocalPoseCache[0x4];
/// 0x0d268130
extern UNK_DATA m_kLocalLocalPoseCache_Array[0x4];
/// 0x0d268128
extern UNK_DATA m_kLocalWorldPoseCache[0x4];
/// 0x0d268134
extern UNK_DATA m_kLocalWorldPoseCache_Array[0x4];
/// 0x0d2680d0
extern UNK_DATA ms_kGrannyLock[0x18];
/// 0x0d2680f0
extern UNK_DATA ms_kStringPalette[0x30];
/// 0x0d2680e8
extern UNK_DATA ms_pkGrannyStringDatabase[0x4];
}
