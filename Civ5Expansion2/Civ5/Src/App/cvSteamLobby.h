/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetLobbySteam.h"

class FSteamGameServer;

/// size ?
/// vtable 0x0a2fbe70
class cvSteamLobby : public FNetLobbySteam {
public:
    /// absent
    virtual ~cvSteamLobby();
    /// 0x08739452
    virtual UNK_RET AddServerToServerBrowser(FNetLobbySteam::SEARCH_TYPE a1, FSteamGameServer* a2);
};
