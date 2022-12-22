cmake_minimum_required(VERSION 3.5)

project(Client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(SRC
        ChatClient.h
        Client.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Message.h
        resource.qrc
        )


add_executable(app
        ${SRC}
        )

target_link_libraries(app
        Qt6::Widgets
        )
