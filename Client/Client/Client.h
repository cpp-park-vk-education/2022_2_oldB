#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "Message.h"
#include "ChatClient.h"
#include "mainwindow.h"


using boost::asio::ip::tcp;

//void СhatСlient::do_read_body() {
//    boost::asio::async_read(socket_, boost::asio::buffer(read_message_.inf(), read_message_.inf_length()),
//        [this](boost::system::error_code ec, std::size_t /*length*/)
//        {
//            if (!ec && read_message_.decode_text()) {
//                std::cout << read_message_.get_username() << ": " << read_message_.get_body();  // выводим прочитанное сообщение на экран
//                std::cout << "\n";

//                // вызов м-да, который выводит сообщения на экран

//                do_read_header();                                                    // сразу же начинаем читать следующее, если оно пришло
//            }
//            else
//                socket_.close();
//        });
//}

class Client {
public:
    Client() : resolver(io_context) {}
    ~Client() {
        if (connected_to_server) {
            chat_client->close();
            execution_thread.join();
            delete chat_client;
        }
    }

    bool Registration(std::string& username, std::string& password) {
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

        if (msg.get_body()[0])
            return true;
        return false;
    }

    bool Authorization(std::string& username, std::string& password) {
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

        if (msg.body_length() == 1 && msg.get_body()[0] == '0')
            return false;

        ports.clear();
        for (int i = 0; i < msg.get_body().size() / Message::lenght_length; i++) {
            char tmp[Message::lenght_length + 1] = "";
            for (int j = 0; j < Message::lenght_length; j++)
                tmp[j] = msg.get_body()[i * Message::lenght_length + j];

            ports.push_back(std::atoi(tmp));
        }

        username = msg.get_username();
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
            chat_client = new СhatСlient(io_context, endpoints, this->ui);
            execution_thread = std::thread([this]() { io_context.run(); });
            connected_to_server = true;

            return true;
        }
        catch (std::exception& e) {
            return false;
        }
    }

    bool DisconnectToChat() {
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

    bool WriteMessage(std::string &message) {
            Message msg(username, message);
            if(connected_to_server == true) {
                chat_client->write(msg);
                return true;
            }
            else
                return false;
        }

private:
    std::string& Hashing(std::string& password);

private:
    boost::asio::io_context io_context;
    tcp::resolver resolver;
    boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints;
    СhatСlient* chat_client;
    std::thread execution_thread;
    bool connected_to_server = false;

    std::string username;
    std::vector<int> ports;
};

//int main(int argc, char* argv[]) {
//    try {
//        Client c;
//
//        std::string name = "fedor";
//        std::string password = "ted12345";
//        c.Authorization(name, password);
//
//
//        std::vector<int> ports;
//        c.GetUsersPorts(ports);
//
//
//        c.ConnectToChat(ports[0]);
//
//
//        std::string txt;
//        while (std::cin >> txt)
//        {
//            Message msg(name, txt, Message::send_message);
//            c.WriteMessage(msg);
//        }
//
//        c.DisconnectToChat();
//    }
//    catch (std::exception& e) {
//        std::cout << "Exception: " << e.what() << "\n";
//    }
//
//    return 0;
//}
