#include "Server.h"

void Server::LibStartUp()
{
	WSAData* wsadata = new WSAData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, wsadata)) {
		throw std::exception("Lib error.");
	}
}

SOCKADDR_IN& Server::CreateAddr()
{
	SOCKADDR_IN* addr = new SOCKADDR_IN;
	addr->sin_addr.s_addr = ip;
	addr->sin_port = port;
	addr->sin_family = AF_INET;
	return *addr;
}

void Server::CheckNewConnections(SOCKET& sock, SOCKADDR_IN* addr, Server::Arr<SOCKET, MAX_PLAYERS>& connections)
{
	if (connections.size() == MAX_PLAYERS)
		return;
	int sizeOfAddr = sizeof(*addr);
	SOCKET newConnection = accept(sock, reinterpret_cast<SOCKADDR*>(addr), &sizeOfAddr);
	if (newConnection != INVALID_SOCKET) {
		std::cout << "New connection.\n";
		char msg[] = "Hello, client!\n";
		send(newConnection, msg, sizeof(msg), NULL);
		for (auto it : connections) {
			char toOldsMsg[] = "Meet a new player!\n";
			send(it, toOldsMsg, sizeof(toOldsMsg), NULL);
		}
		connections.push_back(newConnection);
	}
	else {
		std::cout << "Connection error.\n";
	}
}

//void Server::ReceiveData(SOCKET& sock, Server::Arr<SOCKET, MAX_PLAYERS>& connections)
//{
//	for (auto it : connections) {
//		int buf[2];
//		int res = recv(it, reinterpret_cast<char*>(buf), sizeof(buf), NULL);
//		if (!res) {
//			connections.erase(Server::Arr<SOCKET, MAX_PLAYERS>::const_iterator it);
//		}
//	}
//}
