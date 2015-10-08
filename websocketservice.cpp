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
    m_tcpServer(new QTcpServer(this)),
    m_webSocketServer(new QWebSocketServer(QStringLiteral("Test Server"),
                                           QWebSocketServer::NonSecureMode,
                                           this))
{
    startWebSocketService();
    startTcpService();
}

WebSocketService::~WebSocketService()
{
    m_tcpServer->close();
    m_webSocketServer->close();
}

bool WebSocketService::startTcpService()
{
    if(!m_tcpServer->listen(QHostAddress::Any, 80)){
        qDebug() << "TcpService: error: " << m_tcpServer->errorString();
        return false;
    }else{
        qDebug() << "TcpService: OK";
    }
    connect(m_tcpServer, &QTcpServer::newConnection, [this]{
        QTcpSocket *socket = m_tcpServer->nextPendingConnection();
        qDebug() << "TcpService: new connect:" << socket->peerAddress() << socket->peerName() << socket->peerPort();

        connect(socket, &QTcpSocket::readyRead, [socket]{
            QByteArray path_and_command = socket->readAll().split('\r').first().split(' ')[1];
            QByteArray file_path = path_and_command.split('?').first();
            QByteArray command = path_and_command == file_path ? "" : path_and_command.split('?').last();
            QFileInfo fileInfo(QDir::homePath() + "/.websocket-service/data"+file_path);

            if(!fileInfo.exists()
                || !fileInfo.absoluteFilePath().contains(QDir::homePath() + "/.websocket-service/data/")){
                return;
            }

            QFile file;

            qDebug() << fileInfo.absoluteFilePath() << command.split('&');

            if(fileInfo.isFile()){
                file.setFileName(fileInfo.absoluteFilePath());
            }else if(fileInfo.isDir()){
                file.setFileName(fileInfo.filePath().append("default.html"));
            }

            qDebug() << "TcpSocket: open file path:" << file.fileName();

            if(file.open(QIODevice::ReadOnly)){
                socket->write(file.readAll());
            }else{
                qDebug() << "TcpSocket: open file " << file.fileName() << "error:" << file.errorString();
            }

            socket->close();
        });
        connect(socket, &QTcpSocket::disconnected, [socket]{
            qDebug() << "TcpService: disconnected: " << socket->peerAddress() << socket->peerName() << socket->peerPort();
            socket->deleteLater();
        });
    });

    return true;
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
