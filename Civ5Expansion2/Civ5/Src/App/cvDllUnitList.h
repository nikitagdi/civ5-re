/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStructs.h"
#include <Windows.h>
#include <lua.h>
#include <memory>
#include <string>
#include <vector>

class cvPath;

/// size ?
/// vtable 0x0a2fc750
class cvDllUnitList : public ICvEnumerator {
public:
    /// 0x08764bfc
    cvDllUnitList(std::vector<IDInfo, std::allocator<IDInfo> > const& a1);
    /// 0x08764cec
    virtual void Destroy();
    /// 0x08764d82
    virtual ICvUnknown * GetCurrent();
    /// 0x08764d26
    virtual bool MoveNext();
    /// 0x08764c30
    virtual void * QueryInterface(_GUID a1);
    /// 0x08764d76
    virtual void Reset();
    /// 0x08764c2e
    ~cvDllUnitList();
    /// 0x08764c92
    UNK_RET IncrementReference();
    /// 0x08764c9e
    UNK_RET DecrementReference();
    /// 0x08764cd0
    void operator delete(void* a1);
    /// 0x08764ce4
    UNK_RET GetReferenceCount();
    /// 0x08764d12
    void* operator new(unsigned int a1);
};

namespace GameCore {
/// 0x08764dc0
UNK_RET LinuxConvertRelativeWinDLLPathToAbsoluteLinuxSoPath(char const* a1, std::string& a2);
/// 0x08764eb9
UNK_RET LoadDLL(cvPath const& a1);
/// 0x08765257
UNK_RET UnloadDLL();
/// 0x08765313
UNK_RET GetLoadedDLLPath(cvPath& a1);
/// 0x0876540b
std::auto_ptr<ICvGameContext1> GetContext();
/// 0x0876543c
UNK_RET IsContextAvailable();
/// 0x08765459
std::auto_ptr<ICvPreGame1> GetPreGame();
/// 0x087654d0
std::auto_ptr<ICvGameAsynch1> GetGameAsynch();
/// 0x08765541
UNK_RET GetGame();
/// 0x087655b2
std::auto_ptr<ICvMap1> GetMap();
/// 0x08765623
UNK_RET GetTeam(TeamTypes a1);
/// 0x0876569c
UNK_RET GetDirectionAngle(DirectionTypes a1);
/// 0x087656bc
std::auto_ptr<ICvPlayer1> GetPlayer(PlayerTypes a1);
/// 0x0876574f
UNK_RET GetPlotByDirection(int a1, int a2, DirectionTypes a3);
/// 0x087658df
UNK_RET GetPlotByXY(int a1, int a2, int a3, int a4);
/// 0x087659a3
UNK_RET GetPlotByXYWithRangeCheck(int a1, int a2, int a3, int a4, int a5);
/// 0x08765a2e
UNK_RET GetPlotCity(int a1, int a2, int a3);
/// 0x08765c00
UNK_RET GetPlotDistance(int a1, int a2, int a3, int a4);
/// 0x08765d80
UNK_RET GetCity(IDInfo const& a1);
/// 0x08765e20
UNK_RET GetUnitUnsafe(IDInfo const& a1);
/// 0x08765eb6
UNK_RET GetUnit(IDInfo const& a1);
/// 0x08765edf
UNK_RET GetLock();
/// 0x08765f1d
UNK_RET ReleaseLock();
/// 0x08765f6c
UNK_RET HasLock();
/// 0x08765f94
UNK_RET TryLock();
/// 0x08765fe1
UNK_RET Shutdown();
}

namespace cvLuaContentManager {
/// 0x087662c0
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x08766324
int pRegister(lua_State* a1);
/// 0x08766494
/// lua ?
int lIsActive(lua_State* a1);
/// 0x0876653a
/// lua ?
int lSetActive(lua_State* a1);
/// 0x087668a6
/// lua ?
int lIsInstalled(lua_State* a1);
/// 0x0876691c
/// lua ?
int lGetAllPackageIDs(lua_State* a1);
/// 0x08766aee
/// lua ?
int lGetPackageDescription(lua_State* a1);
/// 0x08766d5e
/// lua ?
int lIsUpgrade(lua_State* a1);
}
