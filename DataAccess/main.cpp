#include "PSQLUserRepository.h"

int main (void) {
   std::string conString = "hostaddr=127.0.0.1 port=5432 dbname=chatDB user=postgres password=123";
   PSQLUserRepository repUser(conString);
   User el = repUser.getUserByLogin("karina");

   std::cout << "ID = " << el.id << std::endl;
   std::cout << "Name = " << el.name << std::endl;
   std::cout << "Surname = " << el.surname << std::endl;
   std::cout << "Login = " << el.login << std::endl;
   std::cout << "Password = " << el.password << std::endl;
}
