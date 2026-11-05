/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"
#include "Civ5/Src/App/CvLeaderHeadErrorHook.h"
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>

class cvPath;
template <class T0, int T1> class FSTLVector;

/// size ?
/// vtable 0x0a2fd2c0
class CvLeaderHeadBenchmark : public Civ5GameState, public CvLeaderHeadErrorHook {
public:
    /// 0x08829cdc
    CvLeaderHeadBenchmark();
    /// 0x08829e5a
    /// deleting 0x08829e32
    virtual ~CvLeaderHeadBenchmark();
    /// 0x0882a092
    virtual UNK_RET Begin();
    /// 0x0882a358
    virtual UNK_RET End();
    /// 0x0882ac84
    virtual UNK_RET Update(float a1);
    /// 0x0882af58
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0882b11c
    virtual UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x0882b16c
    /// thunk 0x0882b278
    virtual UNK_RET ErrorMessage(FStringA const& a1);
    /// 0x08829fb0
    UNK_RET Initialize(FSTLVector<FStringA, 0> const& a1, cvPath const& a2, unsigned int a3, bool a4, bool a5, bool a6, bool a7, bool a8);
    /// 0x0882a118
    UNK_RET OpenBenchmarkLog(cvPath const& a1, bool a2, bool a3);
    /// 0x0882a208
    UNK_RET SetupScene(unsigned int a1);
    /// 0x0882a3ce
    UNK_RET WriteBenchmarkResults(bool a1);
    /// 0x0882abe4
    UNK_RET DestroyScene();
    /// 0x0882ac3e
    UNK_RET WriteLogSeperator();
    /// 0x0882ad48
    UNK_RET LeaderheadTimeStep(float a1);
    /// 0x0a30fa10
    static UNK_DATA DEFAULT_LOG_FILE_NAME[0x4];
    /// 0x0920ded0
    static UNK_DATA FRAME_TIME[0x4];
};

extern "C" {
    /// 0x0a30fbbc
    extern UNK_DATA cvBridgeFilename[0x4];
    /// 0x0a30fbc0
    extern UNK_DATA cvBridgeMeshNames[0x18];
    /// 0x0a30fab4
    extern UNK_DATA cvCityWallIDs[0x1c];
    /// 0x0a30fae8
    extern UNK_DATA cvCityWallNames[0x1c];
    /// 0x0a30fb04
    extern UNK_DATA cvCityWallType[0xc];
    /// 0x0a30fb10
    extern char const* cvGreatWallFilenames[2];
    /// 0x0a30fb18
    extern char const* cvGreatWallModelNames[4];
    /// 0x0a30fb28
    extern char const* cvGreatWallTreecutNames[4];
    /// 0x0a30fad8
    extern UNK_DATA cvIrradianceMapsByCulture[0x10];
    /// 0x0a30faa0
    extern UNK_DATA cvPopBuildingLibrary[0x14];
    /// 0x0a30fad0
    extern UNK_DATA cvReflectionMaps[0x8];
    /// 0x0a30fb38
    extern UNK_DATA cvRoadFilename[0x4];
    /// 0x0a30fb3c
    extern UNK_DATA cvRoadTypeNames[0x80];
    /// 0x0a30fbe0
    extern UNK_DATA g_aUnitCombatTypeColors[0x50];
    /// 0x0a30fc8c
    extern UNK_DATA g_apfnIconSetters[0x14];
    /// 0x0a30fc68
    extern UNK_DATA g_apfnOverlaySetters[0x24];
    /// 0x0a30fc54
    extern UNK_DATA g_aszIconTypes[0x14];
    /// 0x0a30fc30
    extern UNK_DATA g_aszOverlayTypes[0x24];
    /// 0x0a30fbd8
    extern UNK_DATA g_ppBoneBindingNames[0x8];
}
