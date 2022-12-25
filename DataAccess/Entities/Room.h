#pragma once

#include <string>

class Room {
public:
    int id;
    std::string name;
    int port;
    std::string password;

    Room() {}
    Room(int id, std::string name, int port, std::string password)
    : id(id), name(name), port(port), password(password) {}
};