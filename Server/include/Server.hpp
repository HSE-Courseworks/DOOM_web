#ifndef SERVER_HPP
#define SERVER_HPP

#include "Lobby.hpp"
#include "Tools.hpp"
#include <SFML/Network.hpp>
#include <list>
#include <thread>

namespace {    
    const int MAX_PLAYERS = 4;    
    unsigned short TCPPort = 42001;
    unsigned short ClientPlayerUDPPort = 42002; 
    unsigned short ServWorldUDPPort = 42003;
    unsigned short ClientWorldUDPPort = 42004;
    unsigned short ClientMessageUDPPort = 42005;
    unsigned short ServPlayerUDPPort = 42006;
    unsigned short ServMesssageUDPPort = 42007;
    const unsigned short TICK_RATE = 20;
}

void CheckNewConnections(sf::TcpListener &TCPListener, std::list<Lobby> &lobbies, std::list<std::thread> &thrdsComputeLobbies);

#endif