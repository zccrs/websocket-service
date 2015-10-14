#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H

#include <QObject>

class QWebSocket;
class QWebSocketServer;
class ZWebSocketService : public QObject
{
    Q_OBJECT
public:
    explicit ZWebSocketService(QObject *parent = 0);
    ~ZWebSocketService();

    bool startWebSocketService();

private:
    QWebSocketServer *m_webSocketServer;

    //void sendAlert(QWebSocket *socket, ...);
    void sendAlert(QWebSocket *socket, const char *message, ...);
};

#endif // WEBSOCKETSERVICE_H
