cmake_minimum_required(VERSION 3.10)

project("Server")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Wpedantic -Werror")

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/server.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})

add_executable(app ${SRC})

enable_testing()
add_subdirectory(tests)
