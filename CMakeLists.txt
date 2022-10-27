cmake_minimum_required(VERSION 3.16)
project(depeche)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O3")

aux_source_directory(src SRC)
add_executable(app ${SRC})

target_include_directories(app PRIVATE src)
target_compile_definitions(app PRIVATE DOCTEST_CONFIG_DISABLE)

add_executable(test
    ${src}
)
target_include_directories(test PRIVATE doctest)
