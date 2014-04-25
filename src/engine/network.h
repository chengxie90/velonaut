#ifndef NETWORK_H
#define NETWORK_H

#include <map>
#include <lua/lua.hpp>
#include "common.h"

#include "RakNet/RakPeerInterface.h"
#include "RakNet/RPC4Plugin.h"
#include "networkserver.h"

class Network
{
public:

    void init();
    void initLua();
    void shutdown();
    void startServer(int port);
    void findServer(int port);
    void shutdownServer();
    void shutdownClient();
    void connectToServer( const char* serverAdress, int port);
    void sendMessage(std::string msg);
    void rpc(std::string req, std::string params);
    void poll();

    static Network* GetInstance();
private:
    SINGLETON(Network)
    friend class App;

    void onServerPong(RakNet::Packet* packet);
    void onConnectionAccepted(RakNet::Packet* packet);
    void onConnectionFailed(RakNet::Packet* packet);
    void onDisconnect(RakNet::Packet* packet);
    void onGameMessageReceived(RakNet::Packet* packet);
    void fireEvent(std::string event);
    void fireEvent(std::string event, std::string eventargs);

    static int lConnectToServer(lua_State* state);
    static int lStartServer(lua_State* state);
    static int lShutdownServer(lua_State* state);
    static int lShutdownClient(lua_State* state);
    static int lSendMessage(lua_State* state);
    static int lRpc(lua_State* state);
    static int lAddEventListener(lua_State* state);
    static int lSetMaxIncomingConnections(lua_State* state);
    static int lFindServer(lua_State* state);

private:

    NetworkServer* server_;
    RakNet::RakPeerInterface *client_;
    RakNet::SystemAddress serverAddress_;
    RakNet::BitStream bsOut;
    std::map<std::string, std::vector<int> > listenerMap_;
    RakNet::RPC4 rpc_;

};

#endif // NETWORK_H
