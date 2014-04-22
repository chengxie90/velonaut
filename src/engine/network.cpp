#include "network.h"

#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"  // MessageID
#include "RakNet/StringCompressor.h"
#include "luamanager.h"
#include "app.h"

using namespace std;
using namespace RakNet;

Network::Network()
{
}

void Network::initLua() {

    LuaManager::GetInstance()->requiref("engine.network.c",[](lua_State* state) {
        luaL_Reg reg[] = {
            {"startServer", Network::lStartServer},
            {"shutdownServer", Network::lShutdownServer},
            {"shutdownClient", Network::lShutdownClient},
            {"connectToServer", Network::lConnectToServer},
            {"sendMessage", Network::lSendMessage},
            {"sendRequest", Network::lSendRequest},
            {"addEventListener", Network::lAddEventListener},
            {"setMaxIncomingConnections", Network::lSetMaxIncomingConnections},
            {NULL, NULL}
        };
        LuaManager::GetInstance()->addlib(reg);
        return 1;
    } );
}

int Network::lConnectToServer(lua_State* state) {

    string serverAdress;
    int port;
    LuaManager::GetInstance()->extractParam(&serverAdress);
    LuaManager::GetInstance()->extractParam(&port);

    Network::GetInstance()->connectToServer(serverAdress.c_str(), port);
    return 0;

}

int Network::lStartServer(lua_State* state) {
    int port;
    LuaManager::GetInstance()->extractParam(&port);
    Network::GetInstance()->startServer( port );
    return 0;
}

int Network::lShutdownServer(lua_State* state) {
    Network::GetInstance()->shutdownServer();
    return 0;
}

int Network::lShutdownClient(lua_State* state) {
    Network::GetInstance()->shutdownClient();
    return 0;
}

int Network::lSendRequest(lua_State* state) {
    string request;
    LuaManager::GetInstance()->extractParam(&request);
    Network::GetInstance()->sendRequest(request);
    return 0;
}

int Network::lSendMessage(lua_State* state) {
    string serverAdress;
    LuaManager::GetInstance()->extractParam(&serverAdress);
    Network::GetInstance()->sendMessage(serverAdress);
    return 0;
}

int Network::lAddEventListener(lua_State *state) {

    std::string event;    
    LuaManager::GetInstance()->extractParam(&event);
    int r = luaL_ref(state, LUA_REGISTRYINDEX);
    Network::GetInstance()->listenerMap_[event].push_back(r);
}

int Network::lSetMaxIncomingConnections(lua_State *state) {
    int numConnections;
    LuaManager::GetInstance()->extractParam(&numConnections);
    Network::GetInstance()->server_->setMaxIncomingConnections(numConnections);
}

Network *Network::GetInstance()
{
    return App::GetApp()->GetNetwork();
}

void Network::init() {
    client_ = RakPeerInterface::GetInstance();
    client_->AttachPlugin(&rpc_);
}

void Network::shutdown() {
    shutdownClient();
    shutdownServer();
}

void Network::startServer( int port) {
    server_ = new NetworkServer(port);
    server_->start();
}

void Network::shutdownServer() {
    server_->shutdown();
}

void Network::shutdownClient() {
    client_->Shutdown(300);
}

void Network::sendMessage(string msg) {
    bsOut.Reset();
    bsOut.Write((MessageID)GAME_MESSAGE);
    RakNet::StringCompressor compressor;
    compressor.EncodeString(msg.c_str(), msg.size()+1, &bsOut);
    client_->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, serverAddress_,false);
}

void Network::sendRequest(string req) {
    bsOut.Reset();
    bsOut.Write((MessageID)SERVER_REQUEST);
    // RakNet::StringCompressor compressor;
    // compressor.EncodeString(req.c_str(), req.size()+1, &bsOut);

    RakNet::BitStream bsIn;

    std::cout << "sending request" << std::endl;
    rpc_.Call("humbug", &bsOut, HIGH_PRIORITY,RELIABLE_ORDERED, 0, serverAddress_,false);

    // client_->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0, serverAddress_,false);
}

void Network::connectToServer(const char *serverAdress, int port) {

    static SocketDescriptor desc;
    client_->Startup(1, &desc, 1);

    std::cout<< "Starting the client" << std::endl;
    client_->Connect(serverAdress, port, 0,0);
}

void Network::onConnectionAccepted(Packet *packet) {
    serverAddress_ = packet->systemAddress;
    fireEvent("connected_to_server");
}

void Network::onConnectionFailed(Packet *packet) {
    fireEvent("connection_failed");
}

void Network::onDisconnect(Packet *packet) {
    fireEvent("disconnect");
}

void Network::onServerResponse(Packet *packet) {
    RakNet::RakString rs;
    BitStream bsIn(packet->data,packet->length,false);
    bsIn.IgnoreBytes(sizeof(MessageID));
    RakNet::StringCompressor compressor;
    compressor.DecodeString(&rs, 1000, &bsIn);
    fireEvent("server_response", string(rs.C_String()));
}

void Network::onGameMessageReceived(Packet *packet) {
    RakNet::RakString rs;
    BitStream bsIn(packet->data,packet->length,false);
    bsIn.IgnoreBytes(sizeof(MessageID));
    RakNet::StringCompressor compressor;
    compressor.DecodeString(&rs, 1000, &bsIn);
    fireEvent("game_message", string(rs.C_String()));
}

void Network::fireEvent(string event) {
    for (int i = 0; i < listenerMap_[event].size(); ++i) {
        lua_rawgeti(LuaManager::GetInstance()->state(), LUA_REGISTRYINDEX, listenerMap_[event][i]);
        LuaManager::GetInstance()->pCall();
    }
}

void Network::fireEvent(string event, string params) {
    params.insert(0, "return ");
    for (int i = 0; i < listenerMap_[event].size(); ++i) {
        lua_rawgeti(LuaManager::GetInstance()->state(), LUA_REGISTRYINDEX, listenerMap_[event][i]);
        luaL_dostring(LuaManager::GetInstance()->state(), params.c_str());
        LuaManager::GetInstance()->pCall(1);
    }
}

void Network::poll() {

    if (!client_)
        return;

    Packet* packet;
    for (packet=client_->Receive(); packet; client_->DeallocatePacket(packet), packet=client_->Receive())
    {
        switch (packet->data[0])
        {
            case ID_CONNECTION_REQUEST_ACCEPTED:
              onConnectionAccepted(packet);
              break;
            case ID_CONNECTION_ATTEMPT_FAILED:
              onConnectionFailed(packet);
              break;
            case ID_DISCONNECTION_NOTIFICATION:
            case ID_CONNECTION_LOST:
              onDisconnect(packet);
              break;
            case GAME_MESSAGE:
              onGameMessageReceived(packet);
              break;
            case SERVER_RESPONSE:
              onServerResponse(packet);
              break;
            default:
                std::cout << "Message with identifier " << packet->data[0] << " has arrived." << std::endl;
              break;
        }
    }
}



