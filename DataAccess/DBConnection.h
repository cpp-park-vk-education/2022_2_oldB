#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <pqxx/pqxx>

class DBConnection {
public:
    DBConnection(std::string conString) {
        con = std::make_shared<pqxx::connection>(conString);

        if ((*con).is_open())
            std::cout << "Opened database successfully: " << (*con).dbname() << std::endl;
        else
            std::cout << "Can't open database" << std::endl;
    }

    //~DBConnection() {(*con).close();}

    std::shared_ptr<pqxx::connection> getCon() {
        return con;
    }

private:
    std::shared_ptr<pqxx::connection> con;
};