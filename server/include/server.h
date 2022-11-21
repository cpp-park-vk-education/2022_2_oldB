#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QWebSocketServer>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    virtual ~Server();

signals:

public slots:
};

#endif // SERVER_H