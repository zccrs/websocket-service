#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

#include "websocketservice.h"

#define SERVERNAME "z-websocket"
#define PORT 624

const QString datapath = QDir::homePath().isEmpty() || QDir::homePath() == "/"
        ? "/root/."+QString(SERVERNAME)+"/data"
        : QDir::homePath() + "/."+QString(SERVERNAME)+"/data";

ZWebSocketService::ZWebSocketService(QObject *parent) :
    QObject(parent),
    m_webSocketServer(new QWebSocketServer(QStringLiteral(SERVERNAME),
                                           QWebSocketServer::NonSecureMode,
                                           this))
{
}

ZWebSocketService::~ZWebSocketService()
{
    m_webSocketServer->close();
}

bool ZWebSocketService::startWebSocketService()
{
    if(m_webSocketServer->isListening())
            return true;

    if(!m_webSocketServer->listen(QHostAddress::Any, PORT)){
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
        connect(socket, &QWebSocket::binaryMessageReceived, [socket](const QByteArray &message){
            qDebug() << "binaryMessage:" << message << socket->peerAddress() << socket->peerName() << socket->peerPort();
        });
        connect(socket, &QWebSocket::textMessageReceived, [socket, this](const QString &message){
            qDebug() << "WebSocket:" << message << socket->peerAddress() << socket->peerName() << socket->peerPort();
            foreach (const QString &str, message.split(',')) {
                QStringList list = str.split('=');
                qDebug() << list;
                if(list.count() == 2){
                    QString name = list.first();
                    QString value = list.last();

                    if(name.toLower() == "call"){
                        QFileInfo info(datapath + "/script/" + value);

                        if(info.isFile()){
                            if(info.absolutePath().contains(datapath)){
                                QFile file(info.absoluteFilePath());
                                if(file.open(QIODevice::ReadOnly)){
                                    socket->sendTextMessage(file.readAll());
                                }else{
                                    sendAlert(socket, "%s file open failed, error string: %s",
                                              qPrintable(info.absoluteFilePath()), qPrintable(file.errorString()));
                                }
                            }else{
                                sendAlert(socket, "Unauthorized access");
                            }
                        }else{
                            sendAlert(socket, "%s not is file or file not exists", qPrintable(info.absoluteFilePath()));
                        }
                    }else{
                        sendAlert(socket, "Does not support the %s command", qPrintable(name));
                    }
                }
            }
        });
    });

    return true;
}

//void ZWebSocketService::sendAlert(QWebSocket *socket, ...)
//{
//    if(!socket)
//        return;

//    QString str = "alert('";

//    va_list ap;
//    va_start(ap, socket);

//    char* ch;

//    while(true){
//        ch = va_arg(ap, char*);
//        if(strcmp(ch, "") == 0)
//            break;
//        str += QString(ch) + " ";
//    };
//    va_end(ap);
//    str.simplified();
//    str.append("')");
//    socket->sendTextMessage(str);
//}

void ZWebSocketService::sendAlert(QWebSocket *socket, const char *message, ...)
{
    qDebug() << "alert message:" << message;

    if(!socket)
        return;

    QString str;

    va_list ap;
    va_start(ap, message);
    str.vsprintf(message, ap);
    va_end(ap);
    socket->sendTextMessage("alert('" + str + "')");
}
