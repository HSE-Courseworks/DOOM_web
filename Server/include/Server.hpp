#ifndef SERVER_HPP
#define SERVER_HPP

#include "Lobby.hpp"
#include "Tools.hpp"
#include <SFML/Network.hpp>
#include <list>
#include <thread>

namespace {    
    const int MAX_PLAYERS = 10;    
    unsigned short TCPPort = 55001;
    unsigned short ServUDPport = 55003;
    unsigned short ClientUDPPort = 50004;
    const unsigned short TICK_RATE = 20;
}

void CheckNewConnections(sf::TcpListener &TCPListener, std::list<Lobby> &lobbies, std::list<std::thread> &thrdsComputeLobbies);

#endif