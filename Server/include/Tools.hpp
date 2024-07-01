#ifndef SERVER_TOOLS_HPP
#define SERVER_TOOLS_HPP

//#include "Player.hpp"
#include "PlayerInfo.hpp"
#include "Lobby.hpp"
#include <SFML/Network.hpp>

void ExchangeData(PlayerInfo *pl, Lobby *lobby, sf::UdpSocket& UDPMessagesock, sf::UdpSocket& UDPPlayersock);

#endif