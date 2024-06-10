#ifndef USER_HPP
#define USER_HPP

#include <string>

class User {
public:
    User(const int id, const std::string& nickName);
    User(const User &other);
    int getId() const;
    std::string getNickName() const;
    void setNickName(const std::string& newNickName);
    bool getFlagActive() const;
    void setFlagActive(bool flag);
    bool getFlagInGame() const;
    void setFlagInGame(bool flag);

private:
    int id; bool inGame = false, active = true;
    std::string nickName;
};

#endif