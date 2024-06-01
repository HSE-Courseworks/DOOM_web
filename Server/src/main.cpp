#include "LobbyNode.hpp"
#include "Server.h"

using namespace Server;

int main() {
	LibStartUp();
	//SOCKADDR_IN addr = CreateAddr();

	SOCKET sockTCP, sockUDP;
	CreateTCPSocket(sockTCP);
	CreateUDPSocket(sockUDP);	

	LobbyNode connections;

	while (true) {
		CheckNewConnections(sockTCP, connections);
		ReceiveDataTCP(sockTCP, connections);
		ReceiveDataUDP(sockUDP, connections);
	}

	closesocket(sockUDP);
	closesocket(sockTCP);
	WSACleanup();
}