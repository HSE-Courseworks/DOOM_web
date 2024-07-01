#include "Client.hpp"
#include <SFML/System/Clock.hpp>

bool ConnectToServ(std::string name, User* user) {
    sf::TcpSocket TCPsock;
    sf::TcpSocket::Status res = TCPsock.connect(ServIP, ServTCPPort);//, sf::microseconds(5000)); //max time to connect is 5 seconds
    sf::Packet pack;
    pack << name;
    if (res == sf::TcpSocket::Status::Done) {
        TCPsock.send(pack);
        pack.clear();
        TCPsock.receive(pack);
        int id;
        pack >> id;
        user->setId(id);
        TraceLog(LOG_INFO, "Has connected to server.");
        return true;
    }
    TraceLog(LOG_INFO, "Has not connected to server.");
    return false;
}

void ReceiveWorld(sf::UdpSocket& UDPsock, World *world) {
    sf::Packet pack;
    UDPsock.receive(pack, ServIP, ServWorldUDPPort);
    pack >> *world;
}

void SendMessage(sf::UdpSocket& UDPsock, PlayerEvent ev) {
    sf::Packet pack;
    pack << ev;
    UDPsock.send(pack, ServIP, ServMesssageUDPPort);
}
}

