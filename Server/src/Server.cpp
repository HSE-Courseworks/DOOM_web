#define LOG_ERROR 5
#define LOG_INFO 3

#include "LobbyNode.hpp"
#include "Data.hpp"
#include "Server.h"
#include <format>
#include <iostream>

void Server::CustomLog(int msgType, const char *text)
{
    switch (msgType)
    {
        case LOG_INFO: std::cout << "[INFO] : "; break;
        case LOG_ERROR: std::cout << "[ERROR]: "; break;
        default: break;
    }

    std::cout << text << std::endl;
}

void Server::LibStartUp()
{
	WSAData* wsadata = new WSAData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, wsadata)) {
		CustomLog(LOG_ERROR, "Library starting error.");
	}
}

SOCKADDR_IN Server::CreateAddr()
{
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = ip;
	//addr.sin_port = port;
	addr.sin_family = AF_INET;
	return addr;
}

void Server::CreateTCPSocket(SOCKET &sock)
{
	addrinfo *result, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int iRes = getaddrinfo(NULL, "5001", &hints, &result);
	if (iRes != 0) {
		CustomLog(LOG_ERROR, std::format("Function \"getaddrinfo\" failed with error: {}", iRes).data());
		exit(-1);
	}

	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET){
		CustomLog(LOG_ERROR, std::format("Socket failed with error: {}", WSAGetLastError()).data());
		freeaddrinfo(result);
		WSACleanup();
		exit(-1);
	}

	iRes = bind(sock, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iRes == SOCKET_ERROR){
		CustomLog(LOG_ERROR, std::format("Bind failed with error: {}", WSAGetLastError()).data());
		freeaddrinfo(result);
		closesocket(sock);
		WSACleanup();
		exit(-1);
	}
	freeaddrinfo(result);

	iRes = listen(sock, SOMAXCONN);
	if (iRes == SOCKET_ERROR){
		CustomLog(LOG_ERROR, std::format("Listen failed with error: {}", WSAGetLastError()).data());
		closesocket(sock);
		WSACleanup();
		exit(-1);
	}

    //u_long mode = 1;
    //ioctlsocket(sock, FIONBIO, &mode);
}

void Server::CreateUDPSocket(SOCKET &sock)
{
	addrinfo *result, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	int iRes = getaddrinfo(NULL, "5000", &hints, &result);
	if (iRes){
		CustomLog(LOG_ERROR, std::format("Function \"getaddrinfo\" failed with error: {}", iRes).data());
		exit(-1);
	}

	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET){
		CustomLog(LOG_ERROR, std::format("Socket failed with error: {}", WSAGetLastError()).data());
		freeaddrinfo(result);
		WSACleanup();
		exit(-1);
	}

	iRes = bind(sock, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iRes == SOCKET_ERROR){
		CustomLog(LOG_ERROR, std::format("Bind failed with error: {}", WSAGetLastError()).data());
		freeaddrinfo(result);
		closesocket(sock);
		WSACleanup();
		exit(-1);
	}
	freeaddrinfo(result);

    //u_long mode = 1;
    //ioctlsocket(sock, FIONBIO, &mode);
}

void Server::CheckNewConnections(SOCKET &sock, LobbyNode &lobbies)
{
	while (true) {
		addrinfo newAddrInfo;
		int addrlen = newAddrInfo.ai_addrlen;
		SOCKET newConnection = accept(sock, newAddrInfo.ai_addr, &addrlen);

		if (newConnection == INVALID_SOCKET)
			continue;
		
		lobbies.AddTCP(newConnection);
		lobbies.AddUDP(*newAddrInfo.ai_addr);
	}	
}

void Server::ReceiveDataTCP(SOCKET &sock, LobbyNode &lobbies)
{
	while (true) {
		LobbyNode *cur = &lobbies;
		const int MSG_DIGITS_NUM = 5;
		char msg[sizeof(Data)];
		int lobbyCount = 0;
		while (cur) {
			for (u_int i = 0; i < cur->GetTCPLobby().fd_count; ++i){
				int res = recv(cur->GetTCPLobby().fd_array[i], msg, sizeof(msg), NULL);
				if (!(res == SOCKET_ERROR || atoi(msg) == PLAYER_DISCONNECTED)) 
					continue;
				CustomLog(LOG_INFO, std::format("Player in lobby {} in place {} has been disconnected.", lobbyCount, i).data());
				for (u_int j = i; j < cur->GetTCPLobby().fd_count - 1; ++j)
					cur->GetTCPLobby().fd_array[j] = cur->GetTCPLobby().fd_array[j + 1];
				--cur->GetTCPLobby().fd_count;
				itoa(PLAYER_DISCONNECTED, msg, 10);
				char disconnectedPlayerID[MSG_DIGITS_NUM];
				itoa(i, disconnectedPlayerID, 10);
				for (u_int j = 0; j < cur->GetTCPLobby().fd_count; ++j) {
					send(cur->GetTCPLobby().fd_array[j], msg, sizeof(msg), NULL);
					send(cur->GetTCPLobby().fd_array[j], disconnectedPlayerID, sizeof(disconnectedPlayerID), NULL);
				}
			}
			cur = cur->GetNext();
		}
	}
	
}

void Server::ReceiveDataUDP(SOCKET &sock, LobbyNode &lobbies)
{
	while (true) {
		LobbyNode *cur = &lobbies;
		for (int i = 0; i < cur->GetUDPLobby().size; ++i){
			char buf[sizeof(Data)];
			int sinLen = sizeof(cur->GetUDPLobby().array[i]);
			int res = recvfrom(sock, buf, sizeof(buf), NULL, &(cur->GetUDPLobby().array[i]), &sinLen);
			if (res == SOCKET_ERROR){
				CustomLog(LOG_ERROR, std::format("Receiving failure with code: {}", WSAGetLastError()).data());
				continue;
			}
			for (int j = 0; j < cur->GetUDPLobby().size; ++j) {
				if (i == j)
					continue;
				sendto(sock, buf, sizeof(buf), NULL, &(cur->GetUDPLobby().array[j]), sinLen);
			}
		}
	}	
}
