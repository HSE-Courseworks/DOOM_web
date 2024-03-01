#pragma once
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>

namespace Client {
	namespace {
		const auto ip = inet_addr("192.168.0.102");
		const u_short port = htons(50000);
		bool isFirst = true;
	}

	class Player {
	private: 
	public:
		int x, y;
		Player(int x_, int y_) : x(x_), y(y_){}
	};

	void LibStartUp();
	SOCKADDR_IN CreateAddr();
	void CheckMessages(SOCKET& sock);
	void SendData(SOCKET& sock);
}