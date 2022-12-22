#pragma once

#include <string>

class ChatMessage {
public:
    int id;
    std::string text;
    //std::string date;
    //int status;
    int user_id;
    int room_id;

    ChatMessage() {}
    ChatMessage(int id, std::string text, int user_id, int room_id)
    : id(id), text(text), user_id(user_id), room_id(room_id) {}
};