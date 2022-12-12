#include <iostream>
#include "client.h"

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_service service;

        tcp::resolver resolver(service);
        auto ep_iter = resolver.resolve({ "127.0.0.1", "2002" }); // настройка конечной точки для сокета
        std::string username = "karina";
        Сlient client(service, ep_iter, username); //////

        std::thread thread([&service]() { service.run(); });  // для каждого нового клиента новый поток

        char line[ChatMessage::max_body_length + 1];
        while (std::cin.getline(line, ChatMessage::max_body_length + 1)) {
            ChatMessage message;                       // создаем сообщение
            message.body_length(std::strlen(line));    // заполняем его данные
            std::memcpy(message.body(), line, message.body_length());
            message.encode_header();

            client.write(message);                     // передаем на сервер
        }

        client.close();
        thread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}