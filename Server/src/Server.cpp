#include "Server.hpp"
#include <chrono>

using namespace std::chrono_literals;

void CheckNewConnections(sf::TcpListener &TCPListener, std::list<Lobby> &lobbies, std::list<std::thread> &thrdsComputeLobbies) {
    std::chrono::steady_clock::time_point start, end;
    start = end = std::chrono::steady_clock::now();
    while (true) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(start - end + std::chrono::milliseconds(1000 / TICK_RATE)).count() > 0)
			std::this_thread::sleep_for(start - end + std::chrono::milliseconds(1000 / TICK_RATE));
		start = std::chrono::steady_clock::now();

        sf::TcpSocket* clTCPSock = new sf::TcpSocket;
        if (TCPListener.accept(*clTCPSock) == sf::TcpListener::Error) {
            continue;
        }
        bool isAdded = false;
        for (auto& l : lobbies) {
            if (l.GetSize() == MAX_PLAYERS)
                continue;
            l.Add(clTCPSock);
            sf::Packet pack;
            pack << NEW_PLAYER_CONNECTED << *(l[l.GetLastPlayerID()]);
            l.SendAllTCP(pack);
            isAdded = true;
        }
        if (isAdded)
            continue;
        lobbies.push_back(Lobby());
        thrdsComputeLobbies.push_back(std::thread(&Lobby::ExchangeData, &lobbies.back())); //OK
        lobbies.back().Add(clTCPSock);

        end = std::chrono::steady_clock::now();
    }
}
