#include <gtest/gtest.h>

#include "User.h"
#include "Room.h"
#include "PSQLUserRepository.h"

std::string conString = "hostaddr=127.0.0.1 port=5432 dbname=chatDB user=postgres password=123";

TEST(UserTest, getUserById) {
    PSQLUserRepository repUser(conString);
    User user1(2000, "karina", "anohina", "karinaaa", "qwerty");
    repUser.addUser(user1);
    User user = repUser.getUserById(2000);

    ASSERT_EQ(user.login, "karinaaa");
    ASSERT_EQ(user.password, "qwerty");
    repUser.deleteUser(user1);
}

TEST(UserTest, getUsersOfRoom) {
    PSQLUserRepository repUser(conString);
    Room room(3, "scientist", 2003);
    std::vector<User> users = repUser.getUsersOfRoom(room);
    ASSERT_EQ(users[0].login, "michael31");
    ASSERT_EQ(users[1].login, "adamsjames");
}

TEST(UserTest, addUser) {
    PSQLUserRepository repUser(conString);
    std::vector<User> users = repUser.getAllUsers();
    int n = users.size() + 1;
    User user(n, "karina", "anohina", "karina", "qwerty");
    repUser.addUser(user);
    User find = repUser.getUserByLogin("karina");
    ASSERT_EQ(find.id, n);
    ASSERT_EQ(find.password, "qwerty");
}

TEST(UserTest, deleteUser) {
    PSQLUserRepository repUser(conString);
    User user(101, "karina", "anohina", "karina", "qwerty");
    repUser.deleteUser(user);
    User find = repUser.getUserByLogin("karina");
    ASSERT_EQ(find.password, "");
}

TEST(UserTest, checkLogin) {
    PSQLUserRepository repUser(conString);
    ASSERT_EQ(repUser.checkLogin("carrmark"), true);
    ASSERT_EQ(repUser.checkLogin("walter"), false);
}

TEST(UserTest, validateUser) {
    PSQLUserRepository repUser(conString);
    ASSERT_EQ(repUser.validateUser("carrmark", "Uj7MKt&(0_"), true);
    ASSERT_EQ(repUser.validateUser("walter38", "S3QsFa!i"), false);
}


int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
    return 0;
}