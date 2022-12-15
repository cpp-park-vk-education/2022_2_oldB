cmake_minimum_required(VERSION 3.24)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/andrey/Qt/6.4.1/gcc_64/lib/cmake")
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(SRC
        ChatClient.h
        Client.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        Message.h
        )

set(TEST
        )


add_executable(app
        ${SRC}
        ${TEST})

target_link_libraries(
        app ${GTEST_LIBRARIES} Threads::Threads)

target_link_libraries(app
        Qt6::Widgets
        )

enable_testing()





#cmake_minimum_required(VERSION 3.5)
#
#project(Client VERSION 0.1 LANGUAGES CXX)
#
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
#set(CMAKE_PREFIX_PATH "/home/andrey/Qt/6.4.1/gcc_64/lib/cmake")
#
#find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
#
#set(PROJECT_SOURCES
#        src/main.cpp
#        src/Client.h
#        src/ChatClient.h
#        src/Message.h
#        src/mainwindow.cpp
#        src/mainwindow.h
#        src/mainwindow.ui
#)
#
#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(Client
#        MANUAL_FINALIZATION
#        ${PROJECT_SOURCES}
#    )
## Define target properties for Android with Qt 6 as:
##    set_property(TARGET Client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
##                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
## For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
#else()
#    if(ANDROID)
#        add_library(Client SHARED
#            ${PROJECT_SOURCES}
#        )
## Define properties for Android with Qt 5 after find_package() calls as:
##    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    else()
#        add_executable(Client
#            ${PROJECT_SOURCES}
#        )
#    endif()
#endif()
#
#target_link_libraries(Client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
#
#set_target_properties(Client PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#    MACOSX_BUNDLE TRUE
#    WIN32_EXECUTABLE TRUE
#)
#
#install(TARGETS Client
#    BUNDLE DESTINATION .
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
#
#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_finalize_executable(Client)
#endif()
