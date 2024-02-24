#include "Client.h"

void Client::LibStartUp()
{
	WSAData* wsadata = new WSAData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, wsadata)) {
		throw std::exception("Lib error.");
	}
}

SOCKADDR_IN* Client::CreateAddr()
{
	SOCKADDR_IN* addr = new SOCKADDR_IN;
	addr->sin_addr.s_addr = ip;
	addr->sin_port = port;
	addr->sin_family = AF_INET;
	return addr;
}

void Client::CheckMessages(SOCKET& sock)
{
	char msg[256];
	recv(sock, msg, 256, NULL);
	std::cout << msg;
}

