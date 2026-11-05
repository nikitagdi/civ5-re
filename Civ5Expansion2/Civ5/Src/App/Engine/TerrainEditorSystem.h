/// origin guess
#pragma once

class FGXInt2;
class FGXVector3;
class HexPatternCell;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SquareGridPolicy;
class TerrainCacheData;
class TerrainData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class TerrainEditorSystem {
public:
    /// 0x08b783a0
    TerrainEditorSystem();
    /// 0x08b7884c
    UNK_RET Startup(TerrainData const* a1);
    /// 0x08b79282
    UNK_RET OnTerrainCached(EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b79302
    UNK_RET Shutdown();
    /// 0x08b794e4
    UNK_RET CreateTerrain(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& a1);
    /// 0x08b797ca
    UNK_RET StartupGame(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& a1, FGXInt2 a2, unsigned short* a3, unsigned char* a4, unsigned char* a5, unsigned char* a6);
    /// 0x08b79ba6
    UNK_RET ShutdownGame();
    /// 0x08b79ca6
    UNK_RET Render(unsigned int a1);
    /// 0x08b79d42
    UNK_RET Update(float a1, FGXVector3& a2, FGXVector3& a3);
    /// 0x08b79e82
    UNK_RET ToggleDiffuse();
    /// 0x08b79f06
    UNK_RET ToggleSpecular();
    /// 0x08b79f8a
    UNK_RET ReloadLightEnv();
    /// 0x08b79fea
    UNK_RET ToggleCellUpdates();
};
