#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H

#include <QObject>

class QWebSocketServer;
class WebSocketService : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketService(QObject *parent = 0);
    ~WebSocketService();

    bool startWebSocketService();

private:
    QWebSocketServer *m_webSocketServer;
};

#endif // WEBSOCKETSERVICE_H
