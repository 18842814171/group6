#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QObject>
#include <QThread>
#include <client.h>


#define FLOOD_API 6667

class listenThread: public QThread
{
    Q_OBJECT
public:
    listenThread(QTcpSocket *s,QObject *parent = nullptr);
    void run();
signals:
    void sendToWidget(QString&);
public slots:
    void clientInfoSlot();
private:
    Client *client;
};

#endif // LISTENTHREAD_H
