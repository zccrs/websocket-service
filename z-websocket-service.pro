QT += core network websockets
QT -= gui

TARGET = z-websocket-service
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    websocketservice.cpp

HEADERS += \
    websocketservice.h

#DESTDIR         = $$_PRO_FILE_PWD_/

target.path = /usr/bin/

INSTALLS += target

OTHER_FILES += z-websocket.service
