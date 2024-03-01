#pragma once
#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_PLAYERS 10

#include <winsock2.h>
#include <iostream>
#include <exception>

namespace Server {
	namespace {
		const auto ip = inet_addr("192.168.0.102"); //If the prog. infinitly outputs "Connection error", try change ip here
		const u_short port = htons(50000);
	}


	class Player {
	private:
	public:
		int x, y;
		Player() : x(9), y(6) {}
		Player(int x_, int y_) : x(x_), y(y_) {}
	};

	void LibStartUp();
	SOCKADDR_IN CreateAddr();
	void CheckNewConnections(SOCKET& sock, SOCKADDR_IN* addr, FD_SET& connections);
	void ReceiveData(SOCKET& sock, FD_SET& connections);
}