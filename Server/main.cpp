#include "Server.h"

int main() {
	Server::LibStartUp();
	SOCKADDR_IN addr = Server::CreateAddr();

	SOCKET sock = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
	listen(sock, SOMAXCONN);

	FD_SET connections = { 0 };
	while (true) {
		Server::CheckNewConnections(sock, &addr, connections);
		Server::ReceiveData(sock, connections);
	}

	closesocket(sock);
	WSACleanup();
}