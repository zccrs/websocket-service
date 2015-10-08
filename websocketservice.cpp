#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QDebug>

#include "websocketservice.h"

WebSocketService::WebSocketService(QObject *parent) : QObject(parent)
{
    QTcpServer *tcpServer = new QTcpServer(this);
    if(!tcpServer->listen(QHostAddress::Any, 80)){
        qDebug() << "listen 80 port error: " << tcpServer->errorString();
    }else{
        qDebug() << "listen 80 port ok";
    }
    connect(tcpServer, &QTcpServer::newConnection, [tcpServer]{
        QTcpSocket *socket = tcpServer->nextPendingConnection();
        qDebug() << "new connecnt " << socket;
        QFile file(":/default.html");
        if(file.open(QIODevice::ReadOnly)){
            socket->write(file.readAll());
        }
    });
}

