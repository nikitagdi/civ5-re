/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"

class FGXUInt2;
class cvPath;
namespace ReplayMap {
class MapData;
class Surface;
}

namespace ReplayMap {
/// size ?
class Resources {
public:
    /// 0x089ec866
    Resources();
    /// 0x089ec88c
    ~Resources();
    /// 0x089ec8da
    UNK_RET Shutdown();
    /// 0x089ec97c
    UNK_RET Startup(Database::Connection& a1);
    /// 0x089ecdaa
    UNK_RET CreateSurface(ReplayMap::Surface& a1, cvPath const& a2, bool a3);
    /// 0x089ed06a
    UNK_RET DestroySurface(ReplayMap::Surface& a1);
    /// 0x089ed0c6
    UNK_RET GetTerrainSurface(ReplayMap::MapData const& a1, FGXUInt2 const& a2) const;
    /// 0x089ed132
    UNK_RET GetHexSurface() const;
};
}
