#include "LobbyNode.hpp"
#include <cstring>

bool operator!=(sockaddr &lhs, const sockaddr &rhs) {
	return !std::strcmp(lhs.sa_data, rhs.sa_data) && lhs.sa_family == rhs.sa_family;
}

Server::LobbyNode::LobbyNode(LobbyNode *n) :
	TCPlobby({0, {0}}), UDPlobby({0, {0}}), next(n), isRemoving(false), isAdding(false)
{	
}

Server::LobbyNode::~LobbyNode() {
	LobbyNode *cur = this, *next = cur->next;
	while (cur) {
		delete cur;
		cur = next;
		next = (cur) ? (cur->next) : (nullptr);
	}
}

void Server::LobbyNode::Add(const SOCKET &newConnTCP, const sockaddr &newConnUDP) {
	while (isRemoving);
	isAdding = true;
	AddTCP(newConnTCP);
	AddUDP(newConnUDP);
	isAdding = false;
}

void Server::LobbyNode::Remove(const SOCKET &newConnTCP, const sockaddr &newConnUDP) {
	while (isAdding);
	isRemoving = true;
	RemoveTCP(newConnTCP);
	RemoveUDP(newConnUDP);
	isRemoving = false;
}

void Server::LobbyNode::AddTCP(const SOCKET &newConnection)
{
	LobbyNode* cur = this;
	while (cur->next && MAX_PLAYERS == cur->TCPlobby.fd_count)
		cur = cur->next;
	if (MAX_PLAYERS == cur->TCPlobby.fd_count)	{
		cur->next = new LobbyNode();
		cur = cur->next;
	}
	// const int MSG_DIGITS_COUNT = 5;
	// char msg[MSG_DIGITS_COUNT];
	// itoa(NEW_PLAYER_CONNECTED, msg, 10);
	// for (int i = 0; i < cur->TCPlobby.fd_count; ++i)
	// 	send(cur->TCPlobby.fd_array[i], msg, sizeof(msg), NULL);
	
	FD_SET(newConnection, &cur->TCPlobby);
}

void Server::LobbyNode::RemoveTCP(const SOCKET &conn)
{
	LobbyNode *cur = this;
	bool isRemoved = false;
	while (!isRemoved && cur) {
		for (u_int i = 0; i < cur->TCPlobby.fd_count; ++i) {
			if (cur->TCPlobby.fd_array[i] != conn)
				continue;
			for (u_int j = i; j < cur->TCPlobby.fd_count - 1; ++j) 
				cur->TCPlobby.fd_array[j] = cur->TCPlobby.fd_array[j + 1];
			--cur->TCPlobby.fd_count;
			isRemoved = true;			
			break;
		}
		cur = cur->next;
	}
}

void Server::LobbyNode::AddUDP(const sockaddr &newConnection) {
	LobbyNode* cur = this;
	while (cur->next && MAX_PLAYERS == cur->UDPlobby.size)
		cur = cur->next;
	if (MAX_PLAYERS == cur->UDPlobby.size) {
		cur->next = new LobbyNode();
		cur = cur->next;
	}

	cur->UDPlobby.array[cur->UDPlobby.size] = newConnection;
	++cur->UDPlobby.size;
}

void Server::LobbyNode::RemoveUDP(const sockaddr &conn) {
	LobbyNode *cur = this;
	bool isRemoved = false;
	while (!isRemoved && cur) {
		for (int i = 0; i < cur->UDPlobby.size; ++i) {
			if (cur->UDPlobby.array[i] != conn)
				continue;
			for (int j = i; j < cur->UDPlobby.size - 1; ++j)
				cur->UDPlobby.array[j] = cur->UDPlobby.array[j + 1];
			--cur->UDPlobby.size;
			isRemoved = true;
			break;
		}
		cur = cur->next;
	}
}

FD_SET& Server::LobbyNode::GetTCPLobby() { return TCPlobby; }

Server::UDP_SET& Server::LobbyNode::GetUDPLobby() { return UDPlobby; }

void Server::LobbyNode::SetNext(LobbyNode* n) { next = n; }

Server::LobbyNode* Server::LobbyNode::GetNext() { return next; }
