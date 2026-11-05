/// origin binary
#pragma once

class FNetLobbyCallbacksIFace;
class FNetLobbyFactory;
class FNetProfileCallbacksIFace;
class FNetSessionCallbacksIFace;
class LobbyType;

/// size ?
class FireWireMgr {
public:
    /// 0x08d5c00c
    FireWireMgr();
    /// 0x08d5c3c2
    ~FireWireMgr();
    /// 0x08d5c372
    UNK_RET SetLobbyFactory(FNetLobbyFactory* a1);
    /// 0x08d5c7d4
    UNK_RET SetNetworkSecurity(bool a1);
    /// 0x08d5c7e4
    UNK_RET CreateSession(FNetSessionCallbacksIFace* a1);
    /// 0x08d5c836
    UNK_RET GetSession(int a1);
    /// 0x08d5c880
    UNK_RET FreeSession(int a1);
    /// 0x08d5c8fe
    UNK_RET CreateProfile(FNetProfileCallbacksIFace* a1, int a2);
    /// 0x08d5c95a
    UNK_RET GetProfile(int a1);
    /// 0x08d5c9a6
    UNK_RET FreeProfile(int a1);
    /// 0x08d5ca2c
    UNK_RET CreateLobby(FNetLobbyCallbacksIFace* a1, LobbyType a2);
    /// 0x08d5ca8e
    UNK_RET GetLobby(int a1);
    /// 0x08d5cada
    UNK_RET FreeLobby(int a1);
    /// 0x08d5cb60
    UNK_RET PreRenderStep();
    /// 0x08d5cb8c
    UNK_RET PostRenderStep();
    /// 0x08d5cbb8
    UNK_RET GetUserNameA(unsigned int a1, bool a2);
    /// 0x0ff88564
    static UNK_DATA m_pInst[0x4];
    /// 0x0ff88570
    static UNK_DATA ms_iNextLobbyID[0x4];
    /// 0x0ff8856c
    static UNK_DATA ms_iNextProfileID[0x4];
    /// 0x0ff88568
    static UNK_DATA ms_iNextSessionID[0x4];
};

extern "C" {
    /// 0x0ff888a8
    extern UNK_DATA s_sendAddressesSinceLastRecv[0xc];
}
