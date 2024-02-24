#include "Server.h"

int main() {
	Server::LibStartUp();

	SOCKADDR_IN& addr = Server::CreateAddr();

	SOCKET sock = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
	listen(sock, SOMAXCONN);

	Server::Arr<SOCKET, MAX_PLAYERS> connections;
	while (true) {
		Server::CheckNewConnections(sock, &addr, connections);

		//TODO: implement func to exchange info
	}

	closesocket(sock);
	delete (&addr);
	WSACleanup();
}