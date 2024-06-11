#include "Player.hpp"

sf::Packet& operator<<(sf::Packet& pack, Player& pl) {
    return pack;
}

void Player::setId(int _id) {
    this->id = _id;
}

int Player::getId() {
    return id;
}
