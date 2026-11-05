/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/cvLandmarkLibrary.h"
#include "Civ5/Src/App/cvAnimatedSimSystem.h"

class EraChangedData;
class HexFOWStateChangedData;
class HexID;
class LandmarkCreatedData;
class LandmarkHandleData;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NullEventClass;
class SerialContainer;
class TechAcquiredData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
template <unsigned int T0> class cvInfoIDBitSet;
namespace cvLandmark_Properties {
enum eLandmarkEra : int;
enum eLandmarkState : int;
enum eLandmarkType : int;
}
namespace cvLayoutFieldManager {
class CompressedDensityFieldSet_Header;
}

/// size ?
class cvLandmarkSimSystem {
public:
    char unk_0[0x24];                             ///< +0x0
    cvFarmSimSystem m_FarmSim;                    ///< +0x24
    cvAnimatedSimSystem m_AnimatedSim;            ///< +0x207e4
    char unk_208cc[0x402ec];                      ///< +0x208cc
    cvLandmarkLibrary* m_pLandmarkLibrary;        ///< +0x60bb8
    char unk_60bbc[0xa8];                         ///< +0x60bbc
    FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> m_DeterministicRefs; ///< +0x60c64
    FFastVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true, 54u, 0u, BaseVector<cvLandmarkLibrary::cvLandmarkLibraryReference, true>::FDefaultFastVectorAllocator> m_RandomRefs; ///< +0x60c70
    /// 0x087d5814
    cvLandmarkSimSystem();
    /// 0x087d598c
    ~cvLandmarkSimSystem();
    /// 0x087d5a38
    UNK_RET Startup(unsigned int a1);
    /// 0x087d5b78
    UNK_RET SubscribeToEvents();
    /// 0x087d5cc6
    UNK_RET Shutdown();
    /// 0x087d5d78
    UNK_RET UnSubscribeToEvents();
    /// 0x087d5ec6
    UNK_RET DestroySims();
    /// 0x087d5f2e
    UNK_RET StartupGame();
    /// 0x087d5f6e
    UNK_RET ShutdownGame();
    /// 0x087d5fd2
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d6008
    UNK_RET OnRawResourceCreated(GameCoreEventTemplate<Event_Int2Type<55>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d61a2
    UNK_RET OnRawResourceDestroyed(GameCoreEventTemplate<Event_Int2Type<56>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d625c
    UNK_RET OnImprovementCreated(GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d64f6
    UNK_RET OnImprovementDestroyed(GameCoreEventTemplate<Event_Int2Type<60>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d65b0
    UNK_RET OnFeatureCreated(GameCoreEventTemplate<Event_Int2Type<50>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d6738
    UNK_RET OnFeatureDestroyed(GameCoreEventTemplate<Event_Int2Type<51>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d67f2
    UNK_RET OnEraChanged(GameCoreEventTemplate<Event_Int2Type<48>, EraChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d6c80
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d6e56
    UNK_RET OnTechAcquired(GameCoreEventTemplate<Event_Int2Type<128>, TechAcquiredData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d748a
    UNK_RET Update(float a1);
    /// 0x087d74f8
    UNK_RET DestroySim(unsigned int a1);
    /// 0x087d754a
    int GetLandmarkEntryIndex(int iType, int iSubType, cvLandmark_Properties::eLandmarkEra eEra, cvLandmark_Properties::eLandmarkState eState, cvInfoIDBitSet<256u> const& kTechs) const;
    /// 0x087d75e6
    int GetBestLandmarkEntryIndex(int iType, int iSubType, cvLandmark_Properties::eLandmarkEra eEra, cvLandmark_Properties::eLandmarkState eState, cvInfoIDBitSet<256u> const& kTechs) const;
    /// 0x087d765e
    bool IsBestLandmarkEntryIndex(int iEntryIndex, int iType, int iSubType, cvLandmark_Properties::eLandmarkEra eEra, cvLandmark_Properties::eLandmarkState eState, cvInfoIDBitSet<256u> const& kTechs) const;
    /// 0x087d7780
    UNK_RET GetHandleByLocation(HexID const& a1, cvLandmark_Properties::eLandmarkType a2);
};

/// 0x087db248
UNK_RET TransformMaskThroughRotation(unsigned char a1, int a2);
/// 0x087db30f
UNK_RET TransformMaskThroughReflection(unsigned char a1, int a2);

namespace RUN_LENGTH_CODING {
/// 0x08924aa0 CompressUCharDataBlock<FFastVector<unsigned char, true, 43u, 265u, cvCompressedTerrainPiecesLibrary::TerrainPieceTempHeapAlloc> >
/// 0x087dc1ff CompressUCharDataBlock<FFastVector<unsigned char, true, 47u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator> >
template <class T0>
void CompressUCharDataBlock(unsigned char* a1, unsigned int a2, T0* a3);
}

namespace cvLayoutFieldManager {
/// 0x087db3d6
UNK_RET LoadAllDensityFields();
/// 0x087db8d0
UNK_RET LoadCompressedDensityFieldSetFromFiles(char const* a1, cvLayoutFieldManager::CompressedDensityFieldSet_Header& a2);
/// 0x087dbc80
UNK_RET LoadAllTensorFields();
}
