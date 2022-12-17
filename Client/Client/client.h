#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<ChatMessage> chat_message_queue;

class Сlient
{
public:
    Сlient(boost::asio::io_service& io_service, tcp::resolver::iterator ep_iter, std::string& username) 
        : io_service_(io_service), socket_(io_service), username_(username) {
        do_connect(ep_iter);  // подключаем пользователя к серверу
    }

    void write(const ChatMessage& message) {
        io_service_.post(
            [this, message]()
            {
                bool write_in_progress = !write_messages_.empty();
                write_messages_.push_back(message);  // добавляем сообщение в конец очереди
                if (!write_in_progress)              // если очередь пустая, выполняем отправку
                    do_write();
            });
    }

    void close() {
        io_service_.post([this]() { socket_.close(); });   // задание на закрытие сокета добавляется в очередь сервиса
    }

    void command(const ChatMessage& message)
    {
        if (message == ":list:")
            std::cout << "list\n";
            //write(ChatMessage(":list"));
        else if (std::strncmp(message.data(), ":username:", 8) == 0)
        {
            std::cout << "New username: " << std::endl;
            std::cin >> username_;
            //ChatMessage tell_username(":username" + username_);
            //write(tell_username);
        }
        else if (message == ":help:")
        {
            std::cout << "List of commands: " << std::endl;
            std::cout << ":username: -> Change username." << std::endl;
            std::cout << ":list: -> Show list of online users." << std::endl;
            std::cout << ":exit: -> Close the program." << std::endl;
        }
        else
            std::cout << "The command was not found. Write :help: for more information." << std::endl;
    }

private:
    void do_connect(const tcp::resolver::iterator ep_iter) {
        boost::asio::async_connect(socket_, ep_iter,
            [this](boost::system::error_code ec, tcp::resolver::iterator)
            {
                if (!ec) {
                    std::cout << "You have connected to the server "
                        << socket_.remote_endpoint().address() << ":"
                        << socket_.remote_endpoint().port() << std::endl;

                    std::string username = ":username:" + username_;
                    ChatMessage message;                       // создаем сообщение
                    message.body_length(std::strlen(username.c_str()));    // заполняем его данные
                    std::memcpy(message.body(), username.c_str(), message.body_length());
                    message.encode_header();

                    write(message);

                    do_read_header();             // после подключения к серверу запускаем считывание сообщения
                }
            });
    }

    void do_read_header() {
        boost::asio::async_read(socket_, boost::asio::buffer(read_message_.data(), ChatMessage::header_length),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && read_message_.decode_header())  // если смогли прочитать размер сообщения из заголовка, 
                    do_read_body();                        // приступаем к чтению сообщения
                else
                    socket_.close();
            });
    }

    void do_read_body() {
        boost::asio::async_read(socket_, boost::asio::buffer(read_message_.body(), read_message_.body_length()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec) {
                    std::cout.write(read_message_.body(), read_message_.body_length());  // выводим прочитанное сообщение на экран
                    std::cout << "\n";
                    do_read_header();                                                    // сразу же начинаем читать следующее, если оно пришло
                }
                else
                    socket_.close();
            });
    }

    void do_write() {
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_messages_.front().data(), write_messages_.front().length()),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec) {
                    write_messages_.pop_front();               // получаем первое сообщение из очереди
                    if (!write_messages_.empty())              // если очередь не пустая, запускаем еще раз
                        do_write();
                }
                else
                    socket_.close();
            });
    }

private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    ChatMessage read_message_;
    chat_message_queue write_messages_;
    std::string username_;

};

#endif //CLIENT_HPP

