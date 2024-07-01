#include "Lobby.hpp"
#include "Server.hpp"
#include "Tools.hpp"
#include <chrono>
#include <thread>
#include <list>
#include <iostream>
#include <SFML/Network.hpp>

using namespace std::chrono_literals;

int Lobby::lobbyCount = 0;

//sf::Packet& operator<<(sf::Packet& pack, std::unordered_map<int, std::pair<std::thread*, PlayerInfo*>>& players) {
//    for (auto& pl : players) {
//        pack << pl.first;
//        pack << *(pl.second.second);
//    }
//    return pack;
//}

Lobby::Lobby() : lId(lobbyCount), players(), firstFreePlayerID(0), /*lastPlayerID(0), vecID(),*/  world("resources/maze.txt", "resources/textures.txt") {
    ++lobbyCount;
    world.timer.start();
    std::cout << "New lobby has been created. Lobby: " << lId << std::endl;
}

Lobby::Lobby(Lobby& l) : lId(l.lId), players(l.players), /*lastPlayerID(l.lastPlayerID), vecID(l.vecID),*/  world(l.world) {
    //std::cout << "New lobby has been created. Lobby: " << lId << std::endl;
}

Lobby::Lobby(Lobby&& l) : lId(l.lId), players(l.players), /*lastPlayerID(l.lastPlayerID), vecID(l.vecID),*/  world(l.world) {
    //std::cout << "New lobby has been created. Lobby: " << lId << std::endl;
}

Lobby& Lobby::operator=(Lobby& l) {
    lId = l.lId;
    players = l.players;
    //lastPlayerID = l.lastPlayerID;
    //vecID = l.vecID;
    return *this;
}

Lobby::~Lobby() {
    for (auto& pl : players) {
        delete pl.second.first;
        delete pl.second.second;
    }
    --lobbyCount;
    std::cout << "Lobby has been destroyed. Lobby: " << lId << std::endl;
}

Player* Lobby::operator[](int id) {
    return world.getPlayer(id);
}

void Lobby::Remove(int playerID) {
    //std::unique_lock thrdLock(mtx);
    //int idx = std::find(vecID.begin(), vecID.end(), playerID) - vecID.begin();
	//vecID.erase(vecID.begin() + idx);
	players.erase(playerID);
    world.removePlayer(playerID);
    std::cout << "Player has disconnected. Lobby: " << lId << " Player: " << playerID << std::endl;
}

void Lobby::Add(sf::TcpSocket& TCPsock, sf::UdpSocket& UDPWorldsock, sf::UdpSocket& UDPPlayersock, sf::UdpSocket& UDPMessagesock) {
    PlayerInfo* pl = new PlayerInfo(firstFreePlayerID, TCPsock.getRemoteAddress(), 0);
    players[firstFreePlayerID] = std::make_pair(new std::thread(::ExchangeData, pl, this, std::ref(UDPMessagesock), std::ref(UDPPlayersock)), pl);
    sf::Packet pack;
    TCPsock.receive(pack);
    std::string name;
    pack >> name;
    pack.clear();
    pack << firstFreePlayerID;
    TCPsock.send(pack);
    world.addPlayer(firstFreePlayerID, name);
    std::cout << "New player. Lobby: " << lId << " Player: " << firstFreePlayerID << std::endl;
    firstFreePlayerID++;
}

void Lobby::PushEvent(PlayerEvent ev, int id) {
    world.pushEvent(ev, id);
}

//void Lobby::SendAllTCP(sf::Packet &pack) {
//    for (auto& pl : players) 
//        pl.second.second->SendTCP(pack);
//}

void Lobby::ExchangeData(sf::UdpSocket& UDPsock) {
    std::chrono::steady_clock::time_point start;
    while (true) {
        if (players.empty()) 
            break;
        world.compute();
        std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(start - std::chrono::steady_clock::now()) + std::chrono::milliseconds(1000 / TICK_RATE));
        start = std::chrono::steady_clock::now();
        sf::Packet pack;
        pack << world;
        for (auto& pl : players) {
            UDPsock.send(pack, pl.second.second->GetIP(), ClientWorldUDPPort);
        }
    }
}

int Lobby::GetSize() { return players.size(); }

int Lobby::GetID() { return lId; }

int Lobby::GetFirstFreePlayerID() { return firstFreePlayerID; }

//int Lobby::GetLastPlayerID() { return ; }
