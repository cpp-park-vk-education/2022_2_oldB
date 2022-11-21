cmake_minimum_required(VERSION 3.16)
project(ServerTests)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 11)
file(GLOB_RECURSE SRC "${PROJECT_SOURCE_DIR}/server_test.cpp")
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})