#include "Client.hpp"

void CheckNewData(sf::TcpSocket& TCPsock, sf::UdpSocket& UDPsock, World& world) {
    sf::Packet pack;
    sf::TcpSocket::Status resTCP;
    resTCP = TCPsock.receive(pack);
    while (sf::TcpSocket::Done == resTCP || sf::TcpSocket::Partial == resTCP) {
        while (sf::TcpSocket::Partial == resTCP) 
            resTCP = TCPsock.receive(pack);
        int event;
        pack >> event;
        if (PLAYER_DISCONNECTED == event) {
            int id;
            pack >> id;
            world.removePlayer(id);
        }
        if (NEW_PLAYER_CONNECTED == event) {
            int id;
            pack >> id;
            world.addPlayer(id);
        }
    }
    sf::UdpSocket::Status resUDP;
    resUDP = UDPsock.receive(pack, ServIP, ServUDPport);
    while (sf::UdpSocket::Done == resUDP || sf::UdpSocket::Partial == resUDP) {
        while (sf::TcpSocket::Partial == resTCP) 
            resTCP = TCPsock.receive(pack);
    }
}

