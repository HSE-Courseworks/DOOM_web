#pragma once
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>

namespace Client {
	namespace {
		const auto ip = inet_addr("192.168.0.107");
		const u_short port = htons(1111);
	}

	void LibStartUp();
	SOCKADDR_IN* CreateAddr();
	void CheckMessages(SOCKET& sock);
}