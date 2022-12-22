#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "../../Client/Client/Message.h"

#define MAIN_SERVER 2001
#define ALL_CHAT_SERVERS {2001, 2002, 2003, 2004, 2005}
#define SERVERS_COUNT 5

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<Message> chat_message_queue;

//----------------------------------------------------------------------

class ChatParticipant {
public:
    virtual ~ChatParticipant() {}
    virtual void deliver(const Message& message) = 0;
};

typedef std::shared_ptr<ChatParticipant> chat_participant_ptr;

//----------------------------------------------------------------------

class ChatRoom {
public:
    void join(chat_participant_ptr participant) {  // пользователь добавляется в список пользователей комнаты
        participants_.insert(participant);

        // DB add user to room

        for (auto message : recent_messages_)
            participant->deliver(message);
    }

    void leave(chat_participant_ptr participant) {
        participants_.erase(participant);

        // DB del user from room

    }

    void deliver(const Message& message) {
        recent_messages_.push_back(message);

        //DB add messages

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
    ChatSession(tcp::socket socket, ChatRoom& room) : socket_(std::move(socket)), room_(room) {
    }

    void start() {
        room_.join(shared_from_this());  // добавляем его в комнату
        do_read_header();                // начинаем читать его сообщения
    }

    void deliver(const Message& message) {
        bool write_in_progress = !write_messages_.empty();
        write_messages_.push_back(message);

        if (!write_in_progress)
            do_write();
    }

private:
    void do_read_header() {  // читаем заголовок сообщения
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_message_.data(), Message::header_length),
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
                    if (read_message_.get_type() == Message::registration) {

                        //DB Add new user read_message_.username (username) and read_message_.body (password)

                        Message msg;
                        msg.set_username(read_message_.get_username());
                        msg.set_type(Message::registration);
                        if (1 /* if add complete */) {
                            msg.set_body(std::to_string(true));
                        }
                        else {
                            msg.set_body(std::to_string(false));
                        }

                        msg.encode();
                        do_write_sistem_msg(msg);
                    }
                    else if (read_message_.get_type() == Message::authorization) {

                        //DB Check user in db by read_message_.username (username) and read_message_.body (password)

                        Message msg;
                        msg.set_username(read_message_.get_username());
                        msg.set_type(Message::authorization);
                        if (read_message_.get_body() == "123" /* if user in DB */) {

                            //DB get users ports as std::vector<int>

                            std::vector<int> ports = { 2001 };
                            msg.convert_ports_to_string(ports);
                        }
                        else {
                            msg.set_body(std::to_string(false));
                        }

                        msg.encode();
                        do_write_sistem_msg(msg);
                    }
                    else if (read_message_.get_type() == Message::send_message) {
                        room_.deliver(read_message_);
                    }

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
                if (ec)
                    room_.leave(shared_from_this());
            });
    }

    void do_write_sistem_msg(Message &msg) {  // возвращаем информацию одному клиенту
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
            boost::asio::buffer(msg.data(), msg.length()),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (ec) {
                    room_.leave(shared_from_this());
                }
            });
    }

    tcp::socket socket_;
    ChatRoom& room_;
    Message read_message_;
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
		
		//DB get all rooms
		
		
        boost::asio::io_service io_service;

        //MAIN_SERVER
        //tcp::endpoint ep(tcp::v4(), MAIN_SERVER);
        //ChatServer main_servers(io_service, ep);


        int ports[SERVERS_COUNT] = ALL_CHAT_SERVERS;
        std::list<ChatServer> servers;
        for (int i = 0; i < SERVERS_COUNT; ++i) {
            tcp::endpoint endpoint(tcp::v4(), ports[i]);
            servers.emplace_back(io_service, endpoint);
        }

        io_service.run();  // удержание до завершения
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
