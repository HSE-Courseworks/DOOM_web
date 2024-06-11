#include "Lobby.hpp"
#include "Server.hpp"
#include <list>
#include <SFML/Network.hpp>
#include <thread>
#include <iostream>


int main(int argc, char* argv[]) {
    sf::TcpListener TCPListener;
    TCPListener.listen(TCPPort, sf::IpAddress::getLocalAddress());
    std::cout << sf::IpAddress::getLocalAddress();

    std::list<Lobby> lobbies;
    std::list<std::thread> thrdsComputeLobbies;
    
    std::thread thrdCheckNew(CheckNewConnections, std::ref(TCPListener), std::ref(lobbies), std::ref(thrdsComputeLobbies));

    thrdCheckNew.join();
    return 0;
}