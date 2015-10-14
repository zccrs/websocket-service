#include <QCoreApplication>
#include <QCommandLineParser>

#include "websocketservice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ZWebSocketService server;
    server.startWebSocketService();

    return a.exec();
}

