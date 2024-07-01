#include "Client.hpp"

void CheckNewData(sf::TcpSocket& TCPsock, sf::UdpSocket& UDPsock, World& world, std::vector<ScreenSaver*>& sc) {
    sf::Packet pack;
    sf::TcpSocket::Status resTCP;
    resTCP = TCPsock.receive(pack);
    while (sf::TcpSocket::Done == resTCP) {
        int event;
        pack >> event;
        if (PLAYER_DISCONNECTED == event) {
            int id;
            pack >> id;
            world.removePlayer(id);
            auto s = sc.begin();
            for (s = sc.begin(); s != sc.end(); ++s)
                if ((*s)->getUser().getId() == id)
                    break;
            delete *s;
            sc.erase(s);
        }
        if (NEW_PLAYER_CONNECTED == event) {
            int id;
            pack >> id;
            world.addPlayer(id);
            sc.push_back(new ScreenSaver(User(id, "")));
        }
        pack.clear();
        resTCP = TCPsock.receive(pack);
    }
    sf::UdpSocket::Status resUDP;
    resUDP = UDPsock.receive(pack, ServIP, ServUDPport);
    while (sf::UdpSocket::Done == resUDP) {
            resUDP = UDPsock.receive(pack, ServIP, ServUDPport);
        int id;
        pack >> id;
        pack >> *(world.players[id].second);
        pack.clear();
        resUDP = UDPsock.receive(pack, ServIP, ServUDPport);
    }
}

void SendData(sf::UdpSocket& UDPsock, World& world, int id) {
    sf::Packet pack;
    pack << world.GetPlayer(id);
    UDPsock.send(pack, ServIP, ServUDPport);
}
