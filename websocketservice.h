#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H

#include <QObject>

class QTcpServer;
class QWebSocketServer;
class WebSocketService : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketService(QObject *parent = 0);
    ~WebSocketService();

    bool startTcpService();
    bool startWebSocketService();

private:
    QTcpServer *m_tcpServer;
    QWebSocketServer *m_webSocketServer;
};

#endif // WEBSOCKETSERVICE_H
