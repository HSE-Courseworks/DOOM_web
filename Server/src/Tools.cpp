#include "Tools.hpp"
#include "Server.hpp"
#include <chrono>

using namespace std::chrono_literals;

void ExchangeData(PlayerInfo *pl, Lobby *lobby, sf::UdpSocket& UDPMessagesock, sf::UdpSocket& UDPPlayersock) {
    sf::Packet pack;
    sf::IpAddress plIp = pl->GetIP();
    std::chrono::steady_clock::time_point prevPL = std::chrono::steady_clock::now();
    const int SECS_TO_DEL = 4;
    bool isConnected = true;
    while (isConnected) {
        int res;
        if (UDPMessagesock.receive(pack, plIp, ClientMessageUDPPort) == sf::TcpSocket::Done) {
            while (!pack.endOfPacket()){
                pack >> res;
                switch (res) {
                    case PLAYER_DISCONNECTED: {
                        lobby->Remove(pl->GetPlayerID());
                        isConnected = false;
                        break;
                    }
                    case SHOOT:
                    case PICK_UP:
                    case RELOAD: {
                        lobby->PushEvent(static_cast<PlayerEvent>(res), pl->GetPlayerID());
                        break;
                    }
                }
            }
            plIp = pl->GetIP();
        }
        plIp = pl->GetIP();
        if (UDPPlayersock.receive(pack, plIp, ClientPlayerUDPPort) == sf::TcpSocket::Done) {
            prevPL = std::chrono::steady_clock::now();
            pack >> *(*lobby)[pl->GetPlayerID()];
            plIp = pl->GetIP();
        }
        if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - prevPL).count() >= SECS_TO_DEL) {
            lobby->Remove(pl->GetPlayerID());
            isConnected = false;
            break;
        }
    }
}


