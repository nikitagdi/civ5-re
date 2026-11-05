/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetLobbyCallbacksIFace.h"

class FNetLobbyGameListUpdateData;
class FNetLobbyIFace;
class GameListUpdateType;
class LobbyRoomType;
class LoginError;
class NetProxy;
class PingServerError;
class RoomError;

/// size ?
/// vtable 0x0a2fc200
class NetProxyLobbyCallbacks : public FNetLobbyCallbacksIFace {
public:
    /// 0x0873a55a
    NetProxyLobbyCallbacks(NetProxy* a1);
    /// 0x0873cbc4
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x0873cbc6
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x0873cbda
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x0873aae8
    virtual UNK_RET JoinRoomComplete(int a1, LobbyRoomType a2, RoomError a3);
    /// 0x0873a57e
    virtual UNK_RET GameListUpdate(int a1, GameListUpdateType a2, long a3, FNetLobbyGameListUpdateData* a4);
    /// 0x0873b6d0
    virtual UNK_RET DisconnectedCallback(int a1, char const* a2);
    /// 0x0873cbdc
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x0873b6a2
    virtual UNK_RET GetDefaultGameName();
    /// 0x0873cbde
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x0873cbe4
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x0873cc2e
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x0873cc34
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x0873cc7e
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x0873cc84
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x0873ccce
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x0873b6fc
    virtual UNK_RET UpdateGameDetails(int a1);
    /// 0x0873b754
    virtual UNK_RET ServerReportingError(int a1, char const* a2);
    /// 0x0873b7d2
    virtual UNK_RET MasterServerLoginComplete(int a1, LoginError a2);
    /// 0x0873b8bc
    virtual UNK_RET MasterServerDisconnected(int a1, LoginError a2);
    /// 0x0873b93a
    virtual UNK_RET PingServerComplete(unsigned long long a1);
    /// 0x0873b990
    virtual UNK_RET PingServerFailed(PingServerError a1);
};

/// 0x0873a704
UNK_RET RegisterHostedDLC(FNetLobbyIFace* a1, bool a2);
