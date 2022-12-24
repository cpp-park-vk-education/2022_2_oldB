#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

class Message {
public:
    enum { type_length = 1, lenght_length = 5, max_username_length = 20, max_body_length = 512, header_length = type_length + 2 * lenght_length };
    enum { registration = 0, authorization = 1, send_message = 2 };

    Message()
        : type(send_message), username(""), body(""), body_length_(0), username_length_(0), message("") {}
    Message(std::string& _username, std::string& _body, int _type = send_message)
        : type(_type), username(_username), body(_body), body_length_(0), username_length_(0), message("") {
        encode();
    }
    Message(const Message& other) {
        type = other.type;
        username = other.username;
        body = other.body;
        username_length_ = other.username_length_;
        body_length_ = other.body_length_;

        for (int i = 0; i < type_length + 2 * lenght_length + username_length_ + body_length_; i++)
            message[i] = other.message[i];
    }

    const char* data() const {
        return message;
    }

    char* data() {
        return message;
    }

    const char* inf() const {
        return message + header_length;
    }

    char* inf() {
        return message + header_length;
    }

    int get_type() {
        return type;
    }

    std::string& get_username() {
        return username;
    }

    std::string& get_body() {
        return body;
    }

    std::size_t length() const {
        return type_length + 2 * lenght_length + username_length_ + body_length_;
    }

    std::size_t inf_length() const {
        return body_length_ + username_length_;
    }

    std::size_t body_length() const {
        return body_length_;
    }

    std::size_t username_length() const {
        return username_length_;
    }

    void set_type(const int type) {
        this->type = type;
    }

    bool set_username(const std::string& new_username) {
        if (new_username.size() > max_username_length)
            return false;
        this->username = new_username;

        return true;
    }

    bool set_body(const std::string& new_body) {
        if (new_body.size() > max_body_length)
            return false;
        this->body = new_body;

        return true;
    }

    bool decode_header() {
        if (!decode_message_type())
            return false;

        if (!decode_lenght())
            return false;

        return true;
    }

    bool decode_text() {
        decode_inf();

        return true;
    }

    void encode() {
        encode_message_type();
        encode_lenght();
        encode_inf();
    }

    void convert_ports_to_string(std::vector<int>& ports) {
        std::string res;
        res.resize(lenght_length * ports.size());
        for (int i = 0; i < ports.size(); i++) {
            size_t t = 10;
            int tmp = ports[i];
            for (int j = lenght_length - 1; j >= 0; j--) {
                res[i * 4 + j] = std::to_string(tmp % t)[0];
                tmp -= tmp % t;
                t *= 10;
            }
        }

        body = res;
    }

private:
    int type;
    std::string username;
    std::string body;

    char message[type_length + 2 * lenght_length + max_username_length + max_body_length];
    int username_length_;
    int body_length_;

private:
    bool decode_message_type() {
        if (message[0] == '0')
            type = registration;
        else if (message[0] == '1')
            type = authorization;
        else if (message[0] == '2')
            type = send_message;
        else
            return false;

        return true;
    }

    bool decode_lenght() {
        char tmp[lenght_length + 1];
        tmp[lenght_length] = '\0';
        for (int i = 0; i < lenght_length; i++)
            tmp[i] = message[type_length + i];

        username_length_ = std::atoi(tmp);
        if (!username_length_)
            return false;

        for (int i = 0; i < lenght_length; i++)
            tmp[i] = message[type_length + lenght_length + i];

        body_length_ = std::atoi(tmp);
        if (!body_length_)
            return false;

        return true;
    }

    void decode_inf() {
        username.clear();
        for (int i = 0; i < username_length_; i++)
            username.push_back(message[type_length + 2 * lenght_length + i]);

        body.clear();
        for (int i = 0; i < body_length_; i++)
            body.push_back(message[type_length + 2 * lenght_length + username_length_ + i]);
    }

    void encode_message_type() {
        if (type == registration)
            message[0] = '0';
        else if (type == authorization)
            message[0] = '1';
        else if (type == send_message)
            message[0] = '2';
    }

    void encode_lenght() {
        size_t t = 10;
        int tmp = username.size();
        for (int i = lenght_length - 1; i >= 0; i--) {
            message[type_length + i] = std::to_string(tmp % t)[0];
            tmp -= tmp % t;
            t *= 10;
        }
        username_length_ = username.size();

        t = 10;
        tmp = body.size();
        for (int i = lenght_length - 1; i >= 0; i--) {
            message[type_length + lenght_length + i] = std::to_string(tmp % t)[0];
            tmp -= tmp % t;
            t *= 10;
        }
        body_length_ = body.size();
    }

    void encode_inf() {
        for (int i = 0; i < username.size(); i++)
            message[type_length + 2 * lenght_length + i] = username[i];

        for (int i = 0; i < body.size(); i++)
            message[type_length + 2 * lenght_length + username.size() + i] = body[i];
    }
};

#endif // CHAT_MESSAGE_HPP