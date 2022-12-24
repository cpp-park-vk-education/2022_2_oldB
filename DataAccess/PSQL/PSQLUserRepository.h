#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>

#include "../DBConnection.h"
#include "../Entities/User.h"
#include "../Entities/Room.h"
#include "PSQLMessageRepository.h"

class PSQLUserRepository {
public:
    PSQLUserRepository();
    PSQLUserRepository(DBConnection conn) {
        con = std::make_shared<DBConnection>(conn);
    }

    std::vector<User> getAllUsers() {
        std::string sql = "SELECT * from users";

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<User> users;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            users.push_back(User(c[0].as<int>(), c[1].as<std::string>(), c[2].as<std::string>(),
                        c[3].as<std::string>(), c[4].as<std::string>()));
        }

        return users;
    }

    User getUserById(int id) const {
        try {
            std::string sql = "SELECT * from users WHERE id = " + std::to_string(id);

            pqxx::nontransaction N(*(con->getCon()));

            pqxx::result res(N.exec(sql));

            User user(res[0][0].as<int>(), res[0][1].as<std::string>(), res[0][2].as<std::string>(),
                                res[0][3].as<std::string>(), res[0][4].as<std::string>());

            return user;
        } catch (const std::exception &e) {
            return User();
        }

    }

    User getUserByLogin(std::string login) const {
        try {
            std::string sql = "SELECT * from users WHERE login = '" + login + "'";

            pqxx::nontransaction N(*(con->getCon()));

            pqxx::result res(N.exec(sql));

            User user(res[0][0].as<int>(), res[0][1].as<std::string>(), res[0][2].as<std::string>(),
                                res[0][3].as<std::string>(), res[0][4].as<std::string>());

            return user;
        } catch (const std::exception &e) {
            return User();
        }
    }

    std::vector<User> getUsersOfRoom(Room room) const {
        std::string sql = "select u.id, u.name, u.surname, u.login, u.password \
        from rooms r join messages m  on r.id = m.room_id  join users u on u.id = m.user_id \
        where r.id = " + std::to_string(room.id);

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<User> users;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            users.push_back(User(c[0].as<int>(), c[1].as<std::string>(), c[2].as<std::string>(),
                        c[3].as<std::string>(), c[4].as<std::string>()));
        }

        return users;
    }

    bool addUser(User user) {
        std::stringstream sql;

        if(checkLogin(user.login))
            return false;

        sql << "INSERT INTO users (id, name, surname, login, password) "
          "VALUES (";
        sql << user.id << ',' << '\'' << user.name << '\''
            << ',' << '\'' << user.surname << '\'' << ',' << '\'' 
            << user.login << '\'' << ',' << '\'' << user.password << '\'' << ");";

        pqxx::work W(*(con->getCon()));
        W.exec(sql.str());
        W.commit();
        std::cout << "Records created successfully\n";
        
        return true;
    }

    void deleteUser(User user) {
        std::string sql = "DELETE from users where id = " + std::to_string(user.id);

        pqxx::work W(*(con->getCon()));
        W.exec(sql);
        W.commit();
        std::cout << "Records deleted successfully" << std::endl;
    }

    void deleteUserFromRoom(User user, Room room) {
        std::string sql = "DELETE from messages where user_id = " + std::to_string(user.id)
        + "and room_id = " + std::to_string(room.id);

        pqxx::work W(*(con->getCon()));
        W.exec(sql);
        W.commit();
        std::cout << "Records deleted successfully" << std::endl;
    }

    void updateUser(User &user) {
        std::string sql = "UPDATE users set login = " + user.login + ", \
        password = " + user.password + " \
        where id = " + std::to_string(user.id);

        pqxx::work W(*(con->getCon()));
        W.exec(sql);
        W.commit();
        std::cout << "Records updated successfully" << std::endl;
    }

    bool checkLogin(const std::string &login) const {
        try {
            std::string sql = "SELECT * from users WHERE login = '" + login + "'";

            pqxx::nontransaction N(*(con->getCon()));

            pqxx::result res(N.exec(sql));
            return res.size() != 0;
        } catch (const std::exception &e) {
            return false;
        }
        
    }

    bool validateUser(const std::string &login, const std::string &password) const {
        std::string sql = "SELECT * from users WHERE login = '" + login + "'";

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));
        
        return res[0][4].as<std::string>() == password;
    }


private:
    std::shared_ptr<DBConnection> con;
};
