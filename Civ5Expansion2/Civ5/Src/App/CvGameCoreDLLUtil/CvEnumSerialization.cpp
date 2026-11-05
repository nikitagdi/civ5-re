/// origin binary
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnumSerialization.h"

/// 0x08b91916
FDataStream & operator<<(FDataStream & stream, const SeaLevelTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b917c0
FDataStream & operator<<(FDataStream & stream, const PlayerColorTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b91ad6
FDataStream & operator<<(FDataStream & stream, const TurnTimerTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b91d06
FDataStream & operator<<(FDataStream & stream, const TeamTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b91d76
FDataStream & operator<<(FDataStream & stream, const PlayerTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b91de6
FDataStream & operator<<(FDataStream & stream, const ReligionTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b91e56
FDataStream & operator<<(FDataStream & stream, const PlayerVoteTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b91ec6
FDataStream & operator<<(FDataStream & stream, const OrderTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92016
FDataStream & operator<<(FDataStream & stream, const MissionTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b920f6
FDataStream & operator<<(FDataStream & stream, const TechTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92407
FDataStream & operator<<(FDataStream & stream, const SlotClaim & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92477
FDataStream & operator<<(FDataStream & stream, const SlotStatus & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92637
FDataStream & operator<<(FDataStream & stream, const VictoryTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92787
FDataStream & operator<<(FDataStream & stream, const UnitAITypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b927f7
FDataStream & operator<<(FDataStream & stream, const VoteSourceTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92867
FDataStream & operator<<(FDataStream & stream, const VoteTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92a97
FDataStream & operator<<(FDataStream & stream, const UnitTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92b77
FDataStream & operator<<(FDataStream & stream, const ReplayMessageTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92be7
FDataStream & operator<<(FDataStream & stream, const ResourceTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b92c57
FDataStream & operator<<(FDataStream & stream, const MissionAITypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b9302b
FDataStream & operator<<(FDataStream & stream, const UnitClassTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b9309b
FDataStream & operator<<(FDataStream & stream, const MinorCivTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b9333b
FDataStream & operator<<(FDataStream & stream, const TacticalAIMoveTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b9348b
FDataStream & operator<<(FDataStream & stream, const PolicyBranchTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b9356b
FDataStream & operator<<(FDataStream & stream, const NotificationTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b93651
FDataStream & operator<<(FDataStream & stream, const RouteTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b939d1
FDataStream & operator<<(FDataStream & stream, const StorageLocation & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b93a41
FDataStream & operator<<(FDataStream & stream, const LeagueTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b93ab1
FDataStream & operator<<(FDataStream & stream, const LeagueSpecialSessionTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b93b91
FDataStream & operator<<(FDataStream & stream, const LeagueProjectTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b93c01
FDataStream & operator<<(FDataStream & stream, const ResolutionTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b93c71
FDataStream & operator<<(FDataStream & stream, const ResolutionDecisionTypes & value) {
    INT32 raw = static_cast<INT32>(value);
    stream << raw;
    return stream;
}

/// 0x08b917f7
FDataStream & operator>>(FDataStream & stream, PlayerColorTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<PlayerColorTypes>(raw);
    return stream;
}

/// 0x08b935a2
FDataStream & operator>>(FDataStream & stream, NotificationTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<NotificationTypes>(raw);
    return stream;
}

/// 0x08b9194d
FDataStream & operator>>(FDataStream & stream, SeaLevelTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<SeaLevelTypes>(raw);
    return stream;
}

/// 0x08b919bd
FDataStream & operator>>(FDataStream & stream, WorldSizeTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<WorldSizeTypes>(raw);
    return stream;
}

/// 0x08b91b0d
FDataStream & operator>>(FDataStream & stream, TurnTimerTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<TurnTimerTypes>(raw);
    return stream;
}

/// 0x08b91d3d
FDataStream & operator>>(FDataStream & stream, TeamTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<TeamTypes>(raw);
    return stream;
}

/// 0x08b91dad
FDataStream & operator>>(FDataStream & stream, PlayerTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<PlayerTypes>(raw);
    return stream;
}

/// 0x08b91e1d
FDataStream & operator>>(FDataStream & stream, ReligionTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ReligionTypes>(raw);
    return stream;
}

/// 0x08b91e8d
FDataStream & operator>>(FDataStream & stream, PlayerVoteTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<PlayerVoteTypes>(raw);
    return stream;
}

/// 0x08b91efd
FDataStream & operator>>(FDataStream & stream, OrderTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<OrderTypes>(raw);
    return stream;
}

/// 0x08b9204d
FDataStream & operator>>(FDataStream & stream, MissionTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<MissionTypes>(raw);
    return stream;
}

/// 0x08b9212d
FDataStream & operator>>(FDataStream & stream, TechTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<TechTypes>(raw);
    return stream;
}

/// 0x08b9243e
FDataStream & operator>>(FDataStream & stream, SlotClaim & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<SlotClaim>(raw);
    return stream;
}

/// 0x08b924ae
FDataStream & operator>>(FDataStream & stream, SlotStatus & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<SlotStatus>(raw);
    return stream;
}

/// 0x08b9266e
FDataStream & operator>>(FDataStream & stream, VictoryTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<VictoryTypes>(raw);
    return stream;
}

/// 0x08b9282e
FDataStream & operator>>(FDataStream & stream, VoteSourceTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<VoteSourceTypes>(raw);
    return stream;
}

/// 0x08b9289e
FDataStream & operator>>(FDataStream & stream, VoteTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<VoteTypes>(raw);
    return stream;
}

/// 0x08b927be
FDataStream & operator>>(FDataStream & stream, UnitAITypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<UnitAITypes>(raw);
    return stream;
}

/// 0x08b92ace
FDataStream & operator>>(FDataStream & stream, UnitTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<UnitTypes>(raw);
    return stream;
}

/// 0x08b92bae
FDataStream & operator>>(FDataStream & stream, ReplayMessageTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ReplayMessageTypes>(raw);
    return stream;
}

/// 0x08b92c1e
FDataStream & operator>>(FDataStream & stream, ResourceTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ResourceTypes>(raw);
    return stream;
}

/// 0x08b92c8e
FDataStream & operator>>(FDataStream & stream, MissionAITypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<MissionAITypes>(raw);
    return stream;
}

/// 0x08b93062
FDataStream & operator>>(FDataStream & stream, UnitClassTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<UnitClassTypes>(raw);
    return stream;
}

/// 0x08b930d2
FDataStream & operator>>(FDataStream & stream, MinorCivTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<MinorCivTypes>(raw);
    return stream;
}

/// 0x08b93372
FDataStream & operator>>(FDataStream & stream, TacticalAIMoveTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<TacticalAIMoveTypes>(raw);
    return stream;
}

/// 0x08b934c2
FDataStream & operator>>(FDataStream & stream, PolicyBranchTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<PolicyBranchTypes>(raw);
    return stream;
}

/// 0x08b93688
FDataStream & operator>>(FDataStream & stream, RouteTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<RouteTypes>(raw);
    return stream;
}

/// 0x08b93a08
FDataStream & operator>>(FDataStream & stream, StorageLocation & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<StorageLocation>(raw);
    return stream;
}

/// 0x08b93a78
FDataStream & operator>>(FDataStream & stream, LeagueTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<LeagueTypes>(raw);
    return stream;
}

/// 0x08b93ae8
FDataStream & operator>>(FDataStream & stream, LeagueSpecialSessionTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<LeagueSpecialSessionTypes>(raw);
    return stream;
}

/// 0x08b93c38
FDataStream & operator>>(FDataStream & stream, ResolutionTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ResolutionTypes>(raw);
    return stream;
}

/// 0x08b93ca8
FDataStream & operator>>(FDataStream & stream, ResolutionDecisionTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ResolutionDecisionTypes>(raw);
    return stream;
}

/// 0x08b918dd
FDataStream & operator>>(FDataStream & stream, ClimateTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ClimateTypes>(raw);
    return stream;
}

/// 0x08b91986
FDataStream & operator<<(FDataStream & stream, const WorldSizeTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b91a2d
FDataStream & operator>>(FDataStream & stream, HandicapTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<HandicapTypes>(raw);
    return stream;
}

/// 0x08b91a9d
FDataStream & operator>>(FDataStream & stream, GameSpeedTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<GameSpeedTypes>(raw);
    return stream;
}

/// 0x08b91b7d
FDataStream & operator>>(FDataStream & stream, EraTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<EraTypes>(raw);
    return stream;
}

/// 0x08b91bed
FDataStream & operator>>(FDataStream & stream, CivilizationTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<CivilizationTypes>(raw);
    return stream;
}

/// 0x08b91c5d
FDataStream & operator>>(FDataStream & stream, LeaderHeadTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<LeaderHeadTypes>(raw);
    return stream;
}

/// 0x08b91fdd
FDataStream & operator>>(FDataStream & stream, CalendarTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<CalendarTypes>(raw);
    return stream;
}

/// 0x08b922ed
FDataStream & operator>>(FDataStream & stream, GameTypes & value) {
    // Stored as one signed byte, unlike the INT32 enums.
    char raw;
    stream >> raw;
    value = static_cast<GameTypes>(raw);
    return stream;
}

/// 0x08b9235e
FDataStream & operator>>(FDataStream & stream, GameMapTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<GameMapTypes>(raw);
    return stream;
}

/// 0x08b923ce
FDataStream & operator>>(FDataStream & stream, GameMode & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<GameMode>(raw);
    return stream;
}

/// 0x08b925fe
FDataStream & operator>>(FDataStream & stream, BandwidthType & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<BandwidthType>(raw);
    return stream;
}

/// 0x08b9290e
FDataStream & operator>>(FDataStream & stream, BuildingClassTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<BuildingClassTypes>(raw);
    return stream;
}

/// 0x08b92a5e
FDataStream & operator>>(FDataStream & stream, DirectionTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<DirectionTypes>(raw);
    return stream;
}

/// 0x08b92b3e
FDataStream & operator>>(FDataStream & stream, BuildingTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<BuildingTypes>(raw);
    return stream;
}

/// 0x08b92cfe
FDataStream & operator>>(FDataStream & stream, ActivityTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ActivityTypes>(raw);
    return stream;
}

/// 0x08b92f12
FDataStream & operator>>(FDataStream & stream, AutomateTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<AutomateTypes>(raw);
    return stream;
}

/// 0x08b92f82
FDataStream & operator>>(FDataStream & stream, GameOptionTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<GameOptionTypes>(raw);
    return stream;
}

/// 0x08b92ff2
FDataStream & operator>>(FDataStream & stream, ButtonPopupTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ButtonPopupTypes>(raw);
    return stream;
}

/// 0x08b93142
FDataStream & operator>>(FDataStream & stream, ArtStyleTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ArtStyleTypes>(raw);
    return stream;
}

/// 0x08b93222
FDataStream & operator>>(FDataStream & stream, GameStateTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<GameStateTypes>(raw);
    return stream;
}

/// 0x08b93292
FDataStream & operator>>(FDataStream & stream, BuildTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<BuildTypes>(raw);
    return stream;
}

/// 0x08b933e2
FDataStream & operator>>(FDataStream & stream, CitySpecializationTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<CitySpecializationTypes>(raw);
    return stream;
}

/// 0x08b93452
FDataStream & operator>>(FDataStream & stream, InvisibleTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<InvisibleTypes>(raw);
    return stream;
}

/// 0x08b93532
FDataStream & operator>>(FDataStream & stream, EndTurnBlockingTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<EndTurnBlockingTypes>(raw);
    return stream;
}

/// 0x08b936f8
FDataStream & operator>>(FDataStream & stream, ImprovementTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<ImprovementTypes>(raw);
    return stream;
}

/// 0x08b93768
FDataStream & operator>>(FDataStream & stream, CityAIFocusTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<CityAIFocusTypes>(raw);
    return stream;
}

/// 0x08b937d8
FDataStream & operator>>(FDataStream & stream, GreatPeopleDirectiveTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<GreatPeopleDirectiveTypes>(raw);
    return stream;
}

/// 0x08b93848
FDataStream & operator>>(FDataStream & stream, DiploUIStateTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<DiploUIStateTypes>(raw);
    return stream;
}

/// 0x08b938b8
FDataStream & operator>>(FDataStream & stream, LeaderheadAnimationTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<LeaderheadAnimationTypes>(raw);
    return stream;
}

/// 0x08b93b58
FDataStream & operator>>(FDataStream & stream, LeagueNameTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<LeagueNameTypes>(raw);
    return stream;
}

/// 0x08b93bc8
FDataStream & operator>>(FDataStream & stream, LeagueProjectTypes & value) {
    INT32 raw;
    stream >> raw;
    value = static_cast<LeagueProjectTypes>(raw);
    return stream;
}

/// 0x08b918a6
FDataStream & operator<<(FDataStream & stream, const ClimateTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b919f6
FDataStream & operator<<(FDataStream & stream, const HandicapTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b91a66
FDataStream & operator<<(FDataStream & stream, const GameSpeedTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b91b46
FDataStream & operator<<(FDataStream & stream, const EraTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b91bb6
FDataStream & operator<<(FDataStream & stream, const CivilizationTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b91c26
FDataStream & operator<<(FDataStream & stream, const LeaderHeadTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b91fa6
FDataStream & operator<<(FDataStream & stream, const CalendarTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b922b6
FDataStream & operator<<(FDataStream & stream, const GameTypes & value) {
    // One byte in the binary, unlike the other enums.
    stream << static_cast<char>(value);
    return stream;
}

/// 0x08b92327
FDataStream & operator<<(FDataStream & stream, const GameMapTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b92397
FDataStream & operator<<(FDataStream & stream, const GameMode & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b925c7
FDataStream & operator<<(FDataStream & stream, const BandwidthType & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b928d7
FDataStream & operator<<(FDataStream & stream, const BuildingClassTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b92a27
FDataStream & operator<<(FDataStream & stream, const DirectionTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b92b07
FDataStream & operator<<(FDataStream & stream, const BuildingTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b92cc7
FDataStream & operator<<(FDataStream & stream, const ActivityTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b92d37
std::string FSerialization::toString(const ActivityTypes & value) {
    switch (value) {
    case NO_ACTIVITY: return "NO_ACTIVITY";
    case ACTIVITY_AWAKE: return "ACTIVITY_AWAKE";
    case ACTIVITY_SLEEP: return "ACTIVITY_SLEEP";
    case ACTIVITY_HOLD: return "ACTIVITY_HOLD";
    case ACTIVITY_HEAL: return "ACTIVITY_HEAL";
    case ACTIVITY_SENTRY: return "ACTIVITY_SENTRY";
    case ACTIVITY_INTERCEPT: return "ACTIVITY_INTERCEPT";
    case ACTIVITY_MISSION: return "ACTIVITY_MISSION";
    case NUM_ACTIVITY_TYPES: return "NUM_ACTIVITY_TYPES";
    default: return "INVALID ENUM VALUE";
    }
}

/// 0x08b92e22
std::string FSerialization::toString(const DirectionTypes & value) {
    switch (value) {
    case NO_DIRECTION: return "NO_DIRECTION";
    case DIRECTION_NORTHEAST: return "DIRECTION_NORTHEAST";
    case DIRECTION_EAST: return "DIRECTION_EAST";
    case DIRECTION_SOUTHEAST: return "DIRECTION_SOUTHEAST";
    case DIRECTION_SOUTHWEST: return "DIRECTION_SOUTHWEST";
    case DIRECTION_WEST: return "DIRECTION_WEST";
    case DIRECTION_NORTHWEST: return "DIRECTION_NORTHWEST";
    case NUM_DIRECTION_TYPES: return "NUM_DIRECTION_TYPES";
    default: return "INVALID ENUM VALUE";
    }
}

/// 0x08b92edb
FDataStream & operator<<(FDataStream & stream, const AutomateTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b92f4b
FDataStream & operator<<(FDataStream & stream, const GameOptionTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b92fbb
FDataStream & operator<<(FDataStream & stream, const ButtonPopupTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b9310b
FDataStream & operator<<(FDataStream & stream, const ArtStyleTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b931eb
FDataStream & operator<<(FDataStream & stream, const GameStateTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b9325b
FDataStream & operator<<(FDataStream & stream, const BuildTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b933ab
FDataStream & operator<<(FDataStream & stream, const CitySpecializationTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b9341b
FDataStream & operator<<(FDataStream & stream, const InvisibleTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b934fb
FDataStream & operator<<(FDataStream & stream, const EndTurnBlockingTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b936c1
FDataStream & operator<<(FDataStream & stream, const ImprovementTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b93731
FDataStream & operator<<(FDataStream & stream, const CityAIFocusTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b937a1
FDataStream & operator<<(FDataStream & stream, const GreatPeopleDirectiveTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b93811
FDataStream & operator<<(FDataStream & stream, const DiploUIStateTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b93881
FDataStream & operator<<(FDataStream & stream, const LeaderheadAnimationTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}

/// 0x08b93b21
FDataStream & operator<<(FDataStream & stream, const LeagueNameTypes & value) {
    stream << static_cast<INT32>(value);
    return stream;
}
