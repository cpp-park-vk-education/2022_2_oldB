#pragma once

#include <string>

class Users {
public:
    int ID;
    int RoleID;
    std::string nickname;
    std::string login;
    std::string password;
    std::string close_key;
};