#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>

#include "../DBConnection.h"
#include "../Entities/User.h"
#include "../Entities/Room.h"

class PSQLRoomRepository {
public:
    PSQLRoomRepository();
    PSQLRoomRepository(DBConnection conn) {
        con = std::make_shared<DBConnection>(conn);
    }

    std::vector<Room> getAllRooms() {
        std::string sql = "SELECT * from rooms";

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<Room> rooms;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            rooms.push_back(Room(c[0].as<int>(), c[1].as<std::string>(), c[2].as<int>()));
        }

        return rooms;
    }

    Room getRoomById(int id) const {
        std::string sql = "SELECT * from rooms WHERE id = " + std::to_string(id);

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        Room room(res[0][0].as<int>(), res[0][1].as<std::string>(), res[0][2].as<int>());

        return room;
    }

    Room getRoomByPort(int port) const {
        std::string sql = "SELECT * from rooms WHERE port = " + std::to_string(port);

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        Room room(res[0][0].as<int>(), res[0][1].as<std::string>(), res[0][2].as<int>());

        return room;
    }

    std::vector<Room> getRoomsByUser(User user) const {
        std::string sql = "select r \
        from rooms r join messages m  on r.id = m.room_id  join users u on u.id = m.user_id \
        where u.id = " + std::to_string(user.id);

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<Room> rooms;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            rooms.push_back(Room(c[0].as<int>(), c[1].as<std::string>(), c[2].as<int>()));
        }

        return rooms;
    }

    std::vector<int> getPortsByUser(User user) const {
        std::string sql = "select r.port \
        from rooms r join messages m  on r.id = m.room_id  join users u on u.id = m.user_id \
        where u.id = " + std::to_string(user.id);

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<int> ports;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            ports.push_back(c[0].as<int>());
        }

        return ports;
    }

    std::vector<int> getPortsByUsername(std::string username) const {
        std::string sql = "select r.port \
        from rooms r join messages m  on r.id = m.room_id  join users u on u.id = m.user_id \
        where u.id = '" + username + "'";

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));

        std::vector<int> ports;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            ports.push_back(c[0].as<int>());
        }

        return ports;
    }

    void addRoom(Room room) {
        std::stringstream sql;

        sql << "INSERT INTO rooms (id, name) "
          "VALUES (";
        sql << room.id << ',' << '\'' << room.name << '\'' << ");";

        pqxx::work W(*(con->getCon()));
        W.exec(sql.str());
        W.commit();
        std::cout << "Records created successfully\n";
    }

    void deleteRoom(Room room) {
        std::string sql = "DELETE from rooms where id = " + std::to_string(room.id);

        pqxx::work W(*(con->getCon()));
        W.exec(sql);
        W.commit();
        std::cout << "Records deleted successfully" << std::endl;
    }

    void updateRoom(Room &room) {
        std::string sql = "UPDATE rooms set name = '" + room.name + "' \
        where id = " + std::to_string(room.id);

        pqxx::work W(*(con->getCon()));
        W.exec(sql);
        W.commit();
        std::cout << "Records updated successfully" << std::endl;
    }

    bool checkRoom(const std::string &name) const {
        std::string sql = "SELECT * from rooms WHERE name = '" + name + "'";

        pqxx::nontransaction N(*(con->getCon()));

        pqxx::result res(N.exec(sql));
        
        return res.size() != 0;
    }

private:
    std::shared_ptr<DBConnection> con;
};
