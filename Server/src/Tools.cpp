#include "Tools.hpp"
#include "Server.hpp"
#include <chrono>

using namespace std::chrono_literals;

void ExchangeData(PlayerInfo *pl, Lobby *lobby) {
    sf::Packet pack;
    while (true) {
        int res;

        while (pl->GetTCPSock()->receive(pack) == sf::TcpSocket::Done) {
            pack >> res;
            if (PLAYER_DISCONNECTED == res) {
                int plId;
                pack >> plId;
                lobby->Remove(plId);
            }
        }
    }
}

sf::Packet& operator<<(sf::Packet& pack, PlayerInfo& pl) {
    pack << pl.player;
    return pack;
}

