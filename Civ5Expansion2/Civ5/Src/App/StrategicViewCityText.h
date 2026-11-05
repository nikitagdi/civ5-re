/// origin guess
#pragma once

class CvStrategicViewRenderer;
class FGXVector4;

/// size ?
class StrategicViewCityText {
public:
    /// 0x08c9316c
    StrategicViewCityText(CvStrategicViewRenderer const& a1);
    /// 0x08c931a2
    ~StrategicViewCityText();
    /// 0x08c931f2
    UNK_RET Clear();
    /// 0x08c9323c
    UNK_RET CityAdded(int a1, char const* a2, FGXVector4 const& a3);
    /// 0x08c932d8
    UNK_RET CityRemoved(int a1);
    /// 0x08c93368
    UNK_RET GetCityText(int a1);
};
