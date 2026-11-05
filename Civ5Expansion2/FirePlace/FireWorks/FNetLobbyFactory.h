/// origin guess
#pragma once

class FNetLobbyCallbacksIFace;
class LobbyType;

/// size ?
/// vtable 0x0a303f60
class FNetLobbyFactory {
public:
    /// 0x08d4af1e
    virtual UNK_RET CreateNetLobby(int a1, FNetLobbyCallbacksIFace* a2, LobbyType a3);
};
