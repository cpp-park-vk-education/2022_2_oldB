#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "Message.h"

using boost::asio::ip::tcp;

typedef std::deque<Message> chat_message_queue;

class СhatСlient {
public:
    СhatСlient(boost::asio::io_service& io_service, tcp::resolver::iterator ep_iter) : io_service_(io_service), socket_(io_service) {
        do_connect(ep_iter);  // подключаем пользователя к серверу
    }

    void write(Message& message) {
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

private:
    void do_connect(tcp::resolver::iterator ep_iter) {
        boost::asio::async_connect(socket_, ep_iter,
            [this](boost::system::error_code ec, tcp::resolver::iterator)
            {
                if (!ec)
                    do_read_header();                      // псле подключения к серверу запускаем считывание сообщения
            });
    }

    void do_read_header() {
        boost::asio::async_read(socket_, boost::asio::buffer(read_message_.data(), Message::header_length),
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && read_message_.decode_header())  // если смогли прочитать размер сообщения из заголовка,
                    do_read_body();                        // приступаем к чтению сообщения
                else
                    socket_.close();
            });
    }

    void do_read_body() {
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
    Message read_message_;
    chat_message_queue write_messages_;  // очередь из сообщений для определенного сервера (одна на всех клиентов)
};
