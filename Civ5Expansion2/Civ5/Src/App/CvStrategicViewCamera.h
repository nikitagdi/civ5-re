/// origin guess
#pragma once

class CvStrategicViewPlotData;

// Hex range of the strategic view, one value pair per axis.
/// size 0x10
class CvStrategicViewPlotRange {
public:
    int m_iMinX; ///< +0x0
    int m_iMaxX; ///< +0x4
    int m_iMinY; ///< +0x8
    int m_iMaxY; ///< +0xc
};

/// size 0xc
class CvStrategicViewCamera {
public:
    /// 0x08c87f70
    CvStrategicViewCamera();

    float m_fX;               ///< +0x0
    float m_fY;               ///< +0x4
    unsigned char unk_8[0x4]; ///< +0x8
};

/// 0x08c87f89
UNK_RET DefaultGetPlotData(CvStrategicViewPlotRange const& a1, CvStrategicViewPlotData* a2);
