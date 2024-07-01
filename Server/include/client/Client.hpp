#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "World.hpp"
#include "Data.hpp"
#include "ScreenSaver.hpp"
#include <SFML/Network.hpp>

namespace {    
    const int MAX_PLAYERS = 4;    
    unsigned short TCPPort = 55001;
    unsigned short ServUDPport = 55003;
    unsigned short ClientUDPPort = 50004;
    const unsigned short TICK_RATE = 20;
    sf::IpAddress ServIP = sf::IpAddress("192.168.0.102"); //write serv ip here
}

enum MESSAGES {
    PLAYER_DISCONNECTED,
    NEW_PLAYER_CONNECTED
    //SEND_DATA
};

void CheckNewData(sf::TcpSocket& TCPsock, sf::UdpSocket& UDPsock, World& world, std::vector<ScreenSaver*>& sc);
void SendData(sf::UdpSocket& UDPsock, World& world, int id);

#endif