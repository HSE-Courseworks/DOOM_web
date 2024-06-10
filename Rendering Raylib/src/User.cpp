#include "User.hpp"

User::User(const int id, const std::string& nickName) : id(id), nickName(nickName) {}

User::User(const User &other) : id(other.id), nickName(other.nickName) {}

int User::getId() const {
    return id;
}

std::string User::getNickName() const {
    return nickName;
}

void User::setNickName(const std::string& newNickName) {
    nickName = newNickName;
}

bool User::getFlagActive() const {
    return active;
}

void User::setFlagActive(bool flag) {
    active = flag;
}

bool User::getFlagInGame() const {
    return inGame;
}

void User::setFlagInGame(bool flag) {
    inGame = flag;
}