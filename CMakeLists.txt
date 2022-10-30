cmake_minimum_required(VERSION 3.16)
project(depeche)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3")
set(COVERAGE --coverage)
set(SANITIZERS -fsanitize=address,undefined,leak -fno-sanitize-recover=all -fsanitize-undefined-trap-on-error)

aux_source_directory(src SRC)
add_executable(app ${SRC})

target_include_directories(app PRIVATE src)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(test)
endif()
