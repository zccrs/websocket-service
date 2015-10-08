#include <QCoreApplication>

#include "websocketservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebSocketService service;
    Q_UNUSED(service);

    return a.exec();
}

