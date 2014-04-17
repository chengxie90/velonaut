#include <string>
#include <iostream>
#include <vector>

#include "networkserver.h"


using namespace RakNet;
using namespace std;

static void* callHandle(void* data) {
  NetworkServer *h = static_cast<NetworkServer*>(data);
  h->handle();
}

NetworkServer::NetworkServer()
{
}

void NetworkServer::start() {

    pthread_create(&thread_, NULL, &callHandle, static_cast<void*>(this) );

}

void NetworkServer::onClientConnect(Packet* packet) {
    std::cout << "onClientConnect" << std::endl;
    clients_.push_back(packet->guid);

    bsOut.Reset();
    bsOut.Write((MessageID)GAME_INIT);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);

    /*

    bsOut.Write((int)clients_.size());
    bsOut.Reset();
    sleep(3);
    bsOut.Write((MessageID)YOUR_TURN);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
    */

}

void NetworkServer::onClientAlreadyConnected(Packet *packet) {
    cout << "onClientAlreadyConnected" << endl;
}

void NetworkServer::onClientDisconnect(Packet *packet) {
    cout << "onClientDisconnect" << endl;
}

void NetworkServer::onClientConnectionLost(Packet *packet) {
    cout << "onClientConnectionLost" << endl;
}

void NetworkServer::onPlayerReady(Packet *packet) {
    cout << "onPlayerReady" << endl;
    bsOut.Reset();
    bsOut.Write((MessageID)GAME_START);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
}

void NetworkServer::onPlayerUpdate(Packet *packet) {
    cout << "onPlayerUpdate" << endl;
    bsOut.Reset();
    bsOut.Write((MessageID)GAME_UPDATE);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);

    bsOut.Reset();
    bsOut.Write((MessageID)GAME_OVER);
    peer->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
}

void NetworkServer::handle() {

    peer = RakPeerInterface::GetInstance();
    Packet *packet;

    static RakNet::SocketDescriptor desc(SERVER_PORT,0);
    peer->Startup(MAX_CLIENTS, &desc, 1);

    cout << "Starting the server" << endl;

    peer->SetMaximumIncomingConnections(MAX_CLIENTS);

    while (1)
    {

        for (packet=peer->Receive(); packet; peer->DeallocatePacket(packet), packet=peer->Receive())
        {
            BitStream bsIn(packet->data,packet->length,false);
            bsIn.IgnoreBytes(sizeof(MessageID));
            bsIn.Read(rs);

            /*
            std::cout << "Client List:\n";
            for(int i=0; i < (int)clients_.size(); ++i)
            {
                cout << i+1 << " - " << clients_[i].g << endl;
            }
            */

            //cout << "\n\nNew Packet from:" << packet->guid.g << endl;

            switch (packet->data[0])
            {
                case ID_NEW_INCOMING_CONNECTION:
                    onClientConnect(packet);
                    break;
                case ID_ALREADY_CONNECTED:
                    onClientAlreadyConnected(packet);
                    break;
                case ID_DISCONNECTION_NOTIFICATION:
                    onClientDisconnect(packet);
                    break;
                case ID_CONNECTION_LOST:
                    onClientConnectionLost(packet);
                    break;
                case PLAYER_READY:
                    onPlayerReady(packet);
                    break;
                case PLAYER_UPDATE:
                    onPlayerUpdate(packet);
                    break;

                default:
                    cout << "Message with identifier " << packet->data[0] << " has arrived." << endl;
                    break;
            }


            bsOut.Reset();
        }
    }

    RakPeerInterface::DestroyInstance(peer);

}
