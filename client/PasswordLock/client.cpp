#include "client.h"

Client::Client(QString IP,quint16 port):
    IP(IP),port(port)
{
    socket = new QTcpSocket;
}

Client::Client(QTcpSocket *s)
{
    socket = s;
}




void Client::connectToServer()
{
    connect(socket,&QTcpSocket::connected,this,&Client::clientInfoSlot_succ);
    connect(socket,&QTcpSocket::errorOccurred,this,&Client::clientInfoSlot_fail);
    socket->connectToHost(QHostAddress(IP),port);
}

void Client::clientInfoSlot_succ()
{
    emit success_switch();
}

void Client::clientInfoSlot_fail()
{
    emit failed_switch();
}




void Client::sendMsg(QString msg)
{
        QByteArray ba;
        msg = QString::number(msg.length()).rightJustified(4, '0') + msg;
        ba.append(msg.toUtf8());
        socket->write(ba);
}


void Client::recvMsg(QString& msg)
{
        QByteArray ba;
        if (socket->waitForReadyRead()) {
            qDebug()<<"client接收函数内";
            ba = socket->read(4);
            QString str = QString::fromUtf8(ba);
            bool ok; // 用于标识转换是否成功
            int value = str.toInt(&ok);
            ba = socket->read(value);
            msg = QString::fromUtf8(ba);
            qDebug()<<"msg="<<msg;
        }
}


void Client::recvMsgAsUDP(QString& msg)
{
        QByteArray ba;
        ba = socket->read(4);
        QString str = QString::fromUtf8(ba);
        bool ok; // 用于标识转换是否成功
        int value = str.toInt(&ok);
        ba = socket->read(value);
        msg = QString::fromUtf8(ba);
}

