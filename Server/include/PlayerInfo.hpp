#ifndef PLAYERINFO_HPP
#define PLAYERINFO_HPP

//#include "Lobby.hpp"
//#include "Player.hpp"
#include <SFML/Network.hpp>

class PlayerInfo {
public:
    PlayerInfo(int _id, sf::IpAddress plIp, unsigned short p);
    //PlayerInfo(const PlayerInfo& plInf);
    //PlayerInfo(PlayerInfo&& plInf);
    ~PlayerInfo() = default;

    //PlayerInfo& operator=(PlayerInfo&& pl);
    
    //void SendTCP(sf::Packet& pack);

    void SetPlayerID(int _id);
    int GetPlayerID();
    //sf::TcpSocket* GetTCPSock();
    sf::IpAddress& GetIP();
    unsigned short GetPort();

    //friend sf::Packet& operator<<(sf::Packet& pack, PlayerInfo& pl);
private:
    int id;
    //sf::TcpSocket* TCPsock;
    sf::IpAddress ip;
    unsigned short port;
    //Player player;
};

//sf::Packet& operator<<(sf::Packet& pack, PlayerInfo& pl);

#endif