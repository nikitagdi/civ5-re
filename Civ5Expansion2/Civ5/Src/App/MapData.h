/// origin guess
#pragma once

class FGXUInt2;
namespace ReplayMap {
class PlotInfo;
}

namespace ReplayMap {
/// size ?
class MapData {
public:
    /// 0x089ec76c
    MapData();
    /// 0x089ec794
    ~MapData();
    /// 0x089ec7a8
    UNK_RET Shutdown();
    /// 0x089ec7aa
    UNK_RET Startup(FGXUInt2 const& a1, ReplayMap::PlotInfo const& a2);
    /// 0x089ec7ec
    UNK_RET GetPlot(FGXUInt2 const& a1) const;
    /// 0x089ec816
    UNK_RET SetPlot(FGXUInt2 const& a1, ReplayMap::PlotInfo const& a2);
};
}
