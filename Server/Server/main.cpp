#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "../../chat_message.hpp"

#define MAIN_SERVER 2001
//#define RETURN_SERVER 2001

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<ChatMessage> chat_message_queue;

//----------------------------------------------------------------------

class ChatParticipant {
public:
    virtual ~ChatParticipant() {}
    virtual void deliver(const ChatMessage& message) = 0;
};

typedef std::shared_ptr<ChatParticipant> chat_participant_ptr;

//----------------------------------------------------------------------

class ChatRoom {
public:
    void join(chat_participant_ptr participant) {  // пользователь добавляется в список пользователей комнаты
        participants_.insert(participant);
        for (auto message : recent_messages_)
            participant->deliver(message);
    }

    void leave(chat_participant_ptr participant) {
        participants_.erase(participant);
    }

    void deliver(const ChatMessage& message) {
        recent_messages_.push_back(message);
        //db.add_messages();

        while (recent_messages_.size() > max_recent_messages)
            recent_messages_.pop_front();

        for (auto participant : participants_)
            participant->deliver(message);
    }

private:
    std::set<chat_participant_ptr> participants_;   // участники комнаты
    enum { max_recent_messages = 100 };
    chat_message_queue recent_messages_;  // хранится 100 сообщений в комнате
};

//----------------------------------------------------------------------

class ChatSession : public ChatParticipant, public std::enable_shared_from_this<ChatSession> {  // взаимодействие с конкретным клиентов
public:
    ChatSession(tcp::socket socket, ChatRoom& room) : socket_(std::move(socket)), room_(room) {}

    void start() {
        room_.join(shared_from_this());  // добавляем его в комнату
        do_read_header();                // начинаем читать его сообщения
    }

    void deliver(const ChatMessage& message) {
        bool write_in_progress = !write_messages_.empty();
        write_messages_.push_back(message);

        //std::cout << message.data()[9] << message.data()[10] << message.data()[11] << message.data()[12] << message.data()[13] << message.data()[14] << message.data()[15] << std::endl;

        if (!write_in_progress)
            do_write();
    }

private:
    void do_read_header() {  // читаем заголовок сообщения
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_message_.data(), ChatMessage::header_length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && read_message_.decode_header()) {
                    do_read_body();
                }
                else
                    room_.leave(shared_from_this());
            });
    }

    void do_read_body() {  // читаем тело сообщения
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_message_.inf(), read_message_.inf_length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec && read_message_.decode_text()) {
                    room_.deliver(read_message_);
                    do_read_header();
                }
                else
                    room_.leave(shared_from_this());
            });
    }

    void do_write() {  // возвращаем информацию одному клиенту
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_messages_.front().data(), write_messages_.front().length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec) {
                    write_messages_.pop_front();
                    if (!write_messages_.empty()) {
                        do_write();
                    }
                }
                else
                    room_.leave(shared_from_this());
            });
    }

    tcp::socket socket_;
    ChatRoom& room_;
    ChatMessage read_message_;
    chat_message_queue write_messages_;
};

//----------------------------------------------------------------------

class ChatServer {
public:
    ChatServer(boost::asio::io_service& io_service, const tcp::endpoint& endpoint) : acceptor_(io_service, endpoint), socket_(io_service) {
        do_accept();
    }

private:
    void do_accept() {                                                                 // сервер сидит на сокете, ждет подключение
        acceptor_.async_accept(socket_,
            [this](boost::system::error_code ec)
            {
                if (!ec)                                                               // если получилось подключиться, запускается сессия
                    std::make_shared<ChatSession>(std::move(socket_), room_)->start();

                do_accept();                                                           // уходим в прослушивание
            });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
    ChatRoom room_;
};

//----------------------------------------------------------------------

int main(int argc, char* argv[]) {
    try {
        //if (argc < 2)
        //{
        //    std::cerr << "Usage: chat_server <port> [<port> ...]\n";
        //    return 1;
        //}

        boost::asio::io_service io_service;


        //MAIN_SERVER
        tcp::endpoint ep(tcp::v4(), std::atoi("2001"));
        ChatServer servers(io_service, ep);

        io_service.run();  // удержание до завершения
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
