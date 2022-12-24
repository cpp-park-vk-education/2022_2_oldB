#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>

#include "../../Client/Client/Message.h"

#include "../../DataAccess/DBConnection.h"
#include "../../DataAccess/PSQL/PSQLUserRepository.h"
#include "../../DataAccess/PSQL/PSQLRoomRepository.h"
#include "../../DataAccess/PSQL/PSQLMessageRepository.h"


#define MAIN_SERVER 2001
#define ALL_CHAT_SERVERS {2001, 2002, 2003, 2004, 2005}
#define SERVERS_COUNT 5

using boost::asio::ip::tcp;

//----------------------------------------------------------------------

typedef std::deque<Message> chat_message_queue;

//----------------------------------------------------------------------

std::string conString = "hostaddr=127.0.0.1 port=5432 dbname=chatDB user=postgres password=fedyk228";
DBConnection con(conString);
PSQLUserRepository repUser(con);
PSQLRoomRepository repRoom(con);
PSQLMessageRepository repMessage(con);

class ChatParticipant {
public:
    virtual ~ChatParticipant() {}
    virtual void deliver(const Message& message) = 0;
};

typedef std::shared_ptr<ChatParticipant> chat_participant_ptr;

//----------------------------------------------------------------------

class ChatRoom {
public:
    ChatRoom(int _rooms_port) : rooms_port(_rooms_port) {}
    void join(chat_participant_ptr participant) {  // пользователь добавляется в список пользователей комнаты
        participants_.insert(participant);

        // DB add user to room
        //if (rooms_port != MAIN_SERVER) {
        //    User user = repUser.getUserByLogin(participant->get_username());
        //    Room room = repRoom.getRoomByPort(rooms_port);
        //    repMessage.addUserToRoom(user, room);
        //}

        for (auto message : recent_messages_)
            participant->deliver(message);
    }

    void leave(chat_participant_ptr participant) {
        participants_.erase(participant);

        // DB del user from room
        //User user = repUser.getUserByLogin(participant->get_username());
        //Room room = repRoom.getRoomByPort(rooms_port);
        //repUser.deleteUserFromRoom(user, room);
    }

    void deliver(Message& message) {
        recent_messages_.push_back(message);

        //DB add messages
        std::vector<ChatMessage> msgs = repMessage.getAllMessages();
        int id = msgs.size() + 1;
        User user = repUser.getUserByLogin(message.get_username());
        Room room = repRoom.getRoomByPort(rooms_port);
        ChatMessage chat_message(id, message.get_body(), user.id, room.id);
        repMessage.addMessage(chat_message);

        while (recent_messages_.size() > max_recent_messages)
            recent_messages_.pop_front();

        for (auto participant : participants_)
            participant->deliver(message);
    }

private:
    std::set<chat_participant_ptr> participants_;   // участники комнаты
    enum { max_recent_messages = 100 };
    chat_message_queue recent_messages_;  // хранится 100 сообщений в комнате
    int rooms_port;
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
                        std::vector<User> users = repUser.getAllUsers();
                        int id = users.size() + 1;
                        User user(id, "user", "user", read_message_.get_username(), read_message_.get_body());
                        int is_correct = repUser.addUser(user);

                        Message msg;
                        msg.set_username(read_message_.get_username());
                        msg.set_type(Message::registration);
                        if (is_correct) { /* if add complete */
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
                        int is_correct = repUser.validateUser(read_message_.get_username(), read_message_.get_body());

                        Message msg;
                        msg.set_username(read_message_.get_username());
                        msg.set_type(Message::authorization);
                        //if (1 /* if user in DB */) {
                        if (is_correct) {

                            //DB get users ports as std::vector<int>
                            std::vector<int> ports = repRoom.getPortsByUsername(read_message_.get_username());

                            //std::vector<int> ports = { 2001 };
                            msg.convert_ports_to_string(ports);
                        }
                        else {
                            msg.set_body(std::to_string(false));
                        }

                        msg.encode();
                        do_write_sistem_msg(msg);
                    }
                    else if (read_message_.get_type() == Message::create_port) {

                        //DB если комната есть, добавить пользователя в нее

                        User user = repUser.getUserByLogin(read_message_.get_username());
                        Room room = repRoom.getRoomByPort(stoi(read_message_.get_body()));
                        repMessage.addUserToRoom(user, room);

                        Message msg;
                        msg.set_username(read_message_.get_username());
                        msg.set_type(Message::create_port);

                        if (1 /*получилось добавить*/ ) {
                            msg.set_body(std::to_string(true));
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
    ChatServer(boost::asio::io_service& io_service, const tcp::endpoint& endpoint, int port) : acceptor_(io_service, endpoint), socket_(io_service), room_(port) {
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

int main() {
    try {
        //DB get all rooms
        std::vector<Room> rooms = repRoom.getAllRooms();

        boost::asio::io_service io_service;

        std::list<ChatServer> servers;
        for (size_t i = 0; i < rooms.size(); ++i) {
            tcp::endpoint endpoint(tcp::v4(), rooms[i].port);
            servers.emplace_back(io_service, endpoint, rooms[i].port);
        }

        io_service.run();  // удержание до завершения
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
