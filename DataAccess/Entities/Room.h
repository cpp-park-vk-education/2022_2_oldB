#pragma once

#include <string>

class Room {
public:
    int id;
    std::string name;
    int port;

    Room() {}
    Room(int id, std::string name, int port)
    : id(id), name(name), port(port) {}
};