/// origin guess
#pragma once

class FCamera;

/// size ?
class CvLeaderHeadFlypath {
public:
    /// 0x08a2cd70
    CvLeaderHeadFlypath();
    /// 0x08a2cdc4
    ~CvLeaderHeadFlypath();
    /// 0x08a2cdf6
    UNK_RET Clear();
    /// 0x08a2ce38
    UNK_RET Load(char const* a1);
    /// 0x08a2ce88
    UNK_RET InitFromFile(char const* a1);
    /// 0x08a2d626
    UNK_RET AffectCamera(FCamera* a1, float a2, float a3) const;
    /// 0x08a2da94
    UNK_RET Update(float a1);
    /// 0x08a2db0a
    UNK_RET SetFSMPathAction(float a1, bool a2);
    /// 0x08a2db6c
    UNK_RET ResetFSM();
};
