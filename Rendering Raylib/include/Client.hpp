#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "World.hpp"
#include "ScreenSaver.hpp"
#include <SFML/Network.hpp>
#include <string>

namespace {       
    unsigned short ServTCPPort = 42001;
    unsigned short ClientPlayerUDPPort = 42002; 
    unsigned short ServWorldUDPPort = 42003;
    unsigned short ClientWorldUDPPort = 42004;
    unsigned short ClientMessageUDPPort = 42005;
    unsigned short ServPlayerUDPPort = 42006;
    unsigned short ServMesssageUDPPort = 42007;
    const unsigned short TICK_RATE = 20;
    sf::IpAddress ServIP = sf::IpAddress(""); //write serv ip here
}

bool ConnectToServ(std::string name, User* user);
void ReceiveWorld(sf::UdpSocket& UDPsock, World *world);
void SendMessage(sf::UdpSocket& UDPsock, PlayerEvent ev);
void SendPlayer(sf::UdpSocket& UDPsock, Player* player);

#endif