/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Mode.h"

/// size ?
/// vtable 0x0a3006e0
class NexusTerrainEditor : public Nexus::Mode {
public:
    /// 0x08b62934
    NexusTerrainEditor();
    /// 0x08b62bbc
    /// deleting 0x08b62b94
    virtual ~NexusTerrainEditor();
    /// 0x08b6456a
    virtual UNK_RET Shutdown();
    /// 0x08b62cca
    virtual UNK_RET Startup();
    /// 0x08b6466c
    virtual UNK_RET Update();
    /// 0x08b636d0
    UNK_RET SendTerrainPieceNames(int a1, int a2);
    /// 0x08b638da
    UNK_RET LoadTerrainPiece(int a1);
    /// 0x08b63ca0
    UNK_RET SendThumbnailsForContinentType(int a1);
    /// 0x08b63dc8
    UNK_RET CreateNewTerrainPiece(char const* a1, char const* a2, bool a3, char const* a4, char const* a5);
    /// 0x08b6461a
    UNK_RET UnloadTerrainPiece();
    /// 0x08b64aa8
    UNK_RET UpdateGlobals();
};
