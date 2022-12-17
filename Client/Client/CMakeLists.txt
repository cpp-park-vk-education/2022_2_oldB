cmake_minimum_required(VERSION 3.24)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/andrey/Qt/6.4.1/gcc_64/lib/cmake")

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(SRC
        ChatClient.h
        Client.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Message.h
        )


add_executable(app
        ${SRC}
        )

target_link_libraries(app
        Qt6::Widgets
        )
