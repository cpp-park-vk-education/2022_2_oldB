#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "Message.h"
#include "ChatClient.h"

using boost::asio::ip::tcp;

void СhatСlient::do_read_body() {
    boost::asio::async_read(socket_, boost::asio::buffer(read_message_.inf(), read_message_.inf_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_message_.decode_text()) {
                std::cout << read_message_.get_username() << ": " << read_message_.get_body();  // выводим прочитанное сообщение на экран
                std::cout << "\n";
                do_read_header();                                                    // сразу же начинаем читать следующее, если оно пришло
            }
            else
                socket_.close();
        });
}

class Client {
public:
    Client() : resolver(io_context) {}
    ~Client() {
        if(connected_to_server) {
            chat_client->close();
            execution_thread.join();
            delete chat_client;
        }
    }

    bool Registration(std::string &username, std::string &password) {
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "2001"));

        Message msg(username, password, Message::registration);
        msg.encode();
        boost::asio::write(socket, boost::asio::buffer(msg.data(), msg.length()));

        boost::asio::read(socket, boost::asio::buffer(msg.data(), Message::header_length));
        msg.decode_header();
        boost::asio::read(socket, boost::asio::buffer(msg.inf(), msg.inf_length()));
        msg.decode_text();

        if (msg.body_length())
            return true;
        return false;
    }

    bool Authorization(std::string &username, std::string &password) {
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "2001"));

        Message msg(username, password, Message::authorization);
        msg.encode();
        boost::asio::write(socket, boost::asio::buffer(msg.data(), msg.length()));

        boost::asio::read(socket, boost::asio::buffer(msg.data(), Message::header_length));
        msg.decode_header();
        boost::asio::read(socket, boost::asio::buffer(msg.inf(), msg.inf_length()));
        msg.decode_text();


        ports.clear();
        for (int i = 0; i < msg.get_body().size() / 4; i++) {
            char tmp[Message::lenght_length + 1] = "";
            for (int j = 0; j < Message::lenght_length; j++)
                tmp[j] = msg.get_body()[i * 4 + j];

            ports[i] = std::atoi(tmp);
        }

        if (!ports.size() && !msg.get_body()[0])
            return false;
        return true;
    }

    bool GetUsersPorts(std::vector<int>& users_ports) {
        if (ports.size()) {
            users_ports = ports;
            return true;
        }

        return false;
    }

    bool ConnectToChat(int port) {
        try {
            endpoints = resolver.resolve("127.0.0.1", std::to_string(port));
            chat_client  = new СhatСlient(io_context, endpoints);
            execution_thread = std::thread([this](){ io_context.run();});
            connected_to_server = true;

            return true;
        }
        catch (std::exception& e) {
            return false;
        }
    }

    bool DisconnectToChat(int port) {
        if (connected_to_server == true) {
            chat_client->close();
            execution_thread.join();
            io_context.restart();
            delete chat_client;
            connected_to_server = false;
            return true;
        }
        return false;
    }

    bool WriteMessage(Message &message) {
        if(connected_to_server == true) {
            chat_client->write(message);
            return true;
        }
        else
            return false;
    }

private:
    std::string& Hashing(std::string &password);

private:
    boost::asio::io_context io_context;
    tcp::resolver resolver;
    boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints;
    СhatСlient *chat_client;
    std::thread execution_thread;
    bool connected_to_server = false;

    std::string username;
    std::vector<int> ports;
};

int main(int argc, char* argv[]) {
    try {
        Client c;

        std::string name = "fedor";
        std::string txt = "msg";
        Message msg(name, txt);

        c.ConnectToChat(2001);
        c.WriteMessage(msg);
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}

