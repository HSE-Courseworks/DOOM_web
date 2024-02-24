#pragma once
#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_PLAYERS 10

#include <winsock2.h>
#include <iostream>
#include <array>
#include <vector>
#include <exception>

#include "Arr.h"

namespace Server {
	namespace {
		const auto ip = inet_addr("192.168.0.107");
		const u_short port = htons(1111);
	}

	template<typename T, int size_>
	class Arr;

	void LibStartUp();
	SOCKADDR_IN& CreateAddr();
	void CheckNewConnections(SOCKET& sock, SOCKADDR_IN* addr, Server::Arr<SOCKET, MAX_PLAYERS>& connections);
	//void ReceiveData(SOCKET& sock, Server::Arr<SOCKET, MAX_PLAYERS>& connections);
}