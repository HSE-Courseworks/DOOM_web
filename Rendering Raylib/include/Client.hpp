#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "World.hpp"
#include <SFML/Network.hpp>

namespace {    
    const int MAX_PLAYERS = 10;    
    unsigned short TCPPort = 55001;
    unsigned short ServUDPport = 55003;
    unsigned short ClientUDPPort = 50004;
    const unsigned short TICK_RATE = 20;
    sf::IpAddress ServIP = sf::IpAddress(""); //write serv ip here
}

enum MESSAGES {
    PLAYER_DISCONNECTED,
    NEW_PLAYER_CONNECTED
    //SEND_DATA
};

void CheckNewData(sf::TcpSocket& TCPsock, sf::UdpSocket& UDPsock, World& world);
void SendData(sf::UdpSocket& UDPsock);

#endif