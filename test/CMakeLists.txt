cmake_minimum_required(VERSION 3.16)
project(test)

find_package(GTest REQUIRED)
aux_source_directory(. TEST_SRC)
add_executable(${PROJECT_NAME} ${TEST_SRC}  ../src/add_one.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ../src)
target_link_libraries(${PROJECT_NAME} gtest pthread)

target_compile_options(${PROJECT_NAME} PUBLIC ${COVERAGE} ${SANITIZERS})
target_link_options(${PROJECT_NAME} PUBLIC ${COVERAGE} ${SANITIZERS})
