#ifndef USER_HPP
#define USER_HPP

#include <string>

class User
{
public:
    User(const int id, const std::string &nickName);
    User(const User &other);

    void setNickName(const std::string &newNickName);
    void setFlagActive(bool flag);
    void setFlagInGame(bool flag);
    void setId(int _id);

    int getId() const;
    std::string getNickName() const;
    bool getFlagActive() const;
    bool getFlagInGame() const;

private:
    int id;
    bool inGame = false, active = true;
    std::string nickName;
};

#endif