cmake_minimum_required(VERSION 3.16)
project(depeche)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3")

aux_source_directory(src SRC)
add_executable(app ${SRC})

target_include_directories(app PRIVATE src)

find_package(GTest REQUIRED)
aux_source_directory(test TEST_SRC)
add_executable(test ${TEST_SRC})
target_include_directories(test PUBLIC src)
target_link_libraries(test gtest pthread)
target_compile_options(test PUBLIC --coverage -fsanitize=address,undefined,leak -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error)
target_link_options(test PUBLIC --coverage -fsanitize=address,undefined,leak -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error)
