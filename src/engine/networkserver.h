#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <pthread.h>
#include <time.h>
#include "RakNet/RakNetTypes.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/BitStream.h"



#define SERVER_PORT 60000
#define MAX_CLIENTS 10

enum GameMessages
{
    GAME_MESSAGE = ID_USER_PACKET_ENUM+1,
    SERVER_REQUEST = ID_USER_PACKET_ENUM+2,
    SERVER_RESPONSE = ID_USER_PACKET_ENUM+3
};

class NetworkServer
{
public:
    NetworkServer( int port);
    void start();
    void shutdown();
    void handle();
    void print();
    void setMaxIncomingConnections( int numCon );

private:

    void onClientConnect(RakNet::Packet* packet);
    void onClientDisconnect(RakNet::Packet* packet);
    void onClientAlreadyConnected(RakNet::Packet* packet);
    void onClientConnectionLost(RakNet::Packet* packet);
    void onGameMessageReceived(RakNet::Packet* packet);
    void onServerRequest(RakNet::Packet* packet);
    void sendToAllExcept(RakNet::BitStream* stream, RakNet::RakNetGUID except );
    void sendToAll(RakNet::BitStream* stream);

    void humbug(RakNet::BitStream* s, RakNet::Packet* p);

private:

    std::vector<RakNet::RakNetGUID> clients_;
    std::map<unsigned char, std::function<void(NetworkServer&, RakNet::Packet*)> > _callbacks;
    pthread_t thread_;
    RakNet::RakPeerInterface *server_;
    RakNet::BitStream bsOut;
    int port_;
    RakNet::RPC4 rpc_;

};


class timer {
    private:
        unsigned long begTime;
    public:
        void restart() {
            begTime = clock();
        }

        double elapsedTime() {
            return ( clock() - begTime) / static_cast<double>(CLOCKS_PER_SEC);
        }

        bool isTimeout(double seconds) {
            return elapsedTime() >= seconds;
        }
};

#endif // NETWORKSERVER_H
