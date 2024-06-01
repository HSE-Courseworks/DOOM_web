#ifndef SERVER_H
#define SERVER_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define FIONBIO (0x80000000 | (((long)sizeof(Server::u_long) & 0x7f) << 16) | (('f') << 8) | (126))
#define DEFAULT_PORT "27015"

#include "LobbyNode.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

namespace Server {

	namespace {
		const auto ip = inet_addr("192.168.0.106"); //If the prog. infinitly outputs "Connection error", try change ip here
		const u_short UDPport = htons(5000);
		const u_short TCPport = htons(5001);
	}

	enum MESSAGES {
		PLAYER_DISCONNECTED,
		NEW_PLAYER_CONNECTED
	};

	void CustomLog(int msgType, const char *text);
	void LibStartUp();
	SOCKADDR_IN CreateAddr();
	void CreateTCPSocket(SOCKET &sock);
	void CreateUDPSocket(SOCKET &sock);
	void CheckNewConnections(SOCKET &sock, LobbyNode &lobbies);
	void ReceiveDataTCP(SOCKET &sock, LobbyNode &lobbies);
	void ReceiveDataUDP(SOCKET &sock, LobbyNode &lobbies);
}
#endif