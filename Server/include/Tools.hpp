#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "Player.hpp"
#include "PlayerInfo.hpp"
#include "Lobby.hpp"
#include <SFML/Network.hpp>

void ExchangeData(PlayerInfo *pl, Lobby *lobby);
sf::Packet& operator<<(sf::Packet& pack, PlayerInfo& pl);

#endif