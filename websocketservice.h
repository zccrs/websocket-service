#ifndef WEBSOCKETSERVICE_H
#define WEBSOCKETSERVICE_H

#include <QObject>

class WebSocketService : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketService(QObject *parent = 0);

signals:

public slots:
};

#endif // WEBSOCKETSERVICE_H
