#include "network.h"

#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"  // MessageID

using namespace std;
using namespace RakNet;

Network::Network()
{
}

void Network::init() {

}

void Network::startClient() {
    peer = RakPeerInterface::GetInstance();
    Packet *packet;

    static SocketDescriptor desc;

    peer->Startup(1, &desc, 1);

    std::cout<< "Starting the client" << std::endl;
    peer->Connect("127.0.0.1", SERVER_PORT, 0,0);
}

void Network::onConnectionAccepted(Packet *packet) {
    cout << "onConnectionAccepted" << endl;
}

void Network::onGameInit(Packet *packet) {
    cout << "onGameInit" << endl;
    bsOut.Reset();
    bsOut.Write((MessageID)PLAYER_READY);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
}

void Network::onGameStart(Packet *packet) {
    cout << "onGameStart" << endl;
    bsOut.Reset();
    bsOut.Write((MessageID)PLAYER_UPDATE);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
}

void Network::onGameUpdate(Packet *packet) {
    cout << "onGameUpdate" << endl;
}


void Network::onGameOver(Packet *packet) {
    cout << "onGameOver " << endl;
}

void Network::poll() {

    Packet* packet;
    int int_message;
    for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
    {
        RakNet::BitStream bsIn(packet->data,packet->length,false);
        bsIn.IgnoreBytes(sizeof(MessageID));
        bsIn.Read(int_message);

        switch (packet->data[0])
        {
            case ID_CONNECTION_REQUEST_ACCEPTED:
              onConnectionAccepted(packet);
              break;
            case GAME_INIT:
              onGameInit(packet);
              break;
            case GAME_START:
              onGameStart(packet);
              break;
            case GAME_UPDATE:
              onGameUpdate(packet);
              break;
            case GAME_OVER:
              onGameOver(packet);
              break;

            default:
                std::cout << "Message with identifier " << packet->data[0] << " has arrived." << std::endl;
              break;
        }
    }
}

void Network::startServer() {

    server_ = new NetworkServer();
    server_->start();
}


