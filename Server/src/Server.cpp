#include "Server.hpp"
#include <chrono>
#include <utility>
#include <iostream>
#include <format>

using namespace std::chrono_literals;

void CheckNewConnections(sf::TcpListener &TCPListener, std::list<std::pair<Lobby*, std::thread>> &listLobbiesThreads) {
    std::chrono::steady_clock::time_point start, end;
    start = end = std::chrono::steady_clock::now();
    sf::UdpSocket UDPWorldServSock, UDPPlayerServSock, UDPMessageServSock;
    UDPWorldServSock.bind(ServWorldUDPPort);
    UDPPlayerServSock.bind(ServPlayerUDPPort);
    UDPPlayerServSock.setBlocking(false);
    UDPMessageServSock.bind(ServMesssageUDPPort);
    UDPMessageServSock.setBlocking(false);
    while (true) {
        if (std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() < 1000 / TICK_RATE)
			std::this_thread::sleep_for(std::chrono::microseconds(1000 / TICK_RATE) - (end - start));
		start = std::chrono::steady_clock::now();

        //sf::TcpSocket *clTCPSock = new sf::TcpSocket;
        sf::TcpSocket clTCPSock;
        for (auto lo = listLobbiesThreads.begin(); lo != listLobbiesThreads.end(); ++lo) {
            if (lo->first->GetSize() <= 0) {
                std::cout << std::format("Lobby {} has been deleted.\n", lo->first->GetID());
                lo->second.join();
                lo = listLobbiesThreads.erase(lo);
            }
        }
        if (TCPListener.accept(clTCPSock) != sf::TcpListener::Done) {
            continue;
        }
        bool isAdded = false;
        for (auto& l : listLobbiesThreads) {
            if (l.first->GetSize() == MAX_PLAYERS)
                continue;
            l.first->Add(clTCPSock, UDPWorldServSock, UDPPlayerServSock, UDPMessageServSock);
            isAdded = true;
        }
        if (isAdded)
            continue;
        Lobby* lob = new Lobby();
        lob->Add(clTCPSock, UDPWorldServSock, UDPPlayerServSock, UDPMessageServSock);
        listLobbiesThreads.push_back(std::make_pair(lob, std::thread(&Lobby::ExchangeData, lob, std::ref(UDPWorldServSock))));

        end = std::chrono::steady_clock::now();
    }
}
