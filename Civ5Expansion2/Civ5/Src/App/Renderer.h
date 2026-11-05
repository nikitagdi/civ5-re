/// origin guess
#pragma once

class FGXUInt2;
namespace ReplayMap {
class CameraData;
class MapData;
class Resources;
}

namespace ReplayMap {
/// size ?
class Renderer {
public:
    /// 0x089ed28a
    Renderer();
    /// 0x089ed296
    ~Renderer();
    /// 0x089ed2c0
    UNK_RET Shutdown();
    /// 0x089ed338
    UNK_RET Startup(FGXUInt2 const& a1);
    /// 0x089ed3ec
    UNK_RET Render(ReplayMap::CameraData const& a1, ReplayMap::MapData const& a2, ReplayMap::Resources const& a3);
    /// 0x089ed59e
    UNK_RET RenderTerrain(ReplayMap::CameraData const& a1, ReplayMap::MapData const& a2, ReplayMap::Resources const& a3);
    /// 0x089ed770
    UNK_RET RenderColorOverlay(ReplayMap::CameraData const& a1, ReplayMap::MapData const& a2, ReplayMap::Resources const& a3);
};
}
