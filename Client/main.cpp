#include "Client.h"

int main() {
	Client::LibStartUp();

	SOCKADDR_IN& addr = *(Client::CreateAddr());

	SOCKET sock = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));

	if (connect(sock, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr)) == SOCKET_ERROR) {
		throw std::exception("Connection error.");
		return -1;
	}
	
	std::cout << "Connected.\n";
	while (true) {
		Client::CheckMessages(sock);
	}

	closesocket(sock);
	delete (&addr);
	WSACleanup();
}