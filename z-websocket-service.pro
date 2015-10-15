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

OTHER_FILES += z-websocket.service

service.path = /etc/systemd/system/
service.files = z-websocket.service

INSTALLS += target service
