#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <string>
#include <vector>

#include "Message.h"
#include "ChatClient.h"

#include "ui_mainwindow.h"

using boost::asio::ip::tcp;

class Client {
public:
    Client(Ui::MainWindow *_ui) : resolver(io_context), ui(_ui) {}
    ~Client() {
        if (connected_to_server) {
            chat_client->close();
            execution_thread.join();
            delete chat_client;
        }
    }

    //system ports (2001)

    bool Registration(std::string& username, std::string& password) {
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "2001"));

        Message msg(username, Hashing(password), Message::registration);
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

        Message msg(username, Hashing(password), Message::authorization);
        msg.encode();
        boost::asio::write(socket, boost::asio::buffer(msg.data(), msg.length()));

        boost::asio::read(socket, boost::asio::buffer(msg.data(), Message::header_length));
        msg.decode_header();
        boost::asio::read(socket, boost::asio::buffer(msg.inf(), msg.inf_length()));
        msg.decode_text();

        if (msg.body_length() == 1 && msg.get_body()[0] == '0')
            return false;

        ports.clear();
        for (size_t i = 0; i < msg.get_body().size() / Message::lenght_length; i++) {
            char tmp[Message::lenght_length + 1] = "";
            for (size_t j = 0; j < Message::lenght_length; j++)
                tmp[j] = msg.get_body()[i * Message::lenght_length + j];

            ports.push_back(std::atoi(tmp));
        }

        this->username = msg.get_username();
        return true;
    }

    bool СreateNewRoom(int port) {
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "2001"));

        std::string tmp_port = std::to_string(port);
        Message msg(username, tmp_port, Message::create_port);
        msg.encode();
        boost::asio::write(socket, boost::asio::buffer(msg.data(), msg.length()));

        boost::asio::read(socket, boost::asio::buffer(msg.data(), Message::header_length));
        msg.decode_header();
        boost::asio::read(socket, boost::asio::buffer(msg.inf(), msg.inf_length()));
        msg.decode_text();

        if (msg.get_body()[0]) {
            ports.push_back(port);
            return true;
        }
        return false;
    }

    bool ErrorMessageWasSend() {
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "2001"));

        std::string empty_str = "";
        Message msg(username, empty_str, Message::error_msg);
        msg.encode();
        boost::asio::write(socket, boost::asio::buffer(msg.data(), msg.length()));

        boost::asio::read(socket, boost::asio::buffer(msg.data(), Message::header_length));
        msg.decode_header();
        boost::asio::read(socket, boost::asio::buffer(msg.inf(), msg.inf_length()));
        msg.decode_text();

        if (msg.get_body()[0]) {
            return true;
        }
        return false;
    }

    bool GetErrorStatistics(int &statistics) {
        boost::asio::io_context io_context;

        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket, resolver.resolve("127.0.0.1", "2001"));

        std::string empty_str = "";
        Message msg(username, empty_str, Message::get_statistic);
        msg.encode();
        boost::asio::write(socket, boost::asio::buffer(msg.data(), msg.length()));

        boost::asio::read(socket, boost::asio::buffer(msg.data(), Message::header_length));
        msg.decode_header();
        boost::asio::read(socket, boost::asio::buffer(msg.inf(), msg.inf_length()));
        msg.decode_text();

        if (msg.get_body().size()) {
            char tmp[Message::lenght_length + 1] = "";
            for (size_t j = 0; j < Message::lenght_length; j++)
                tmp[j] = msg.get_body()[j];
            for (size_t j = 0; j < Message::lenght_length; j++)
                tmp[j] = msg.get_body()[Message::lenght_length + j];

            int all_msg = std::atoi(tmp);
            int err_msg = std::atoi(tmp);
            double tmp_statistics = err_msg / all_msg * 100;
            statistics = tmp_statistics;

            return true;
        }
        return false;
    }

    //users ports

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
            chat_client = new ChatClient(io_context, endpoints, ui);
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
    std::string& Hashing(std::string& password) {
        return password;
    }

private:
    boost::asio::io_context io_context;
    tcp::resolver resolver;
    boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints;
    ChatClient* chat_client;
    std::thread execution_thread;
    bool connected_to_server = false;

    std::string username;
    std::vector<int> ports;
    Ui::MainWindow *ui;
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
//            c.WriteMessage(txt);
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
