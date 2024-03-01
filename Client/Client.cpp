#include "Client.h"

void Client::LibStartUp()
{
	WSAData* wsadata = new WSAData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, wsadata)) {
		throw std::exception("Lib error.");
	}
}

SOCKADDR_IN Client::CreateAddr()
{
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = ip;
	addr.sin_port = port;
	addr.sin_family = AF_INET;
	return addr;
}

void Client::CheckMessages(SOCKET& sock)
{
	if (isFirst) {
		Player p(1, 8);
		recv(sock, reinterpret_cast<char*>(&p), sizeof(p), NULL);
		std::cout << p.x << ' ' << p.y << std::endl;
		isFirst = false;
		return;
	}
	char p[256];
	recv(sock, p, sizeof(p), NULL);
	std::cout << p;
}

void Client::SendData(SOCKET& sock)
{
	Player p(0, 0);
	send(sock, reinterpret_cast<char*>(&p), sizeof(p), NULL);
}

