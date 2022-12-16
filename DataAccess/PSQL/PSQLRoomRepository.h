#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>

#include "User.h"
#include "Room.h"

class PSQLRoomRepository {
public:
    PSQLRoomRepository();
    PSQLRoomRepository(std::string conString) {
        con = std::make_shared<pqxx::connection>(conString);

        if ((*con).is_open())
            std::cout << "Opened database successfully: " << (*con).dbname() << std::endl;
        else
            std::cout << "Can't open database" << std::endl;
    }

    ~PSQLRoomRepository() {
        //con->disconnect();
    }

    std::vector<Room> getAllRooms() {
        std::string sql = "SELECT * from rooms";

        pqxx::nontransaction N(*con);

        pqxx::result res(N.exec(sql));

        std::vector<Room> rooms;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            rooms.push_back(Room(c[0].as<int>(), c[1].as<std::string>()));
        }

        return rooms;
    }

    Room getRoomById(int id) const {
        std::string sql = "SELECT * from rooms WHERE id = " + std::to_string(id);

        pqxx::nontransaction N(*con);

        pqxx::result res(N.exec(sql));

        Room room(res[0][0].as<int>(), res[0][1].as<std::string>());

        return room;
    }

    std::vector<Room> getRoomsByUser(User user) const {
        std::string sql = "select r \
        from rooms r join messages m  on r.id = m.room_id  join users u on u.id = m.user_id \
        where u.id = " + std::to_string(user.id);

        pqxx::nontransaction N(*con);

        pqxx::result res(N.exec(sql));

        std::vector<Room> rooms;

        for (pqxx::result::const_iterator c = res.begin(); c != res.end(); ++c) {
            rooms.push_back(Room(c[0].as<int>(), c[1].as<std::string>()));
        }

        return rooms;
    }

    void addRoom(Room room) {
        std::stringstream sql;

        sql << "INSERT INTO rooms (id, name) "
          "VALUES (";
        sql << room.id << ',' << '\'' << room.name << '\'' << ");";

        pqxx::work W(*con);
        W.exec(sql.str());
        W.commit();
        std::cout << "Records created successfully\n";
    }

    void deleteRoom(Room room) {
        std::string sql = "DELETE from rooms where id = " + std::to_string(room.id);

        pqxx::work W(*con);
        W.exec(sql);
        W.commit();
        std::cout << "Records deleted successfully" << std::endl;
    }

    void updateRoom(Room &room) {
        std::string sql = "UPDATE rooms set name = " + room.name + " \
        where id = " + std::to_string(room.id);

        pqxx::work W(*con);
        W.exec(sql);
        W.commit();
        std::cout << "Records updated successfully" << std::endl;
    }

    bool checkRoom(const std::string &name) const {
        std::string sql = "SELECT * from rooms WHERE name = " + name;

        pqxx::nontransaction N(*con);

        pqxx::result res(N.exec(sql));
        
        return res.size() != 0;
    }

private:
    std::shared_ptr<pqxx::connection> con;
};
