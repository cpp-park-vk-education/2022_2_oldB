TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpqxx -lpq

SOURCES += \
        main.cpp
