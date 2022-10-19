#define DOCTEST_CONFIG_IMPLEMENT
#include <iostream>

#include "doctest.h"

int add_one(int x);
int program();

int main(int argc, char** argv) {
    doctest::Context context;
    context.applyCommandLine(argc, argv);

    int res = context.run();

    if (context.shouldExit()) {
        return res;
    }
    int prg = program();
    return res + prg;
}

int program() {
    std::cout << add_one(2) << std::endl;
    return 0;
}

int add_one(int x) {
    return x + 1;
}

TEST_CASE("add-one") {
    CHECK(add_one(3) == 4);
}
