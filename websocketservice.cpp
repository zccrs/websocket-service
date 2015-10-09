#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "websocketservice.h"

WebSocketService::WebSocketService(QObject *parent) :
    QObject(parent),
    m_webSocketServer(new QWebSocketServer(QStringLiteral("Test Server"),
                                           QWebSocketServer::NonSecureMode,
                                           this))
{
}

WebSocketService::~WebSocketService()
{
    m_webSocketServer->close();
}

bool WebSocketService::startWebSocketService()
{
    if(!m_webSocketServer->listen(QHostAddress::Any, 624)){
        qDebug() << "WebSocketService: error: " << m_webSocketServer->errorString() << m_webSocketServer->error();
        return false;
    }else{
        qDebug() << "WebSocketService: OK";
    }
    connect(m_webSocketServer, &QWebSocketServer::newConnection, [this]{
        QWebSocket *socket = m_webSocketServer->nextPendingConnection();
        qDebug() << "WebSocketService: new connect:" << socket->peerAddress() << socket->peerName() << socket->peerPort();

        connect(socket, &QWebSocket::disconnected, [socket]{
            qDebug() << "WebSocketService: disconnected: " << socket->peerAddress() << socket->peerName() << socket->peerPort();
            socket->deleteLater();
        });
        connect(socket, &QWebSocket::textMessageReceived, [](const QString &message){
            qDebug() << "WebSocket:" << message;
        });
    });

    return true;
}
