/// origin guess
#pragma once
#include <enet.h>
#include <steam.h>

class FInetAddress;
class FNetAccessENet;
class FNetCallbackIFaceENet;
class P2PSessionConnectFail_t;
class P2PSessionRequest_t;
class TransportType;

/// size ?
class FNetAccessENet {
public:
    /// 0x08d66a88
    FNetAccessENet(FNetCallbackIFaceENet* a1);
    /// 0x08d66e6c
    ~FNetAccessENet();
    /// 0x08d66be6
    UNK_RET OnSteamP2PSessionRequest(P2PSessionRequest_t* a1);
    /// 0x08d66ce6
    UNK_RET OnSteamP2PSessionConnectFail(P2PSessionConnectFail_t* a1);
    /// 0x08d66e44
    UNK_RET OnSteamGameServerP2PSessionRequest(P2PSessionRequest_t* a1);
    /// 0x08d66e58
    UNK_RET OnSteamGameServerP2PSessionConnectFail(P2PSessionConnectFail_t* a1);
    /// 0x08d66f92
    UNK_RET CloseAll(bool a1);
    /// 0x08d673a4
    UNK_RET Create(TransportType a1, unsigned short a2, bool a3, unsigned int a4);
    /// 0x08d674b0
    UNK_RET GetDefaultMessageFragmentationSize(TransportType a1);
    /// 0x08d674d4
    UNK_RET Connect(FInetAddress const& a1, long& a2, unsigned char const* a3, unsigned int a4);
    /// 0x08d675b0
    UNK_RET GetConnectionID(ENetPeer* a1) const;
    /// 0x08d675be
    UNK_RET RegisterAccessObject(long a1, FNetAccessENet* a2);
    /// 0x08d6761e
    UNK_RET Connect(CSteamID a1, long& a2, unsigned char const* a3, unsigned int a4);
    /// 0x08d67906
    UNK_RET CreateConnectionID(CSteamID a1);
    /// 0x08d67986
    UNK_RET Send(long a1, unsigned char const* a2, unsigned int a3, bool a4);
    /// 0x08d67b12
    UNK_RET GetSteamID(long a1) const;
    /// 0x08d67b7e
    UNK_RET GetENetPeer(long a1) const;
    /// 0x08d67bc4
    UNK_RET Close(long a1, bool a2);
    /// 0x08d67dc6
    UNK_RET ClosedCallbackImpl(long a1);
    /// 0x08d67e24
    UNK_RET RemoveConnectionID(long a1);
    /// 0x08d67e9c
    UNK_RET Update();
    /// 0x08d68c8a
    UNK_RET GetConnectionID(CSteamID a1) const;
    /// 0x08d68d70
    UNK_RET ReceivedCallbackImpl(long a1, unsigned char const* a2, unsigned int a3, bool a4);
    /// 0x08d68dc6
    UNK_RET ConnectedCallbackImpl(long a1, unsigned char const* a2, unsigned int a3, bool a4);
    /// 0x08d68ec4
    UNK_RET ConnectedCallbackImpl(long a1, ENetPacket* a2, bool a3);
    /// 0x08d68f22
    UNK_RET ReceivedCallbackImpl(long a1, ENetPacket* a2);
    /// 0x08d68f88
    UNK_RET IsOnline() const;
    /// 0x08d68ff2
    UNK_RET GetLocalAddress() const;
    /// 0x08d6907a
    UNK_RET GetLocalIP() const;
    /// 0x08d69094
    UNK_RET GetLocalPort() const;
    /// 0x08d690b4
    UNK_RET GetLocalSteamID() const;
    /// 0x08d69106
    UNK_RET ReleaseAccessObject(long a1);
    /// 0x08d69170
    UNK_RET GetAccessObject(long a1);
    /// 0x08d691c0
    UNK_RET ReleaseAccessObject(FNetAccessENet* a1);
    /// 0x08d6925e
    UNK_RET GetPingTime(long a1);
    /// 0x08d692d0
    UNK_RET CheckPingTimes();
    /// 0x08d69308
    UNK_RET SetDefaultMessageFragmentationSize(TransportType a1, unsigned int a2);
    /// 0x08d6932c
    UNK_RET ServerStartup();
    /// 0x08d693a4
    UNK_RET ServerShutdown();
    /// 0x08d6940c
    UNK_RET AuthorizationComplete(long a1, bool a2, unsigned char const* a3, unsigned int a4);
    /// 0x0ff89cb0
    static UNK_DATA m_Connections[0xc];
    /// 0x0a312eb4
    static UNK_DATA m_uiDefaultMessageFragmentationSize[0xc];
    /// 0x0a312ec0
    static UNK_DATA ms_NextSteamConnectionID[0x4];
    /// 0x0ff89cbc
    static UNK_DATA ms_SteamConnections[0xc];
};

extern "C" {
    /// 0x0ff89d78
    extern UNK_DATA gFSteamTransportLogPackets[0x1];
}
