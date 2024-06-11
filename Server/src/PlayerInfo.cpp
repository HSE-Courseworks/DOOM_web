#include "PlayerInfo.hpp"

PlayerInfo::PlayerInfo(sf::TcpSocket* TCPsok, sf::IpAddress plIP, unsigned short p) 
    : TCPsock(TCPsok), ip(plIP), port(p), player() {}

//PlayerInfo& PlayerInfo::operator=(PlayerInfo&& pl) {
//    this->TCPsock = pl.TCPsock;
//    this->ip = pl.ip;
//    this->port = pl.port;
//    this->player = pl.player;
//}

//PlayerInfo::PlayerInfo(const PlayerInfo& plInf) 
//    : TCPsock(plInf.TCPsock), ip(plInf.ip), port(plInf.port), player(plInf.player) {}
//
//PlayerInfo::PlayerInfo(PlayerInfo&& plInf) 
//    : TCPsock(std::move(plInf.TCPsock)), ip(plInf.ip), port(plInf.port), player(plInf.player) {}

void PlayerInfo::SendTCP(sf::Packet& pack) {
    TCPsock->send(pack);
}

void PlayerInfo::SetPlayerID(int id) {
    player.setId(id);
} 

int PlayerInfo::GetPlayerID() {
    return player.getId();
}   

sf::TcpSocket* PlayerInfo::GetTCPSock() {
    return TCPsock;
}

sf::IpAddress PlayerInfo::GetIP() {
    return ip;
}

unsigned short PlayerInfo::GetPort() {
    return port;
}
