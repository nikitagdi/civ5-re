/// origin guess
#pragma once
#include "Civ5/Src/App/ModAssociations.h"
#include "Civ5/Src/CvGameDatabase/Database.h"
#include <Windows.h>
#include <list>
#include <lua.h>
#include <memory>
#include <vector>

class AppUpdateData;
class CvReplayHeader;
class CvSavedGameHeader;
class FGameIniParser;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvContentPackageIDList;
class cvEventSystemLocalMachineAccessor;
class cvPath;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class CvModdingFrameworkAppSide {
public:
    /// 0x08983f78
    CvModdingFrameworkAppSide();
    /// 0x089840d2
    ~CvModdingFrameworkAppSide();
    /// 0x08984220
    UNK_RET UpdateContentSearchFolders();
    /// 0x089844d2
    UNK_RET UpdateModsList();
    /// 0x0898450e
    UNK_RET Initialize(Database::Connection& a1);
    /// 0x08984bb4
    UNK_RET SetDefaultDLCExclusion(_GUID const& a1, bool a2);
    /// 0x08984bf8
    UNK_RET OnModdingAppUpdate(EventTemplate<Event_Int2Type<2>, AppUpdateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08984cae
    UNK_RET AddFolderToWatch(cvPath const& a1);
    /// 0x08984d34
    UNK_RET Shutdown();
    /// 0x08984e2e
    UNK_RET FolderChangeNotify(void* a1, unsigned char a2);
    /// 0x08984e52
    UNK_RET SetFolderContentsChanged();
    /// 0x08984e5e
    UNK_RET DiscoverBaseGameMaps(std::vector<cvPath, std::allocator<cvPath> >& a1);
    /// 0x08985524
    UNK_RET DiscoverModderMaps(std::vector<cvPath, std::allocator<cvPath> >& a1);
    /// 0x08985684
    UNK_RET DiscoverBaseGameMapScripts(std::vector<cvPath, std::allocator<cvPath> >& a1);
    /// 0x08985d66
    UNK_RET DiscoverModderMapScripts(std::vector<cvPath, std::allocator<cvPath> >& a1);
    /// 0x08985f06
    UNK_RET ParseMapScripts(Database::Connection& a1, std::vector<cvPath, std::allocator<cvPath> > const& a2);
    /// 0x0898608a
    static int pRegisterMapScript(lua_State* a1);
    /// 0x089861b0
    static int pGetMapScriptInfo(lua_State* a1);
    /// 0x089872a8
    UNK_RET GetAvailableDLC(cvContentPackageIDList& a1) const;
    /// 0x089872f8
    UNK_RET CanLoadReplay(CvReplayHeader& a1);
    /// 0x08987594
    UNK_RET CanLoadSavedGame(CvSavedGameHeader& a1);
    /// 0x08987830
    UNK_RET SetupAudioCatalogByLanguageType(char const* a1);
    /// 0x089879a8
    UNK_RET ActivatedModsAffectSavedgame();
    /// 0x089879e0
    UNK_RET ActivatedModsNeedAudioSystemReload();
    /// 0x08987a18
    UNK_RET ActivatedModsNeedLandmarkSystemReload();
    /// 0x08987a50
    UNK_RET ActivatedModsNeedStrategicViewSystemReload();
    /// 0x08987a88
    UNK_RET ActivatedModsNeedUnitSystemReload();
    /// 0x08987ac0
    UNK_RET CheckWhetherStatsAndAchievementsCanBeUnlocked();
    /// 0x08987c4a
    UNK_RET ReloadAudioSystem();
    /// 0x08987c78
    UNK_RET ReloadLandmarkSystem();
    /// 0x08987cca
    UNK_RET ReloadStrategicViewSystem();
    /// 0x08987cfa
    UNK_RET ReloadUnitSystem();
    /// 0x08987d58
    UNK_RET SetModDllPath(cvPath const& a1);
    /// 0x08987d8c
    UNK_RET GetDLLPathFromMods(cvPath& a1);
    /// 0x08987e78
    UNK_RET GetDLLPathFromDLC(cvPath& a1);
    /// 0x08988168
    UNK_RET GetBaseGameDLL(cvPath& a1);
    /// 0x08988198
    UNK_RET AdjustAndValidatePath(cvPath& a1);
    /// 0x0898819e
    UNK_RET GetDLLPath(cvPath& a1);
    /// 0x089881f2
    UNK_RET UnloadCvGameCoreDLL();
    /// 0x08988f9e
    UNK_RET LoadCvGameCoreDLL();
    /// 0x0898a346
    UNK_RET ApplyGameCoreConfigSettings(FGameIniParser* a1);
    /// 0x0898ab8c
    UNK_RET ActivateModsAndDLCForCloudSave(cvPath const& a1);
    /// 0x0898ad02
    UNK_RET ActivateModsAndDLCForSavedGame(CvSavedGameHeader& a1);
    /// 0x0898b4c6
    UNK_RET ActivateModsAndDLCForSavedGameFile(cvPath const& a1);
    /// 0x0898b5a8
    UNK_RET SetActiveDLCandMods(cvContentPackageIDList const& a1, std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a2, bool a3, bool a4);
    /// 0x0898c60a
    UNK_RET ActivateModsAndDLCForReplay(CvReplayHeader& a1);
    /// 0x0898cc4a
    UNK_RET GetActiveDLCForMods(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1, cvContentPackageIDList& a2);
    /// 0x0898d392
    UNK_RET ActivateModsAndDLCForEnabledMods();
    /// 0x0898d58a
    UNK_RET ActivateSpecificModAndDLC(char const* a1, int a2);
    /// 0x0898d6a2
    UNK_RET IsDLCGameplay(_GUID const& a1);
    /// 0x0898d702
    UNK_RET GetActivatedDLC() const;
    /// 0x0898d70c
    UNK_RET GetActivatedMods();
    /// 0x0898d716
    UNK_RET DeactivateMods();
    /// 0x0898d7da
    UNK_RET PostProcessModActions();
};
