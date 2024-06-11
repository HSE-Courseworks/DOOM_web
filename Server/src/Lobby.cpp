#include "Lobby.hpp"
#include "Server.hpp"
#include "Tools.hpp"
#include <chrono>
#include <thread>
#include <list>
#include <iostream>

using namespace std::chrono_literals;

int Lobby::lobbyCount = 0;

sf::Packet& operator<<(sf::Packet& pack, std::unordered_map<int, std::pair<std::thread*, PlayerInfo*>>& players) {
    for (auto& pl : players) {
        pack << pl.first;
        pack << *(pl.second.second);
    }
    return pack;
}

Lobby::Lobby() : lId(lobbyCount), players(), lastPlayerID(0), vecID(), mtx() {
    ++lobbyCount;
    std::cout << "New lobby has been created. Lobby: " << lId << std::endl;
}

Lobby::Lobby(Lobby&& l) : lId(l.lId), players(l.players), lastPlayerID(l.lastPlayerID), vecID(l.vecID), mtx() {
    ++lobbyCount;
    std::cout << "New lobby has been created. Lobby: " << lId << std::endl;
}

Lobby::~Lobby() {
    for (auto& pl : players) {
        delete pl.second.first;
        delete pl.second.second;
    }
    --lobbyCount;
    std::cout << "Lobby has been destroyed. Lobby: " << lId << std::endl;
}

PlayerInfo* Lobby::operator[](int id) {
    return (players[id]).second;
}

void Lobby::Remove(int playerID) {
    std::unique_lock thrdLock(mtx);
    int idx = std::find(vecID.begin(), vecID.end(), playerID) - vecID.begin();
	vecID.erase(vecID.begin() + idx);
	players.erase(playerID);
    std::cout << "Player has disconnected. Lobby: " << lId << " Player: " << playerID << std::endl;
}

void Lobby::Add(sf::TcpSocket *TCPsock) {
    std::unique_lock thrdLock(mtx);
    players.insert(std::make_pair(lastPlayerID, std::make_pair(
        new std::thread(::ExchangeData, (players.at(lastPlayerID).second), this),
        new PlayerInfo(TCPsock, TCPsock->getRemoteAddress(), TCPsock->getRemotePort() + 1)
    )));
    players.at(lastPlayerID).second->SetPlayerID(lastPlayerID);
    std::cout << "New player. Lobby: " << lId << " Player: " << lastPlayerID << std::endl;
    vecID.push_back(lastPlayerID++);
}

void Lobby::SendAllTCP(sf::Packet &pack) {
    std::unique_lock thrdLock(mtx);
    for (auto& pl : players) 
        pl.second.second->SendTCP(pack);
}

void Lobby::ExchangeData() {
//     for (auto& pl : players) 
//         pl.second.first = std::thread(::ExchangeData, &pl.second, this); //OK
    sf::UdpSocket UDPsock;
    UDPsock.bind(ServUDPport);
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now(), end;
    while (true) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(start - end + std::chrono::milliseconds(1000 / TICK_RATE)).count() > 0)
			std::this_thread::sleep_for(start - end + std::chrono::milliseconds(1000 / TICK_RATE));
        start = std::chrono::steady_clock::now();
        sf::Packet pack;
        std::unique_lock thrdLock(mtx);
        pack << players;
        thrdLock.unlock();
        for (auto& pl : players) 
            UDPsock.send(pack, pl.second.second->GetIP(), pl.second.second->GetPort());
        end = std::chrono::steady_clock::now();
    }
}

int Lobby::GetSize() { return players.size(); }

int Lobby::GetLastPlayerID() { return vecID.back(); }
