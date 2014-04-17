#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <pthread.h>
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/BitStream.h"


#define SERVER_PORT 60000
#define MAX_CLIENTS 10

enum GameMessages
{
    GAME_INIT       = ID_USER_PACKET_ENUM+1,
    GAME_START      = ID_USER_PACKET_ENUM+2,
    GAME_UPDATE     = ID_USER_PACKET_ENUM+3,
    GAME_OVER       = ID_USER_PACKET_ENUM+4,
    PLAYER_READY    = ID_USER_PACKET_ENUM+5,
    PLAYER_UPDATE   = ID_USER_PACKET_ENUM+6
};

class NetworkServer
{
public:
    NetworkServer();
    void start();
    void handle();
    void print();

private:

    void onClientConnect(RakNet::Packet* packet);
    void onClientDisconnect(RakNet::Packet* packet);
    void onClientAlreadyConnected(RakNet::Packet* packet);
    void onClientConnectionLost(RakNet::Packet* packet);
    void onPlayerReady(RakNet::Packet* packet);
    void onPlayerUpdate(RakNet::Packet* packet);

    std::vector<RakNet::RakNetGUID> clients_;
    std::map<unsigned char, std::function<void(NetworkServer&, RakNet::Packet*)> > _callbacks;
    pthread_t thread_;
    RakNet::RakPeerInterface *peer;
    RakNet::BitStream bsOut;
    RakNet::RakString rs;
};

#endif // NETWORKSERVER_H
