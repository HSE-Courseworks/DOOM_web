#define FD_SETSIZE (MAX_PLAYERS)
#include "Server.h"


void Server::LibStartUp()
{
	WSAData* wsadata = new WSAData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, wsadata)) {
		throw std::exception("Lib error.");
	}
}

SOCKADDR_IN Server::CreateAddr()
{
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = ip;
	addr.sin_port = port;
	addr.sin_family = AF_INET;
	return addr;
}

void Server::CheckNewConnections(SOCKET& sock, SOCKADDR_IN* addr, FD_SET& connections)
{
	if (connections.fd_count >= MAX_PLAYERS)
		return;
	int sizeOfAddr = sizeof(*addr);
	SOCKET newConnection = accept(sock, reinterpret_cast<SOCKADDR*>(addr), &sizeOfAddr);
	if (newConnection != INVALID_SOCKET) {
		std::cout << "New connection.\n";
		Player p(0, 0);
		send(newConnection, reinterpret_cast<char*>(&p), sizeof(p), NULL);
		for (int i = 0; i < connections.fd_count; ++i) {
			char toOldsMsg[] = "Meet a new player!\n";
			send(connections.fd_array[i], toOldsMsg, sizeof(toOldsMsg), NULL);
		}
		FD_SET(newConnection, &connections);
	}
	if (newConnection == INVALID_SOCKET) {
		std::cout << "Connection error.\n";
	}
}

void Server::ReceiveData(SOCKET& sock, FD_SET& connections)
{
	for (int i = 0; i < connections.fd_count; ++i) {
		Player buf;
		int res = recv(connections.fd_array[i], reinterpret_cast<char*>(&buf), sizeof(buf), NULL);
		if (res < 0) {
			std::cout << "Element " << i << " has disconnected.\n";
			for (int j = i; j < connections.fd_count - 1; ++j)
				connections.fd_array[j] = connections.fd_array[j + 1];
			--connections.fd_count;
		}
	}
}
