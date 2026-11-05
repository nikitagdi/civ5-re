/// origin guess
#pragma once

class FGXVector2;
class FGXVector3;
class cvCatmullRom;

/// size ?
class cvCatmullRom {
public:
    /// 0x08a251c8
    cvCatmullRom();
    /// 0x08a251e6
    cvCatmullRom(cvCatmullRom const& a1);
    /// 0x08a251e0
    ~cvCatmullRom();
    /// 0x08a252be
    UNK_RET GetNumControlPoints() const;
    /// 0x08a252c6
    UNK_RET ReserveControlPoints(unsigned int a1);
    /// 0x08a252cc
    UNK_RET AddControlPoint(FGXVector3 const& a1);
    /// 0x08a2532e
    UNK_RET GetControlPoint(unsigned int a1) const;
    /// 0x08a2533a
    UNK_RET ClearControlPoints();
    /// 0x08a25350
    UNK_RET FreeControlPoints();
    /// 0x08a25392
    UNK_RET AddControlPoint(FGXVector2 const& a1);
    /// 0x08a25416
    UNK_RET GetDirectionOnSpline(float a1) const;
    /// 0x08a254bc
    UNK_RET GetPointOnSpline(float a1) const;
};

namespace cvTimer {
/// 0x08a259b8
UNK_RET Pause(bool a1);
/// 0x08a25a10
UNK_RET IsPaused();
/// 0x08a25a35
UNK_RET Restore();
/// 0x08a25a90
UNK_RET AddToUpdateRate(double a1);
/// 0x08a25b10
UNK_RET AddToSimulatedFPS(double a1);
/// 0x08a25b98
UNK_RET GetSimulatedFPS();
/// 0x08a25bab
UNK_RET GetUpdateRate();
}
