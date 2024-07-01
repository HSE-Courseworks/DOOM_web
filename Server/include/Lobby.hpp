#ifndef LOBBY_HPP
#define LOBBY_HPP

#include "PlayerInfo.hpp"
#include "World.hpp"
#include <vector>
#include <unordered_map>
#include <mutex>
#include <SFML/Network.hpp>
#include <list>
#include <thread>

class Lobby {
public:
    Lobby();
    Lobby(Lobby& l);
    Lobby(Lobby&& l);

    Lobby& operator=(Lobby& l);

    ~Lobby();

    Player* operator[](int id);

    void Add(sf::TcpSocket& TCPsock, sf::UdpSocket& UDPWorldsock, sf::UdpSocket& UDPPlayersock, sf::UdpSocket& UDPMessagesock);
    void PushEvent(PlayerEvent ev, int id);
    void Remove(int playerID);
    //void SendAllTCP(sf::Packet &pack);
    void ExchangeData(sf::UdpSocket& UDPWorldsock);

    int GetSize();
    int GetID();
    //int GetLastPlayerID();
    int GetFirstFreePlayerID();

    static int lobbyCount;
private:
    int lId;
    std::unordered_map<int, std::pair<std::thread*, PlayerInfo*>> players;
    int firstFreePlayerID;
    World world;
};

sf::Packet& operator<<(sf::Packet& pack, std::unordered_map<int, std::pair<std::thread*, PlayerInfo*>>& players);

#endif