cmake_minimum_required(VERSION 3.16)
project(depeche)

set(src
    src/main.cpp
)

add_executable(app
    ${src}
)
target_include_directories(app PRIVATE doctest)
target_compile_definitions(app PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(test
    ${src}
)
target_include_directories(test PRIVATE doctest)
