#include "Lobby.hpp"
#include "Server.hpp"
#include <list>
#include <SFML/Network.hpp>
#include <thread>
#include <iostream>


int main(int argc, char* argv[]) {
    sf::TcpListener TCPListener;
    TCPListener.listen(TCPPort);
    //TCPListener.setBlocking(false);
    std::cout << sf::IpAddress::getPublicAddress() << std::endl;

    //std::list<Lobby*> lobbies;
    std::list<std::pair<Lobby*, std::thread>> listLobbiesThreads;
    
    std::thread thrdCheckNew(CheckNewConnections, std::ref(TCPListener), std::ref(listLobbiesThreads));

    thrdCheckNew.join();
    return 0;
}