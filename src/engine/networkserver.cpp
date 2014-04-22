#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "RakNet/StringCompressor.h"
#include "RakNet/RPC4Plugin.h"
#include "networkserver.h"

using namespace RakNet;
using namespace std;

static void* callHandle(void* data) {
  NetworkServer *h = static_cast<NetworkServer*>(data);
  h->handle();
}

NetworkServer::NetworkServer(int port):port_(port)
{
}

void NetworkServer::start() {
    server_ = RakPeerInterface::GetInstance();
    server_->AttachPlugin(&rpc_);

    rpc_.RegisterFunction("humbug", [](RakNet::BitStream* s, Packet* p) {
        std::cout << "RPC CALL humbug" << std::endl;
    });

    pthread_create(&thread_, NULL, &callHandle, static_cast<void*>(this) );
}

void NetworkServer::shutdown() {
    server_->Shutdown(300);
}

void NetworkServer::setMaxIncomingConnections(int numCon) {
    server_->SetMaximumIncomingConnections(numCon);
}

void NetworkServer::onClientConnect(Packet* packet) {
    std::cout << "onClientConnect" << std::endl;
    clients_.push_back(packet->guid);
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

void NetworkServer::onServerRequest(Packet *packet) {
    std::cout << "Server::onServerRequest" << std::endl;
    RakNet::RakString rs;
    BitStream bsIn(packet->data,packet->length,false);
    bsIn.IgnoreBytes(sizeof(MessageID));
    RakNet::StringCompressor compressor;
    compressor.DecodeString(&rs, 1000, &bsIn);
}

void NetworkServer::onGameMessageReceived(Packet *packet) {
    RakNet::RakString rs;
    BitStream bsIn(packet->data,packet->length,false);
    bsIn.IgnoreBytes(sizeof(MessageID));
    RakNet::StringCompressor compressor;
    compressor.DecodeString(&rs, 1000, &bsIn);

    bsOut.Reset();
    bsOut.Write((MessageID)GAME_MESSAGE);
    compressor.EncodeString(&rs, rs.GetLength()+1, &bsOut );
    // TODO: directly forward encoded message instead of decoding and encoding
    sendToAllExcept(&bsOut, packet->guid);
}

void NetworkServer::sendToAllExcept(BitStream *stream, RakNetGUID except) {
    for(int i = 0; i < clients_.size(); i++) {
        server_->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, server_->GetSystemAddressFromGuid(clients_[i]), false);
    }
}

void NetworkServer::sendToAll(BitStream *stream) {
    for(int i = 0; i < clients_.size(); i++) {
        server_->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, server_->GetSystemAddressFromGuid(clients_[i]), false);
    }
}

void NetworkServer::handle() {

    Packet *packet;
    static RakNet::SocketDescriptor desc(port_,0);
    server_->Startup(MAX_CLIENTS, &desc, 1);
    cout << "Starting the server" << endl;

    timer t;
    t.restart();

    while (1)
    {
        /*
        if (t.isTimeout(2.0)) {

            RakNet::StringCompressor compressor;
            bsOut.Reset();
            bsOut.Write((MessageID)SERVER_RESPONSE);
            string resp = "{ 'Player 1', 'Player 2', 'Player 3' }";
            compressor.EncodeString( resp.c_str(), resp.size()+1, &bsOut );
            // TODO: directly forward encoded message instead of decoding and encoding
            sendToAll(&bsOut);

            t.restart();
        }
        */

        for (packet=server_->Receive(); packet; server_->DeallocatePacket(packet), packet=server_->Receive())
        {
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
                case GAME_MESSAGE:
                    onGameMessageReceived(packet);
                    break;                    
                case SERVER_REQUEST:
                    onServerRequest(packet);
                    break;

                default:
                    cout << "Message with identifier " << packet->data[0] << " has arrived." << endl;
                    break;
            }


            bsOut.Reset();
        }
    }

    RakPeerInterface::DestroyInstance(server_);

}
