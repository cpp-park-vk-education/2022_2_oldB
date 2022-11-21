#include <sstream>

#include "gtest/gtest.h"
#include "server.h"

TEST(serverTest, Integer) {
    EXPECT_EQ(add_one(3), 4);
}
