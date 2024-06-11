#ifndef LOBBY_HPP
#define LOBBY_HPP

#include "PlayerInfo.hpp"
#include <vector>
#include <unordered_map>
#include <mutex>
#include <SFML/Network.hpp>
#include <list>
#include <thread>

class Lobby {
public:
    Lobby();
    Lobby(Lobby&& l);
    ~Lobby();

    PlayerInfo* operator[](int id);

    void Add(sf::TcpSocket* TCPsock);
    void Remove(int playerID);
    void SendAllTCP(sf::Packet &pack);
    void ExchangeData();

    int GetSize();
    int GetLastPlayerID();

    static int lobbyCount;
private:
    int lId;
    std::unordered_map<int, std::pair<std::thread*, PlayerInfo*>> players;
    int lastPlayerID;
    std::vector<int> vecID;
    std::mutex mtx;
};

sf::Packet& operator<<(sf::Packet& pack, std::unordered_map<int, std::pair<std::thread*, PlayerInfo*>>& players);

#endif