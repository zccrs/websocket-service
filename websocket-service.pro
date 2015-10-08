QT += core network websockets
QT -= gui

TARGET = websocket-service
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    websocketservice.cpp

HEADERS += \
    websocketservice.h

RESOURCES +=

