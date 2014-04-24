#include "network.h"

#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakNetTypes.h"
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
            {"RPC", Network::lRpc},
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

int Network::lRpc(lua_State* state) {
    string func;
    string params;
    LuaManager::GetInstance()->extractParam(&func);
    LuaManager::GetInstance()->extractParam(&params);
    Network::GetInstance()->rpc(func, params);
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
    server_ = NetworkServer::GetInstance();
    server_->start(port);
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
    StringCompressor compressor;
    compressor.EncodeString(msg.c_str(), msg.size()+1, &bsOut);
    client_->Send(&bsOut,HIGH_PRIORITY,UNRELIABLE_SEQUENCED,0, serverAddress_,false);
}

void Network::rpc(string req, string params) {
    bsOut.Reset();
    StringCompressor compressor;
    compressor.EncodeString(params.c_str(), params.size()+1, &bsOut);
    std::cout << "sending rpc" << std::endl;
    rpc_.Call(req.c_str(), &bsOut, HIGH_PRIORITY,RELIABLE_ORDERED, 0, serverAddress_,false);
}

void Network::onServerPong(Packet *packet) {
    std::cout << "onServerPong" << std::endl;
    RakNet::TimeMS time;
    RakNet::BitStream bsIn(packet->data,packet->length,false);
    bsIn.IgnoreBytes(1);
    bsIn.Read(time);
    cout << "Got pong from " << packet->systemAddress.ToString() << std::endl;
}

void Network::connectToServer(const char *serverAdress, int port) {

    static SocketDescriptor desc;
    client_->Startup(1, &desc, 1);
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

void Network::onGameMessageReceived(Packet *packet) {
    RakString rs;
    BitStream bsIn(packet->data,packet->length,false);
    bsIn.IgnoreBytes(sizeof(MessageID));
    StringCompressor compressor;
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
            case ID_UNCONNECTED_PONG:
                onServerPong(packet);
                break;
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
            default:
                std::cout << "Message with identifier " << packet->data[0] << " has arrived." << std::endl;
              break;
        }
    }
}



