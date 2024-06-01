#ifndef LOBBYNODE_HPP
#define LOBBYNODE_HPP

#define MAX_PLAYERS 10
#define FD_SETSIZE (MAX_PLAYERS)

#include <winsock2.h>
#include <ws2tcpip.h>

namespace Server {
    struct UDP_SET {
		int size;
		sockaddr array[MAX_PLAYERS];
	};

    class LobbyNode {
    public:
        LobbyNode(LobbyNode *n = nullptr);

        ~LobbyNode();
        
        void Add(const SOCKET &newConnTCP, const sockaddr &newConnUDP);
        void Remove(const SOCKET &newConnTCP, const sockaddr &newConnUDP);

        void AddTCP(const SOCKET &newConnection);
        void RemoveTCP(const SOCKET &conn);

        void AddUDP(const sockaddr &newConnection);
        void RemoveUDP(const sockaddr &conn);

        FD_SET& GetTCPLobby();
        Server::UDP_SET& GetUDPLobby();

        void SetNext(LobbyNode* n);
        LobbyNode* GetNext();

    private:
        FD_SET TCPlobby;
        Server::UDP_SET UDPlobby;
        LobbyNode* next;
        bool isRemoving, isAdding;
    };
}

#endif