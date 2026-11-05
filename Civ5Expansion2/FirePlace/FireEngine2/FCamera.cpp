/// origin guess
#include "FirePlace/FireEngine2/FCamera.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/core/FImageCoder.h"
#include <math.h>

/// 0x08cbeeec
FCamera::FCamera( float fViewAngle ) {
    m_fPanRemaining = 0.0f;
    m_fPanRate = 0.0f;
    m_fTiltRemaining = 0.0f;
    m_fTiltRate = 0.0f;
    m_fRollRemaining = 0.0f;
    m_fRollRate = 0.0f;
    m_fMoveRemaining = 0.0f;
    m_fMoveRate = 0.0f;
    m_frustum.m_fNear = 1.0f;
    m_frustum.m_fFar = 10000.0f;
    m_fViewPort[0] = 0.0f;
    m_fViewPort[1] = 1.0f;
    m_fViewPort[2] = 1.0f;
    m_fViewPort[3] = 0.0f;
    m_mWorld._11 = 1.0f; m_mWorld._12 = 0.0f; m_mWorld._13 = 0.0f; m_mWorld._14 = 0.0f;
    m_mWorld._21 = 0.0f; m_mWorld._22 = 1.0f; m_mWorld._23 = 0.0f; m_mWorld._24 = 0.0f;
    m_mWorld._31 = 0.0f; m_mWorld._32 = 0.0f; m_mWorld._33 = 1.0f; m_mWorld._34 = 0.0f;
    m_mWorld._41 = 0.0f; m_mWorld._42 = 0.0f; m_mWorld._43 = 0.0f; m_mWorld._44 = 1.0f;
    m_fViewAngle = fViewAngle;
    m_fAspectRatio = 1.3333334f;
    float fTan = tanf( (float)( (double)( fViewAngle * 0.5f ) * 0.017453292519943295 ) );
    float fAngle = atanf( 0.75f );
    float fCos = cosf( fAngle );
    float fSin = sinf( fAngle );
    m_frustum.m_fLeft = -( fCos * fTan );
    m_frustum.m_fRight = fCos * fTan;
    m_frustum.m_fTop = fTan * fSin;
    m_frustum.m_fBottom = -( fTan * fSin );
}

/// 0x08cbf0de
void FCamera::Reset() {
    m_fPanRemaining = 0.0f;
    m_fPanRate = 0.0f;
    m_fTiltRemaining = 0.0f;
    m_fTiltRate = 0.0f;
    m_fRollRemaining = 0.0f;
    m_fRollRate = 0.0f;
    m_fMoveRemaining = 0.0f;
    m_fMoveRate = 0.0f;
    m_frustum.m_fNear = 1.0f;
    m_frustum.m_fFar = 10000.0f;
    m_fViewPort[0] = 0.0f;
    m_fViewPort[1] = 1.0f;
    m_fViewPort[2] = 1.0f;
    m_fViewPort[3] = 0.0f;
    m_mWorld._11 = 1.0f; m_mWorld._12 = 0.0f; m_mWorld._13 = 0.0f; m_mWorld._14 = 0.0f;
    m_mWorld._21 = 0.0f; m_mWorld._22 = 1.0f; m_mWorld._23 = 0.0f; m_mWorld._24 = 0.0f;
    m_mWorld._31 = 0.0f; m_mWorld._32 = 0.0f; m_mWorld._33 = 1.0f; m_mWorld._34 = 0.0f;
    m_mWorld._41 = 0.0f; m_mWorld._42 = 0.0f; m_mWorld._43 = 0.0f; m_mWorld._44 = 1.0f;
}

/// 0x08cbfa1a
void FCamera::Update( float fDeltaTime ) {
    auto StepToward = []( float fRemaining, float fMaxStep ) -> float {
        float fStep = 0.0f;
        if ( 0.0f < fRemaining ) {
            fStep = fRemaining;
            if ( fMaxStep <= fRemaining ) {
                fStep = fMaxStep;
            }
        }
        if ( fRemaining < 0.0f ) {
            fStep = -fMaxStep;
            if ( fStep < fRemaining ) {
                fStep = fRemaining;
            }
        }
        return fStep;
    };
    if ( m_fPanRemaining != 0.0f ) {
        float fStep = StepToward( m_fPanRemaining, m_fPanRate * fDeltaTime );
        m_fPanRemaining -= fStep;
        D3DXMATRIX mRotation;
        D3DXMATRIX mResult;
        D3DXMatrixRotationY( &mRotation, fStep );
        D3DXMatrixMultiply( &mResult, &mRotation, &m_mWorld );
        m_mWorld = mResult;
    }
    if ( m_fTiltRemaining != 0.0f ) {
        float fStep = StepToward( m_fTiltRemaining, m_fTiltRate * fDeltaTime );
        m_fTiltRemaining -= fStep;
        D3DXMATRIX mRotation;
        D3DXMATRIX mResult;
        D3DXMatrixRotationX( &mRotation, fStep );
        D3DXMatrixMultiply( &mResult, &mRotation, &m_mWorld );
        m_mWorld = mResult;
    }
    if ( m_fRollRemaining != 0.0f ) {
        float fStep = StepToward( m_fRollRemaining, m_fRollRate * fDeltaTime );
        m_fRollRemaining -= fStep;
        D3DXMATRIX mRotation;
        D3DXMATRIX mResult;
        D3DXMatrixRotationZ( &mRotation, fStep );
        D3DXMatrixMultiply( &mResult, &m_mWorld, &mRotation );
        m_mWorld = mResult;
    }
    if ( m_fMoveRemaining != 0.0f ) {
        float fStep = StepToward( m_fMoveRemaining, m_fMoveRate * fDeltaTime );
        m_fMoveRemaining -= fStep;
        D3DXMATRIX mTranslation;
        D3DXMATRIX mResult;
        D3DXMatrixTranslation( &mTranslation, 0.0f, 0.0f, fStep );
        D3DXMatrixMultiply( &mResult, &mTranslation, &m_mWorld );
        m_mWorld = mResult;
    }
}

/// 0x08cbf1b2
void FCamera::SetViewAngle( float fViewAngle, float fAspectRatio ) {
    m_fViewAngle = fViewAngle;
    m_fAspectRatio = fAspectRatio;
}

/// 0x08cbf1fe
void FCamera::SetViewRange( float fNear, float fFar ) {
    if ( fNear != 0.0f ) {
        m_frustum.m_fNear = fNear;
    }
    if ( fFar != 0.0f ) {
        m_frustum.m_fFar = fFar;
    }
}

/// 0x08cbf230
void FCamera::SetPosition( FGXVector3 const* pPosition ) {
    m_mWorld._41 = pPosition->x;
    m_mWorld._42 = pPosition->y;
    m_mWorld._43 = pPosition->z;
}

/// 0x08cbf256
void FCamera::SetRotation( D3DXMATRIX const& mRotation ) {
    m_mWorld._11 = mRotation._11;
    m_mWorld._12 = mRotation._12;
    m_mWorld._13 = mRotation._13;
    m_mWorld._21 = mRotation._21;
    m_mWorld._22 = mRotation._22;
    m_mWorld._23 = mRotation._23;
    m_mWorld._31 = mRotation._31;
    m_mWorld._32 = mRotation._32;
    m_mWorld._33 = mRotation._33;
}

/// 0x08cbf2b8
D3DXMATRIX const* FCamera::GetRotation() const {
    return &m_mWorld;
}

/// 0x08cbf2c0
D3DXMATRIX const* FCamera::GetWorldTransform() const {
    return &m_mWorld;
}

/// 0x08cbf2c8
void FCamera::SetTransform( D3DXMATRIX const& mTransform ) {
    m_mWorld = mTransform;
}

/// 0x08cbf2e0
float FCamera::GetViewAngle() const {
    float fWidth = m_frustum.m_fRight - m_frustum.m_fLeft;
    float fHeight = m_frustum.m_fTop - m_frustum.m_fBottom;
    float fDiagonal = sqrtf( fHeight * fHeight + fWidth * fWidth );
    float fHalf = atanf( fDiagonal * 0.5f );
    return (float)( (double)( fHalf + fHalf ) * 57.29577951308232 );
}

/// 0x08cbf374
float FCamera::GetAspectRatio() const {
    return m_fAspectRatio;
}

/// 0x08cbf37c
void FCamera::Zoom( float fFactor ) {
    m_frustum.m_fLeft = fFactor * m_frustum.m_fLeft;
    m_frustum.m_fRight = fFactor * m_frustum.m_fRight;
    m_frustum.m_fBottom = fFactor * m_frustum.m_fBottom;
    m_frustum.m_fTop = fFactor * m_frustum.m_fTop;
}

/// 0x08cbf39e
void FCamera::SetWorldLocation( FGXVector3 const& vLocation ) {
    m_mWorld._41 = vLocation.x;
    m_mWorld._42 = vLocation.y;
    m_mWorld._43 = vLocation.z;
}

/// 0x08cbf3c4
void FCamera::SetWorldDirection( FGXVector3 const& vDirection ) {
    m_mWorld._31 = vDirection.x;
    m_mWorld._32 = vDirection.y;
    m_mWorld._33 = vDirection.z;
}

/// 0x08cbf3ea
void FCamera::SetLookAt( FGXVector3 const& vEye, FGXVector3 const& vAt, FGXVector3 const& vUp ) {
    D3DXMATRIX mView;
    D3DXMatrixLookAtRH( &mView, (D3DXVECTOR3 const*)&vEye, (D3DXVECTOR3 const*)&vAt, (D3DXVECTOR3 const*)&vUp );
    D3DXMatrixInverse( &m_mWorld, 0, &mView );
}

/// 0x08cbf442
void FCamera::GetWorldLocation( FGXVector3* pOut ) const {
    pOut->x = m_mWorld._41;
    pOut->y = m_mWorld._42;
    pOut->z = m_mWorld._43;
}

/// 0x08cbf468
FGXVector3 FCamera::GetWorldLocation() const {
    FGXVector3 vResult;
    vResult.x = m_mWorld._41;
    vResult.y = m_mWorld._42;
    vResult.z = m_mWorld._43;
    return vResult;
}

/// 0x08cbf490
void FCamera::GetWorldDirection( FGXVector3* pOut ) const {
    pOut->x = m_mWorld._31;
    pOut->y = m_mWorld._32;
    pOut->z = m_mWorld._33;
}

/// 0x08cbf4b6
FGXVector3 FCamera::GetWorldDirection() const {
    FGXVector3 vResult;
    vResult.x = m_mWorld._31;
    vResult.y = m_mWorld._32;
    vResult.z = m_mWorld._33;
    return vResult;
}

/// 0x08cbf4de
void FCamera::GetViewingDirection( FGXVector3* pOut ) const {
    pOut->x = -m_mWorld._31;
    pOut->y = -m_mWorld._32;
    pOut->z = -m_mWorld._33;
}

/// 0x08cbf522
FGXVector3 FCamera::GetViewingDirection() const {
    FGXVector3 vResult;
    vResult.x = -m_mWorld._31;
    vResult.y = -m_mWorld._32;
    vResult.z = -m_mWorld._33;
    return vResult;
}

/// 0x08cbf568
void FCamera::GetRow( FGXVector3* pOut, unsigned int uRow ) const {
    pOut->x = m_mWorld.m[uRow][0];
    pOut->y = m_mWorld.m[uRow][1];
    pOut->z = m_mWorld.m[uRow][2];
}

/// 0x08cbf598
void FCamera::Move( float fX, float fY, float fZ ) {
    D3DXMATRIX mTranslation;
    D3DXMATRIX mResult;
    D3DXMatrixTranslation( &mTranslation, fX, fY, fZ );
    D3DXMatrixMultiply( &mResult, &mTranslation, &m_mWorld );
    m_mWorld = mResult;
}

/// 0x08cbf616
void FCamera::Move( FGXVector3 const* pOffset ) {
    D3DXMATRIX mTranslation;
    D3DXMATRIX mResult;
    D3DXMatrixTranslation( &mTranslation, pOffset->x, pOffset->y, pOffset->z );
    D3DXMatrixMultiply( &mResult, &mTranslation, &m_mWorld );
    m_mWorld = mResult;
}

/// 0x08cbf68e
void FCamera::Pan( float fAngle ) {
    D3DXMATRIX mRotation;
    D3DXMATRIX mResult;
    D3DXMatrixRotationY( &mRotation, fAngle );
    D3DXMatrixMultiply( &mResult, &mRotation, &m_mWorld );
    m_mWorld = mResult;
}

/// 0x08cbf6ee
void FCamera::Yaw( float fAngle ) {
    D3DXMATRIX mRotation;
    D3DXMATRIX mResult;
    D3DXMatrixRotationY( &mRotation, fAngle );
    D3DXMatrixMultiply( &mResult, &mRotation, &m_mWorld );
    m_mWorld = mResult;
}

/// 0x08cbf74e
void FCamera::Tilt( float fAngle ) {
    D3DXMATRIX mRotation;
    D3DXMATRIX mResult;
    D3DXMatrixRotationX( &mRotation, fAngle );
    D3DXMatrixMultiply( &mResult, &mRotation, &m_mWorld );
    m_mWorld = mResult;
}

/// 0x08cbf7ae
void FCamera::Roll( float fAngle ) {
    D3DXMATRIX mRotation;
    D3DXMATRIX mResult;
    D3DXMatrixRotationZ( &mRotation, fAngle );
    D3DXMatrixMultiply( &mResult, &m_mWorld, &mRotation );
    m_mWorld = mResult;
}

/// 0x08cbf80e
void FCamera::Pan( float a1, float a2, float a3 ) {
}

/// 0x08cbf810
void FCamera::Tilt( float a1, float a2, float a3 ) {
}

/// 0x08cbf812
void FCamera::Pan( float fAngle, float fRate ) {
    if ( fRate != 0.0f ) {
        m_fPanRemaining = fAngle;
        m_fPanRate = fRate;
    } else {
        D3DXMATRIX mRotation;
        D3DXMATRIX mResult;
        D3DXMatrixRotationY( &mRotation, fAngle );
        D3DXMatrixMultiply( &mResult, &mRotation, &m_mWorld );
        m_mWorld = mResult;
    }
}

/// 0x08cbf890
void FCamera::Tilt( float fAngle, float fRate ) {
    if ( fRate != 0.0f ) {
        m_fTiltRemaining = fAngle;
        m_fTiltRate = fRate;
    } else {
        D3DXMATRIX mRotation;
        D3DXMATRIX mResult;
        D3DXMatrixRotationX( &mRotation, fAngle );
        D3DXMatrixMultiply( &mResult, &mRotation, &m_mWorld );
        m_mWorld = mResult;
    }
}

/// 0x08cbf90e
void FCamera::Roll( float fAngle, float fRate ) {
    if ( fRate != 0.0f ) {
        m_fRollRemaining = fAngle;
        m_fRollRate = fRate;
    } else {
        D3DXMATRIX mRotation;
        D3DXMATRIX mResult;
        D3DXMatrixRotationZ( &mRotation, fAngle );
        D3DXMatrixMultiply( &mResult, &m_mWorld, &mRotation );
        m_mWorld = mResult;
    }
}

/// 0x08cbf98c
void FCamera::Move( float fDistance, float fRate ) {
    if ( fRate != 0.0f ) {
        m_fMoveRemaining = fDistance;
        m_fMoveRate = fRate;
    } else {
        D3DXMATRIX mTranslation;
        D3DXMATRIX mResult;
        D3DXMatrixTranslation( &mTranslation, 0.0f, 0.0f, fDistance );
        D3DXMatrixMultiply( &mResult, &mTranslation, &m_mWorld );
        m_mWorld = mResult;
    }
}

/// 0x08cbfc7e
FCamera* FCamera::Create() {
    if ( ms_pfCreator ) {
        return ms_pfCreator();
    }
    return new FCamera( 45.0f );
}

/// 0x08cbfcc8
FCamera* (*FCamera::SetCreator( FCamera* (*pfCreator)() ))() {
    FCamera* (*pfPrevious)() = ms_pfCreator;
    ms_pfCreator = pfCreator;
    return pfPrevious;
}

/// 0x08cbfce4
void FCamera::GetViewProj( D3DXMATRIX* pView, D3DXMATRIX* pProj ) const {
    D3DXMatrixInverse( pView, 0, &m_mWorld );
    D3DXMatrixPerspectiveFovRH( pProj, (float)( (double)m_fViewAngle * 0.017453292519943295 ), m_fAspectRatio, m_frustum.m_fNear, m_frustum.m_fFar );
}

/// 0x08cbfd66
void FCamera::GetClipPlanesInWorldSpace( FGXVector4* pPlanes ) const {
    D3DXMATRIX mProj;
    FGXVector4 aViewPlanes[6];
    D3DXMATRIX mInverse;
    D3DXMatrixPerspectiveFovRH( &mProj, (float)( (double)m_fViewAngle * 0.017453292519943295 ), m_fAspectRatio, m_frustum.m_fNear, m_frustum.m_fFar );
    FGXGetClipPlanesFromMatrix( &mProj, aViewPlanes );
    D3DXMatrixInverse( &mInverse, 0, &m_mWorld );
    D3DXMatrixTranspose( &mInverse, &mInverse );
    for ( int i = 0; i < 6; i++ ) {
        D3DXPlaneTransform( (D3DXPLANE*)&pPlanes[i], (D3DXPLANE const*)&aViewPlanes[i], &mInverse );
    }
}

/// 0x08cbfe3a
void FCamera::GetClipPlanes( FFrustumPlanes* pPlanes ) const {
    D3DXMATRIX mProj;
    FGXVector4 aViewPlanes[6];
    D3DXMATRIX mInverse;
    float fDeterminant;
    D3DXMatrixPerspectiveFovRH( &mProj, (float)( (double)m_fViewAngle * 0.017453292519943295 ), m_fAspectRatio, m_frustum.m_fNear, m_frustum.m_fFar );
    FGXGetClipPlanesFromMatrix( &mProj, aViewPlanes );
    D3DXMatrixInverse( &mInverse, &fDeterminant, &m_mWorld );
    D3DXMatrixTranspose( &mInverse, &mInverse );
    FGXVector4* pOut = (FGXVector4*)pPlanes;
    for ( int i = 0; i < 6; i++ ) {
        D3DXPlaneTransform( (D3DXPLANE*)&pOut[i], (D3DXPLANE const*)&aViewPlanes[i], &mInverse );
    }
}

/// 0x08cbff0e
D3DXMATRIX* FCamera::GetWorldToCameraMatrix() {
    return &m_mWorldToCamera;
}

/// 0x08cbff38
void FCamera::SetViewFrustum( FCamera::Frustum const& frustum ) {
    m_frustum = frustum;
}

/// 0x08cbff68
void FCamera::LookAtWorldPoint( FGXVector3 const& vAt, FGXVector3 const& vUp ) {
    D3DXVECTOR3 vEye;
    vEye.x = m_mWorld._41;
    vEye.y = m_mWorld._42;
    vEye.z = m_mWorld._43;
    D3DXMATRIX mView;
    D3DXMatrixLookAtRH( &mView, &vEye, (D3DXVECTOR3 const*)&vAt, (D3DXVECTOR3 const*)&vUp );
    D3DXMatrixInverse( &m_mWorld, 0, &mView );
}

/// 0x08cbffe2
FGXVector3 FCamera::GetWorldUpVector() const {
    FGXVector3 vResult;
    vResult.x = m_mWorld._21;
    vResult.y = m_mWorld._22;
    vResult.z = m_mWorld._23;
    return vResult;
}

/// 0x08cc000a
FGXVector3 FCamera::GetWorldRightVector() const {
    FGXVector3 vResult;
    vResult.x = m_mWorld._11;
    vResult.y = m_mWorld._12;
    vResult.z = m_mWorld._13;
    return vResult;
}
