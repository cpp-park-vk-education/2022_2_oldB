#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>

#include "../DBConnection.h"
#include "../Entities/User.h"
#include "../Entities/Room.h"
#include "../Entities/ChatMessage.h"

class PSQLMessageRepository {
public:
    PSQLMessageRepository();
    PSQLMessageRepository(DBConnection conn) {
        con = std::make_shared<DBConnection>(conn);
    }

    std::vector<ChatMessage> getAllMessages() {
        std::string sql = "SELECT * from messages";

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<ChatMessage> messages;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            messages.push_back(ChatMessage(c[0].as<int>(), c[1].as<std::string>(),
                                     c[2].as<int>(), c[3].as<int>()));
        }

        return messages;
    }

    ChatMessage getMessageById(int id) const {
        std::string sql = "SELECT * from messages WHERE id = " + std::to_string(id);

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        ChatMessage message(res[0][0].as<int>(), res[0][1].as<std::string>(),
                        res[0][2].as<int>(), res[0][3].as<int>());

        return message;
    }

    std::vector<ChatMessage> getMessagesOfUser(User user) const {
        std::string sql = "select * from messages \
        where user_id = " + std::to_string(user.id);

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<ChatMessage> messages;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            messages.push_back(ChatMessage(c[0].as<int>(), c[1].as<std::string>(),
                                        c[2].as<int>(), c[3].as<int>()));
        }

        return messages;
    }

    void addMessage(ChatMessage message) {
        std::stringstream sql;

        sql << "INSERT INTO messages (id, text, user_id, room_id) "
          "VALUES (";
        sql << message.id << ',' << '\'' << message.text << '\'' 
        << ',' << message.user_id << ',' << message.room_id << ");";

        pqxx::work W(*(con->getCon()));
        W.exec(sql.str());
        W.commit();
        std::cout << "Records created successfully\n";
    }

    void deleteMessage(ChatMessage message) {
        std::string sql = "DELETE from messages where id = " + std::to_string(message.id);

        pqxx::work W(*(con->getCon()));
        W.exec(sql);
        W.commit();
        std::cout << "Records deleted successfully" << std::endl;
    }

    void updateMessage(ChatMessage &message) {
        std::string sql = "UPDATE messages set text = '" + message.text + "' \
        where id = " + std::to_string(message.id);

        pqxx::work W(*(con->getCon()));
        W.exec(sql);
        W.commit();
        std::cout << "Records updated successfully" << std::endl;
    }

    void addUserToRoom(User user, Room room) {
        std::stringstream sql;

        std::vector<ChatMessage> msgs = getAllMessages();
        int id = msgs.size() + 1;

        sql << "INSERT INTO messages (id, text, user_id, room_id) "
          "VALUES (";
        sql << id << ',' << '\'' << '\'' << ',' << user.id << ',' << room.id << ");";

        pqxx::work W(*(con->getCon()));
        W.exec(sql.str());
        W.commit();
        std::cout << "Records created successfully\n";
    }

private:
    std::shared_ptr<DBConnection> con;
};
