#include "listenthread.h"



listenThread::listenThread(QTcpSocket *s,QObject *parent)
    : QThread{parent}
{
    client = new Client{s};
}

void listenThread::run()
{
    connect(client->socket, &QTcpSocket::readyRead, this, &listenThread::clientInfoSlot);
}

void listenThread::clientInfoSlot()             // 当可以读取数据时触发此函数，此函数用于通知客户端显示信息
{
    QString msg;
    client->recvMsgAsUDP(msg);
    emit sendToWidget(msg);
}
