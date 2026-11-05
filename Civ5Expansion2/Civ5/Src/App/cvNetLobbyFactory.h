/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetLobbyFactory.h"

class FNetLobbyCallbacksIFace;
class LobbyType;

/// size ?
/// vtable 0x0a2fbe48
class cvNetLobbyFactory : public FNetLobbyFactory {
public:
    /// 0x0872dae0
    virtual UNK_RET CreateNetLobby(int a1, FNetLobbyCallbacksIFace* a2, LobbyType a3);
};
