#include "PSQLUserRepository.h"
#include "PSQLRoomRepository.h"
#include "DBConnection.h"

int main (void) {
   std::string conString = "hostaddr=127.0.0.1 port=5432 dbname=chatDB user=postgres password=123";
   DBConnection con(conString);
   PSQLUserRepository repUser(con);
   User el = repUser.getUserByLogin("karina");

   std::cout << "ID = " << el.id << std::endl;
   std::cout << "Name = " << el.name << std::endl;
   std::cout << "Surname = " << el.surname << std::endl;
   std::cout << "Login = " << el.login << std::endl;
   std::cout << "Password = " << el.password << std::endl;

   User user = repUser.getUserById(1);
   PSQLRoomRepository repRoom(con);
   std::vector<int> ports = repRoom.getPortsByUser(user);
   for (auto &port: ports)
      std::cout << port << "\n";

   Room room(3, "scientist", 2003);
   std::vector<User> users = repUser.getUsersOfRoom(room);
   std::cout << users[0].login << " = michael31\n";
   std::cout << users[1].login << " = adamsjames\n";
}
