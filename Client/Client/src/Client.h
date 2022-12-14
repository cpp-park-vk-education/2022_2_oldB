#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "Message.h"

using boost::asio::ip::tcp;

class Client {
public:
    bool GetUsersChats();
    bool Authorization(std::string &username, std::string &password);
    bool ConnectToChat(int port);
    bool WriteMessage(std::string &message);

private:
    std::string& Hashing(std::string &password);

private:
    std::string username;
    std::vector<int> ports;
};

//int main(int argc, char* argv[]) {
//    try {
//        boost::asio::io_service service;
//
//        tcp::resolver resolver(service);
//        auto ep_iter = resolver.resolve({ "127.0.0.1", "2002" }); // настройка конечной точки для сокета
//        СhatСlient client(service, ep_iter);
//
//        std::string username = "fedor";
//
//        std::thread thread([&service]() { service.run(); });  // для каждого нового клиента новый поток
//
//        std::string line;
//        while (std::cin >> line) {
//            ChatMessage message(username, line, 0);     // заполняем его данные
//
//            client.write(message);                     // передаем на сервер
//        }
//
//        client.close();
//        thread.join();
//    }
//    catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << "\n";
//    }
//
//    return 0;
//}
