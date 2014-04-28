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

NetworkServer::NetworkServer():currentState_(0), thread_(0), isRunning_(false) {
    server_ = RakPeerInterface::GetInstance();
    server_->AttachPlugin(&rpc_);

    rpc_.RegisterFunction("setPlayerName", &NetworkServer::rpcSetPlayerName);
    rpc_.RegisterFunction("setPlayerReady", &NetworkServer::rpcSetPlayerReady);
    rpc_.RegisterFunction("setNumPlayers", &NetworkServer::rpcSetNumPlayers);
    rpc_.RegisterFunction("startGame", &NetworkServer::rpcStartGame);
}

void NetworkServer::rpcSetPlayerName(BitStream* bsIn,Packet* p)
{
    NetworkServer* srv = NetworkServer::GetInstance();
    RakString rs;
    srv->readString(bsIn, rs, false);

    string clientIp = srv->server_->GetSystemAddressFromGuid(p->guid).ToString(false);

    auto& player = srv->players_[p->guid];
    player.name = string(rs.C_String());
    player.guid = p->guid;
    player.ipAdress = clientIp;

    if ( clientIp.compare("127.0.0.1") == 0) {
        player.isServer = true;
        string serverAddress = srv->server_->GetSystemAddressFromGuid(srv->server_->GetMyGUID()).ToString(false);
        player.ipAdress = serverAddress;
    }
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
    server->players_[p->guid].isReady = true;
}

void NetworkServer::rpcSetNumPlayers(BitStream* bsIn,Packet* p)
{
    NetworkServer* server = NetworkServer::GetInstance();
    RakString rs;
    server->readString(bsIn, rs, false);
    server->numPlayers_ = atoi(rs.C_String());
    server->setMaxIncomingConnections(server->numPlayers_);
    std::cout << "setting max incoming to: " << server->numPlayers_ << std::endl;
}

void NetworkServer::start(int port)
{
    reset();

    port_ = port;
    isRunning_ = true;

    server_ = RakPeerInterface::GetInstance();
    server_->AttachPlugin(&rpc_);

    pthread_create(&thread_, NULL, &callRun, static_cast<void*>(this) );
}

void NetworkServer::reset() {
    clients_.clear();
    players_.clear();
    bitSteamOut_.Reset();
    currentState_ = NULL;
    port_ = 0;
    isRunning_ = false;
    numPlayers_ = 0;
}

void NetworkServer::shutdown()
{
    if (isRunning_)
        isRunning_ = false;
}

void NetworkServer::setMaxIncomingConnections(int numCon)
{
    server_->SetMaximumIncomingConnections(numCon);
}

void NetworkServer::onClientConnect(Packet* packet)
{
    std::cout << "onClientConnect" << std::endl;
    clients_.push_back(packet->guid);

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

RakString NetworkServer::createPlayerListEvent()
{
    std::string hostname;
    std::string hostip;

    for(auto it = players_.begin(); it != players_.end(); ++it) {

        if (it->second.isServer) {
            hostname = it->second.name;
            hostip = it->second.ipAdress;
        }
    }

    string s;
    s.append("{ eventType='playerlist', ");
    s.append("host='");
    s.append(hostname);
    s.append("'");
    s.append(", hostip='");
    s.append(hostip);
    s.append("'");
    s.append(", minPlayers='");
    s.append(to_string(numPlayers_));
    s.append("'");
    s.append(", players={");

    for(auto it = players_.begin(); it != players_.end(); ++it) {
        s.append("{name='");
        s.append(it->second.name);
        s.append("', id='");
        s.append(to_string(it->second.guid.g));

        s.append("'},");


    }
    s.append("} }");
    return RakString(s.c_str());
}

RakString NetworkServer::createWelcomeEvent(RakNet::RakNetGUID guid)
{
    string s;
    s.append("{eventType='welcome',id='");
    s.append(to_string(guid.g));
    s.append("'}");
    return RakString(s.c_str());
}

RakString NetworkServer::createGameInitEvent(uint64_t guid, int seed)
{
    string s;
    s.append("{eventType='gameinit',seed=");
    s.append(to_string(123));
    s.append("}");
    return RakString(s.c_str());
}

RakString NetworkServer::createCountDownEvent(int count)
{
    string s;
    s.append("{eventType='countdown',count=");
    s.append(to_string(count));
    s.append("}");
    return RakString(s.c_str());
}

RakString NetworkServer::createGameStartEvent()
{
    string s;
    s.append("{eventType='gamestart'}");
    return RakString(s.c_str());
}

void NetworkServer::setServerState(AbstractServerState *state)
{
    if (currentState_)
        delete currentState_;
    currentState_ = state;
}

void NetworkServer::run()
{

    static SocketDescriptor desc(port_,0);
    server_->Startup(12, &desc, 1);
    setServerState(new WaitingForPlayersState);

    timer t;
    t.restart();

    while (isRunning_)
    {
        if (currentState_)
            currentState_->update(this, t);
        pollPackets();
    }

    shutdown();

    RakPeerInterface::DestroyInstance(server_);
}


void NetworkServer::pollPackets()
{
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

void NetworkServer::WaitingForPlayersState::update(NetworkServer* server, timer& t)
{
    if (!t.isTimeout(1.0))
        return;

    server->writeMessage(GAME_MESSAGE, server->createPlayerListEvent());
    server->sendToAll(RELIABLE_ORDERED);

    if (server->players_.size() >= server->numPlayers_) {
        server->setServerState(new WaitingForPlayerReadyState);

        server->writeMessage(GAME_MESSAGE, server->createGameInitEvent(123, 0));
        server->sendToAll(RELIABLE_ORDERED);

        server->setServerState(new WaitingForPlayerReadyState);
        return;
    }


    t.restart();
}

void NetworkServer::WaitingForPlayerReadyState::update(NetworkServer* server, timer& t)
{
    if (!t.isTimeout(2.0))
        return;

    for(auto it = server->players_.begin(); it != server->players_.end(); ++it) {
        if (!it->second.isReady) {
            t.restart();
            return;
        }
    }
    server->countdown_.restart();
    server->setServerState(new CountDownState);
}

void NetworkServer::CountDownState::update(NetworkServer* server, timer& t)
{
    if (!t.isTimeout(1))
        return;

        int count = 2-server->countdown_.elapsedTime();

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
