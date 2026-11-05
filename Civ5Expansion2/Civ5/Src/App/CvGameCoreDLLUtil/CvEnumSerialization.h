/// origin binary
#ifndef INCLUDED_CvEnumSerialization_H
#define INCLUDED_CvEnumSerialization_H

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"

/// 0x08b917c0
FDataStream & operator<<(FDataStream &, const PlayerColorTypes &);
/// 0x08b917f7
FDataStream & operator>>(FDataStream &, PlayerColorTypes &);

/// 0x08b91830
FDataStream & operator<<(FDataStream &, const PlayerOptionTypes &);
/// 0x08b91867
FDataStream & operator>>(FDataStream &, PlayerOptionTypes &);

/// 0x08b918a6
FDataStream & operator<<(FDataStream &, const ClimateTypes &);
/// 0x08b918dd
FDataStream & operator>>(FDataStream &, ClimateTypes &);

/// 0x08b91916
FDataStream & operator<<(FDataStream &, const SeaLevelTypes &);
/// 0x08b9194d
FDataStream & operator>>(FDataStream &, SeaLevelTypes &);

/// 0x08b91986
FDataStream & operator<<(FDataStream &, const WorldSizeTypes &);
/// 0x08b919bd
FDataStream & operator>>(FDataStream &, WorldSizeTypes &);

/// 0x08b919f6
FDataStream & operator<<(FDataStream &, const HandicapTypes &);
/// 0x08b91a2d
FDataStream & operator>>(FDataStream &, HandicapTypes &);

/// 0x08b91a66
FDataStream & operator<<(FDataStream &, const GameSpeedTypes &);
/// 0x08b91a9d
FDataStream & operator>>(FDataStream &, GameSpeedTypes &);

/// 0x08b91ad6
FDataStream & operator<<(FDataStream &, const TurnTimerTypes &);
/// 0x08b91b0d
FDataStream & operator>>(FDataStream &, TurnTimerTypes &);

/// 0x08b91b46
FDataStream & operator<<(FDataStream &, const EraTypes &);
/// 0x08b91b7d
FDataStream & operator>>(FDataStream &, EraTypes &);

/// 0x08b91bb6
FDataStream & operator<<(FDataStream &, const CivilizationTypes &);
/// 0x08b91bed
FDataStream & operator>>(FDataStream &, CivilizationTypes &);

/// 0x08b91c26
FDataStream & operator<<(FDataStream &, const LeaderHeadTypes &);
/// 0x08b91c5d
FDataStream & operator>>(FDataStream &, LeaderHeadTypes &);

/// 0x08b91c96
FDataStream & operator<<(FDataStream &, const ChatTargetTypes &);
/// 0x08b91ccd
FDataStream & operator>>(FDataStream &, ChatTargetTypes &);

/// 0x08b91d06
FDataStream & operator<<(FDataStream &, const TeamTypes &);
/// 0x08b91d3d
FDataStream & operator>>(FDataStream &, TeamTypes &);

/// 0x08b91d76
FDataStream & operator<<(FDataStream &, const PlayerTypes &);
/// 0x08b91dad
FDataStream & operator>>(FDataStream &, PlayerTypes &);

/// 0x08b91de6
FDataStream & operator<<(FDataStream &, const ReligionTypes &);
/// 0x08b91e1d
FDataStream & operator>>(FDataStream &, ReligionTypes &);

/// 0x08b91e56
FDataStream & operator<<(FDataStream &, const PlayerVoteTypes &);
/// 0x08b91e8d
FDataStream & operator>>(FDataStream &, PlayerVoteTypes &);

/// 0x08b91ec6
FDataStream & operator<<(FDataStream &, const OrderTypes &);
/// 0x08b91efd
FDataStream & operator>>(FDataStream &, OrderTypes &);

/// 0x08b91f36
FDataStream & operator<<(FDataStream &, const TaskTypes &);
/// 0x08b91f6d
FDataStream & operator>>(FDataStream &, TaskTypes &);

/// 0x08b91fa6
FDataStream & operator<<(FDataStream &, const CalendarTypes &);
/// 0x08b91fdd
FDataStream & operator>>(FDataStream &, CalendarTypes &);

/// 0x08b92016
FDataStream & operator<<(FDataStream &, const MissionTypes &);
/// 0x08b9204d
FDataStream & operator>>(FDataStream &, MissionTypes &);

/// 0x08b92086
FDataStream & operator<<(FDataStream &, const CommandTypes &);
/// 0x08b920bd
FDataStream & operator>>(FDataStream &, CommandTypes &);

/// 0x08b920f6
FDataStream & operator<<(FDataStream &, const TechTypes &);
/// 0x08b9212d
FDataStream & operator>>(FDataStream &, TechTypes &);

/// 0x08b92166
FDataStream & operator<<(FDataStream &, const PolicyTypes &);
/// 0x08b9219d
FDataStream & operator>>(FDataStream &, PolicyTypes &);

/// 0x08b921d6
FDataStream & operator<<(FDataStream &, const NetContactTypes &);
/// 0x08b9220d
FDataStream & operator>>(FDataStream &, NetContactTypes &);

/// 0x08b92246
FDataStream & operator<<(FDataStream &, const SaveGameTypes &);
/// 0x08b9227d
FDataStream & operator>>(FDataStream &, SaveGameTypes &);

/// 0x08b922b6
FDataStream & operator<<(FDataStream &, const GameTypes &);
/// 0x08b922ed
FDataStream & operator>>(FDataStream &, GameTypes &);

/// 0x08b92327
FDataStream & operator<<(FDataStream &, const GameMapTypes &);
/// 0x08b9235e
FDataStream & operator>>(FDataStream &, GameMapTypes &);

/// 0x08b92397
FDataStream & operator<<(FDataStream &, const GameMode &);
/// 0x08b923ce
FDataStream & operator>>(FDataStream &, GameMode &);

/// 0x08b92407
FDataStream & operator<<(FDataStream &, const SlotClaim &);
/// 0x08b9243e
FDataStream & operator>>(FDataStream &, SlotClaim &);

/// 0x08b92477
FDataStream & operator<<(FDataStream &, const SlotStatus &);
/// 0x08b924ae
FDataStream & operator>>(FDataStream &, SlotStatus &);

/// 0x08b924e7
FDataStream & operator<<(FDataStream &, const CivLoginStates &);
/// 0x08b9251e
FDataStream & operator>>(FDataStream &, CivLoginStates &);

/// 0x08b92557
FDataStream & operator<<(FDataStream &, const VersionTypes &);
/// 0x08b9258e
FDataStream & operator>>(FDataStream &, VersionTypes &);

/// 0x08b925c7
FDataStream & operator<<(FDataStream &, const BandwidthType &);
/// 0x08b925fe
FDataStream & operator>>(FDataStream &, BandwidthType &);

/// 0x08b92637
FDataStream & operator<<(FDataStream &, const VictoryTypes &);
/// 0x08b9266e
FDataStream & operator>>(FDataStream &, VictoryTypes &);

/// 0x08b926a7
FDataStream & operator<<(FDataStream &, const SmallAwardTypes &);
/// 0x08b926de
FDataStream & operator>>(FDataStream &, SmallAwardTypes &);

/// 0x08b92717
FDataStream & operator<<(FDataStream &, const AdvancedStartActionTypes &);
/// 0x08b9274e
FDataStream & operator>>(FDataStream &, AdvancedStartActionTypes &);

/// 0x08b92787
FDataStream & operator<<(FDataStream &, const UnitAITypes &);
/// 0x08b927be
FDataStream & operator>>(FDataStream &, UnitAITypes &);

/// 0x08b927f7
FDataStream & operator<<(FDataStream &, const VoteSourceTypes &);
/// 0x08b9282e
FDataStream & operator>>(FDataStream &, VoteSourceTypes &);

/// 0x08b92867
FDataStream & operator<<(FDataStream &, const VoteTypes &);
/// 0x08b9289e
FDataStream & operator>>(FDataStream &, VoteTypes &);

/// 0x08b928d7
FDataStream & operator<<(FDataStream &, const BuildingClassTypes &);
/// 0x08b9290e
FDataStream & operator>>(FDataStream &, BuildingClassTypes &);

/// 0x08b92947
FDataStream & operator<<(FDataStream &, const ColorTypes &);
/// 0x08b9297e
FDataStream & operator>>(FDataStream &, ColorTypes &);

/// 0x08b929b7
FDataStream & operator<<(FDataStream &, const InterfaceMessageTypes &);
/// 0x08b929ee
FDataStream & operator>>(FDataStream &, InterfaceMessageTypes &);

/// 0x08b92a27
FDataStream & operator<<(FDataStream &, const DirectionTypes &);
/// 0x08b92a5e
FDataStream & operator>>(FDataStream &, DirectionTypes &);
/// 0x08b92e22
namespace FSerialization { std::string toString(const DirectionTypes &); }

/// 0x08b92a97
FDataStream & operator<<(FDataStream &, const UnitTypes &);
/// 0x08b92ace
FDataStream & operator>>(FDataStream &, UnitTypes &);

/// 0x08b92b07
FDataStream & operator<<(FDataStream &, const BuildingTypes &);
/// 0x08b92b3e
FDataStream & operator>>(FDataStream &, BuildingTypes &);

/// 0x08b92b77
FDataStream & operator<<(FDataStream &, const ReplayMessageTypes &);
/// 0x08b92bae
FDataStream & operator>>(FDataStream &, ReplayMessageTypes &);

/// 0x08b92be7
FDataStream & operator<<(FDataStream &, const ResourceTypes &);
/// 0x08b92c1e
FDataStream & operator>>(FDataStream &, ResourceTypes &);

/// 0x08b92c57
FDataStream & operator<<(FDataStream &, const MissionAITypes &);
/// 0x08b92c8e
FDataStream & operator>>(FDataStream &, MissionAITypes &);

/// 0x08b92cc7
FDataStream & operator<<(FDataStream &, const ActivityTypes &);
/// 0x08b92cfe
FDataStream & operator>>(FDataStream &, ActivityTypes &);
/// 0x08b92d37
namespace FSerialization { std::string toString(const ActivityTypes &); }

/// 0x08b92edb
FDataStream & operator<<(FDataStream &, const AutomateTypes &);
/// 0x08b92f12
FDataStream & operator>>(FDataStream &, AutomateTypes &);

/// 0x08b92f4b
FDataStream & operator<<(FDataStream &, const GameOptionTypes &);
/// 0x08b92f82
FDataStream & operator>>(FDataStream &, GameOptionTypes &);

/// 0x08b92fbb
FDataStream & operator<<(FDataStream &, const ButtonPopupTypes &);
/// 0x08b92ff2
FDataStream & operator>>(FDataStream &, ButtonPopupTypes &);

/// 0x08b9302b
FDataStream & operator<<(FDataStream &, const UnitClassTypes &);
/// 0x08b93062
FDataStream & operator>>(FDataStream &, UnitClassTypes &);

/// 0x08b9309b
FDataStream & operator<<(FDataStream &, const MinorCivTypes &);
/// 0x08b930d2
FDataStream & operator>>(FDataStream &, MinorCivTypes &);

/// 0x08b9310b
FDataStream & operator<<(FDataStream &, const ArtStyleTypes &);
/// 0x08b93142
FDataStream & operator>>(FDataStream &, ArtStyleTypes &);

/// 0x08b9317b
FDataStream & operator<<(FDataStream &, const DiploCommentTypes &);
/// 0x08b931b2
FDataStream & operator>>(FDataStream &, DiploCommentTypes &);

/// 0x08b931eb
FDataStream & operator<<(FDataStream &, const GameStateTypes &);
/// 0x08b93222
FDataStream & operator>>(FDataStream &, GameStateTypes &);

/// 0x08b9325b
FDataStream & operator<<(FDataStream &, const BuildTypes &);
/// 0x08b93292
FDataStream & operator>>(FDataStream &, BuildTypes &);

/// 0x08b932cb
FDataStream & operator<<(FDataStream &, const CultureLevelTypes &);
/// 0x08b93302
FDataStream & operator>>(FDataStream &, CultureLevelTypes &);

/// 0x08b9333b
FDataStream & operator<<(FDataStream &, const TacticalAIMoveTypes &);
/// 0x08b93372
FDataStream & operator>>(FDataStream &, TacticalAIMoveTypes &);

/// 0x08b933ab
FDataStream & operator<<(FDataStream &, const CitySpecializationTypes &);
/// 0x08b933e2
FDataStream & operator>>(FDataStream &, CitySpecializationTypes &);

/// 0x08b9341b
FDataStream & operator<<(FDataStream &, const InvisibleTypes &);
/// 0x08b93452
FDataStream & operator>>(FDataStream &, InvisibleTypes &);

/// 0x08b9348b
FDataStream & operator<<(FDataStream &, const PolicyBranchTypes &);
/// 0x08b934c2
FDataStream & operator>>(FDataStream &, PolicyBranchTypes &);

/// 0x08b934fb
FDataStream & operator<<(FDataStream &, const EndTurnBlockingTypes &);
/// 0x08b93532
FDataStream & operator>>(FDataStream &, EndTurnBlockingTypes &);

/// 0x08b9356b
FDataStream & operator<<(FDataStream &, const NotificationTypes &);
/// 0x08b935a2
FDataStream & operator>>(FDataStream &, NotificationTypes &);

/// 0x08b935e1
FDataStream & operator<<(FDataStream &, const FromUIDiploEventTypes &);
/// 0x08b93618
FDataStream & operator>>(FDataStream &, FromUIDiploEventTypes &);

/// 0x08b93651
FDataStream & operator<<(FDataStream &, const RouteTypes &);
/// 0x08b93688
FDataStream & operator>>(FDataStream &, RouteTypes &);

/// 0x08b936c1
FDataStream & operator<<(FDataStream &, const ImprovementTypes &);
/// 0x08b936f8
FDataStream & operator>>(FDataStream &, ImprovementTypes &);

/// 0x08b93731
FDataStream & operator<<(FDataStream &, const CityAIFocusTypes &);
/// 0x08b93768
FDataStream & operator>>(FDataStream &, CityAIFocusTypes &);

/// 0x08b937a1
FDataStream & operator<<(FDataStream &, const GreatPeopleDirectiveTypes &);
/// 0x08b937d8
FDataStream & operator>>(FDataStream &, GreatPeopleDirectiveTypes &);

/// 0x08b93811
FDataStream & operator<<(FDataStream &, const DiploUIStateTypes &);
/// 0x08b93848
FDataStream & operator>>(FDataStream &, DiploUIStateTypes &);

/// 0x08b93881
FDataStream & operator<<(FDataStream &, const LeaderheadAnimationTypes &);
/// 0x08b938b8
FDataStream & operator>>(FDataStream &, LeaderheadAnimationTypes &);

/// 0x08b938f1
FDataStream & operator<<(FDataStream &, const GameExecutableTypes &);
/// 0x08b93928
FDataStream & operator>>(FDataStream &, GameExecutableTypes &);

/// 0x08b93961
FDataStream & operator<<(FDataStream &, const BeliefTypes &);
/// 0x08b93998
FDataStream & operator>>(FDataStream &, BeliefTypes &);

/// 0x08b939d1
FDataStream & operator<<(FDataStream&, const StorageLocation&);
/// 0x08b93a08
FDataStream & operator>>(FDataStream&, StorageLocation&);

/// 0x08b93a41
FDataStream & operator<<(FDataStream&, const LeagueTypes&);
/// 0x08b93a78
FDataStream & operator>>(FDataStream&, LeagueTypes&);

/// 0x08b93ab1
FDataStream & operator<<(FDataStream&, const LeagueSpecialSessionTypes&);
/// 0x08b93ae8
FDataStream & operator>>(FDataStream&, LeagueSpecialSessionTypes&);

/// 0x08b93b21
FDataStream & operator<<(FDataStream&, const LeagueNameTypes&);
/// 0x08b93b58
FDataStream & operator>>(FDataStream&, LeagueNameTypes&);

/// 0x08b93b91
FDataStream & operator<<(FDataStream&, const LeagueProjectTypes&);
/// 0x08b93bc8
FDataStream & operator>>(FDataStream&, LeagueProjectTypes&);

/// 0x08b93c01
FDataStream & operator<<(FDataStream&, const ResolutionTypes&);
/// 0x08b93c38
FDataStream & operator>>(FDataStream&, ResolutionTypes&);

/// 0x08b93c71
FDataStream & operator<<(FDataStream&, const ResolutionDecisionTypes&);
/// 0x08b93ca8
FDataStream & operator>>(FDataStream&, ResolutionDecisionTypes&);


#endif//INCLUDED_CvEnumSerialization_H
