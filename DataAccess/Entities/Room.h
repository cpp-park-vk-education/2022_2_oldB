#pragma once

#include <string>

class Room {
public:
    int id;
    std::string name;

    Room() {}
    Room(int id, std::string name)
    : id(id), name(name) {}
};