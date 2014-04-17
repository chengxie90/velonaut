#ifndef NETWORK_H
#define NETWORK_H

#include <lua/lua.hpp>
#include "common.h"

#include "RakNet/RakPeerInterface.h"
#include "networkserver.h"

class Network
{
public:

    void init();
    void initLua();
    void startServer();
    void startClient();
    void poll();

    static Network* GetInstance();
private:
    SINGLETON(Network)

    void onConnectionAccepted(RakNet::Packet* packet);
    void onGameInit(RakNet::Packet* packet);
    void onGameStart(RakNet::Packet* packet);
    void onGameUpdate(RakNet::Packet* packet);
    void onGameOver(RakNet::Packet* packet);
    friend class App;
    NetworkServer* server_;
    RakNet::RakPeerInterface *peer;
    RakNet::BitStream bsOut;

};

#endif // NETWORK_H
