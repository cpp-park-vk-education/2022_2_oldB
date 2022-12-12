#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class ChatMessage
{
public:
    static constexpr std::size_t header_length = 12;
    static constexpr std::size_t max_body_length = 512;

    ChatMessage() : operation_code_(0), username_length_(0), body_length_(0) {}

    /*ChatMessage(const char* msg) {
        copy(msg);
    }

    ChatMessage(const std::string& msg) {
        copy(msg.c_str());
    }

    ChatMessage(const ChatMessage& msg) {
        copy(msg.data_);
    }

    void copy(const char* msg) {
        body_length(std::strlen(msg));    // заполняем его данные
        std::memcpy(this->body(), msg, this->body_length());
        encode_header();
    }

    void copy(const std::string& msg) {
        copy(msg.c_str());
    }*/

    const char* data() const {
        return data_;
    }

    char* data() {
        return data_;
    }

    std::size_t length() const {
        return header_length + username_length_ + body_length_;
    }

    const int operation_code() const {
        return operation_code_;
    }

    int operation_code() {
        return operation_code_;
    }

    void operation_code(int code) {
        operation_code_ = code;
    }

    const char* username() const {
        return data_ + header_length;
    }

    char* username() {
        return data_ + header_length;
    }

    std::size_t username_length() const {
        return username_length_;
    }

    void username_length(std::size_t new_length) {
        username_length_ = new_length;
        if (body_length_ + username_length_ > max_body_length)
            username_length_ = max_body_length - body_length_;
    }

    const char* body() const {
        return data_ + header_length + username_length_;
    }

    char* body() {
        return data_ + header_length + username_length_;
    }

    std::size_t body_length() const {
        return body_length_;
    }

    void body_length(std::size_t new_length) {
        body_length_ = new_length;
        if (body_length_ + username_length_ > max_body_length)
            body_length_ = max_body_length - username_length_;
    }

    bool decode_header()
    {
        char header1[4 + 1] = "";
        char header2[4 + 1] = "";
        char header3[4 + 1] = "";
        strncat_s(header1, data_, 4);
        strncat_s(header2, data_ + 4, 4);
        strncat_s(header3, data_ + 8, 4);
        operation_code_ = std::atoi(header1);
        username_length_ = std::atoi(header2);
        body_length_ = std::atoi(header3);
        if (body_length_ + username_length_ > max_body_length)
        {
            username_length_ = 0;
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        char header[header_length + 1] = "";
        sprintf_s(header, "%4d%4d%4d", operation_code_, static_cast<int>(username_length_), static_cast<int>(body_length_));
        std::memcpy(data_, header, header_length);
    }

    bool operator==(const char* str) const
    {
        return (std::strcmp(data_, str) == 0);
    }


    bool operator==(const std::string& str) const
    {
        return (*this == str.c_str());
    }


    bool operator==(const ChatMessage& msg) const
    {
        return (*this == msg.data_);
    }


    /*friend std::ostream& operator<<(std::ostream& os, const Message& msg)
    {
        os << msg.data() << std::endl;

        return os;
    }*/

private:
    int operation_code_;
    std::size_t username_length_;
    std::size_t body_length_;
    char data_[header_length + max_body_length];
};

#endif // CHAT_MESSAGE_HPP