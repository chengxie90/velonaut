#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include "RakNet/RPC4Plugin.h"
#include "networkserver.h"

using namespace RakNet;
using namespace std;


static void* callRun(void* data)
{
  NetworkServer *server = static_cast<NetworkServer*>(data);
  server->run();
}

NetworkServer *NetworkServer::GetInstance()
{
    static NetworkServer instance;
    return &instance;
}

NetworkServer::NetworkServer():currentState_(0) {}

void NetworkServer::rpcSetPlayerName(BitStream* bsIn,Packet* p)
{
    NetworkServer* server = NetworkServer::GetInstance();
    RakString rs;
    server->readString(bsIn, rs, false);
    NetworkServer::GetInstance()->players_[p->guid].name = string(rs.C_String());
    NetworkServer::GetInstance()->players_[p->guid].guid = p->guid;
}

void NetworkServer::rpcStartGame(BitStream* bsIn,Packet* p)
{
    NetworkServer* server = NetworkServer::GetInstance();

    server->setServerState(new WaitingForPlayerReadyState);

    server->writeMessage(GAME_MESSAGE, server->createGameInitEvent(123, 0));
    server->sendToAll(RELIABLE_ORDERED);

    server->setServerState(new WaitingForPlayerReadyState);
}

void NetworkServer::rpcSetPlayerReady(BitStream* bsIn,Packet* p)
{
    NetworkServer* server = NetworkServer::GetInstance();
    server->players_[p->guid].ready = true;
}

void NetworkServer::start(int port)
{
    port_ = port;
    server_ = RakPeerInterface::GetInstance();
    server_->AttachPlugin(&rpc_);

    rpc_.RegisterFunction("setPlayerName", &NetworkServer::rpcSetPlayerName);
    rpc_.RegisterFunction("setPlayerReady", &NetworkServer::rpcSetPlayerReady);
    rpc_.RegisterFunction("startGame", &NetworkServer::rpcStartGame);
    pthread_create(&thread_, NULL, &callRun, static_cast<void*>(this) );
}

void NetworkServer::shutdown()
{
    server_->Shutdown(300);
}

void NetworkServer::setMaxIncomingConnections(int numCon)
{
    server_->SetMaximumIncomingConnections(numCon);
}

void NetworkServer::onClientConnect(Packet* packet)
{
    std::cout << "onClientConnect" << std::endl;
    clients_.push_back(packet->guid);
    std::cout << packet->guid.g << std::endl;

    writeMessage(GAME_MESSAGE, createWelcomeEvent(packet->guid));
    sendToOne(packet->guid, RELIABLE_ORDERED);

}

void NetworkServer::onClientAlreadyConnected(Packet *packet)
{
    cout << "onClientAlreadyConnected" << endl;
}

void NetworkServer::onClientDisconnect(Packet *packet)
{
    cout << "onClientDisconnect" << endl;
    players_.erase(packet->guid);
}

void NetworkServer::onClientConnectionLost(Packet *packet)
{
    cout << "onClientConnectionLost" << endl;
    players_.erase(packet->guid);
}

void NetworkServer::onGameMessageReceived(Packet *packet)
{
    RakString rs;
    BitStream bsIn(packet->data,packet->length,false);
    readString(&bsIn, rs);
    writeMessage(GAME_MESSAGE, rs);
    sendToAll(UNRELIABLE_SEQUENCED);
}

void NetworkServer::readString(BitStream *bsIn, RakString &str, bool ignoreMsgType)
{
    static StringCompressor compressor;
    if (ignoreMsgType)
        bsIn->IgnoreBytes(sizeof(MessageID));

    compressor.DecodeString(&str, 1000, bsIn);
}

void NetworkServer::writeMessage(GameMessages msgType, RakString msg)
{
    bitSteamOut_.Reset();
    bitSteamOut_.Write((MessageID)msgType);
    writeString(msg);
}

void NetworkServer::writeString(RakString msg)
{
    static StringCompressor compressor;
    compressor.EncodeString(&msg, msg.GetLength()+1, &bitSteamOut_ );
}

void NetworkServer::sendToAllExcept(BitStream *stream, RakNetGUID except, PacketReliability reliability)
{
    for(int i = 0; i < clients_.size(); i++) {
        if (clients_[i] != except)
            server_->Send(&bitSteamOut_,HIGH_PRIORITY,reliability,0, server_->GetSystemAddressFromGuid(clients_[i]), false);
    }
}

void NetworkServer::sendToAll(PacketReliability reliability)
{
    for(int i = 0; i < clients_.size(); i++) {
        server_->Send(&bitSteamOut_,HIGH_PRIORITY,reliability,0, server_->GetSystemAddressFromGuid(clients_[i]), false);
    }
}

void NetworkServer::sendToOne(RakNet::RakNetGUID &guid, PacketReliability reliability)
{
    server_->Send(&bitSteamOut_,HIGH_PRIORITY,reliability,0, server_->GetSystemAddressFromGuid(guid), false);
}

RakString NetworkServer::createPlayerListEvent() {
    string s;
    s.append("{ eventType = 'playerlist', players = {");


    for(auto it = players_.begin(); it != players_.end(); ++it) {
        s.append("{name='");
        s.append(it->second.name);
        s.append("', id='");
        s.append(to_string(it->second.guid.g));
        s.append("'},");
    }
    s.append("} }");

    cout << s << endl;
    return RakString(s.c_str());
}

RakString NetworkServer::createWelcomeEvent(RakNet::RakNetGUID guid) {
    std::cout << "creating welcome " << guid.g << std::endl;
    string s;
    s.append("{eventType='welcome',id=");
    s.append("");
    s.append("}");
    return RakString(s.c_str());
}

RakString NetworkServer::createGameInitEvent(uint64_t guid, int seed) {
    string s;
    s.append("{eventType='gameinit',seed=");
    s.append(to_string(123));
    s.append("}");
    return RakString(s.c_str());
}

RakString NetworkServer::createCountDownEvent(int count) {
    string s;
    s.append("{eventType='countdown',count=");
    s.append(to_string(count));
    s.append("}");
    return RakString(s.c_str());
}

RakString NetworkServer::createGameStartEvent() {
    string s;
    s.append("{eventType='gamestart'}");
    return RakString(s.c_str());
}

void NetworkServer::setServerState(AbstractServerState *state) {
    if (currentState_)
        delete currentState_;
    currentState_ = state;
}

void NetworkServer::run() {

    static SocketDescriptor desc(port_,0);
    server_->Startup(12, &desc, 1);
    setServerState(new WaitingForPlayersState);

    timer t;
    t.restart();

    while (1)
    {
        if (currentState_)
            currentState_->update(this, t);
        pollPackets();
    }

    shutdown();

    RakPeerInterface::DestroyInstance(server_);
}


void NetworkServer::pollPackets() {
    Packet *packet;

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

            default:
                cout << "Message with identifier " << packet->data[0] << " has arrived." << endl;
                break;
        }

        bitSteamOut_.Reset();
    }
}

void NetworkServer::WaitingForPlayersState::update(NetworkServer* server, timer& t) {
    if (!t.isTimeout(1.0))
        return;

    server->writeMessage(GAME_MESSAGE, server->createPlayerListEvent());
    server->sendToAll(RELIABLE_ORDERED);

    t.restart();
}

void NetworkServer::WaitingForPlayerReadyState::update(NetworkServer* server, timer& t) {
    if (!t.isTimeout(2.0))
        return;

    for(auto it = server->players_.begin(); it != server->players_.end(); ++it) {
        if (!it->second.ready) {
            t.restart();
            return;
        }
    }
    server->countdown_.restart();
    server->setServerState(new CountDownState);
}

void NetworkServer::CountDownState::update(NetworkServer* server, timer& t) {
    if (!t.isTimeout(1))
        return;

        int count = 5-server->countdown_.elapsedTime();

        if (count == 0) {

            server->writeMessage(GAME_MESSAGE, server->createGameStartEvent() );
            server->sendToAll(RELIABLE_ORDERED);
            server->setServerState(0);
            return;

        }
        server->writeMessage(GAME_MESSAGE, server->createCountDownEvent(count));
        server->sendToAll(RELIABLE_ORDERED);


    t.restart();
}
