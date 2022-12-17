#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>

#include "User.h"
#include "Room.h"
#include "Message.h"

class PSQLMessageRepository {
public:
    PSQLMessageRepository();
    PSQLMessageRepository(std::string conString) {
        con = std::make_shared<pqxx::connection>(conString);

        if ((*con).is_open())
            std::cout << "Opened database successfully: " << (*con).dbname() << std::endl;
        else
            std::cout << "Can't open database" << std::endl;
    }

    ~PSQLMessageRepository() {
        //con->disconnect();
    }

    std::vector<Message> getAllMessages() {
        std::string sql = "SELECT * from messages";

        pqxx::nontransaction N(*con);

        pqxx::result res(N.exec(sql));

        std::vector<Message> messages;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            messages.push_back(Message(c[0].as<int>(), c[1].as<std::string>(),
                                     c[2].as<int>(), c[3].as<int>()));
        }

        return messages;
    }

    Message getMessageById(int id) const {
        std::string sql = "SELECT * from messages WHERE id = " + std::to_string(id);

        pqxx::nontransaction N(*con);

        pqxx::result res(N.exec(sql));

        Message message(res[0][0].as<int>(), res[0][1].as<std::string>(),
                        res[0][2].as<int>(), res[0][3].as<int>());

        return message;
    }

    std::vector<Message> getMessagesOfUser(User user) const {
        std::string sql = "select * from messages \
        where user_id = " + std::to_string(user.id);

        pqxx::nontransaction N(*con);

        pqxx::result res(N.exec(sql));

        std::vector<Message> messages;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            messages.push_back(Message(c[0].as<int>(), c[1].as<std::string>(),
                                        c[2].as<int>(), c[3].as<int>()));
        }

        return messages;
    }

    void addMessage(Message message) {
        std::stringstream sql;

        sql << "INSERT INTO messages (id, text, user_id, room_id) "
          "VALUES (";
        sql << message.id << ',' << '\'' << message.text << '\'' 
        << ',' << message.user_id << ',' << message.room_id << ");";

        pqxx::work W(*con);
        W.exec(sql.str());
        W.commit();
        std::cout << "Records created successfully\n";
    }

    void deleteMessage(Message message) {
        std::string sql = "DELETE from messages where id = " + std::to_string(message.id);

        pqxx::work W(*con);
        W.exec(sql);
        W.commit();
        std::cout << "Records deleted successfully" << std::endl;
    }

    void updateMessage(Message &message) {
        std::string sql = "UPDATE messages set text = '" + message.text + "' \
        where id = " + std::to_string(message.id);

        pqxx::work W(*con);
        W.exec(sql);
        W.commit();
        std::cout << "Records updated successfully" << std::endl;
    }

    void addUserToRoom(User user, Room room) {
        std::stringstream sql;

        std::vector<Message> msgs = getAllMessages();
        int id = msgs.size() + 1;

        sql << "INSERT INTO messages (id, text, user_id, room_id) "
          "VALUES (";
        sql << id << ',' << '\'' << '\'' << ',' << user.id << ',' << room.id << ");";

        pqxx::work W(*con);
        W.exec(sql.str());
        W.commit();
        std::cout << "Records created successfully\n";
    }

private:
    std::shared_ptr<pqxx::connection> con;
};
