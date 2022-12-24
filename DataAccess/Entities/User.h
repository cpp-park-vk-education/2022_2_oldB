#pragma once

#include <string>

class User {
public:
    int id;
    std::string name;
    std::string surname;
    std::string login;
    std::string password;
    int mistakes;

    User() {}
    User(int id, std::string name, std::string surname, std::string login, std::string password, int mistakes)
    : id(id), name(name), surname(surname), login(login), password(password), mistakes(mistakes) {}
};