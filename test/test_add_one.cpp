#include <gtest/gtest.h>
#include <sstream>

#include "add_one.h"

TEST(AddOne, Integer) {
    EXPECT_EQ(add_one(3), 4);
}
