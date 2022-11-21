#pragma once

#include <string>

class Message {
public:
    int ID;
    int UserID;
    std::string opponents_nickname;
    int status;
    std::string title;
    std::string text;
    std::string date;
};