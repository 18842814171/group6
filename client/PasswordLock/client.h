#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>


#define SUUCESS_CONNECT 1
#define FAILED_CONNECT 0

class Client:public QWidget
{
    Q_OBJECT
public:
    Client(QString,quint16);        // 构造方法1：IP，端口
    Client(QTcpSocket *s);          // 构造方法2：直接传入一个socket
    void connectToServer();         // 此函数应与构造方法1使用，绑定连接成功与失败，开始连接
    void sendMsg(QString);          // 发送信息
    void recvMsg(QString &);        // 接收信息
    void recvMsgAsUDP(QString&);    // 用于像UDP一样持续接收信息
    QTcpSocket *socket;

public slots:
    void clientInfoSlot_succ();     // 登录成功槽
    void clientInfoSlot_fail();     // 登录失败槽


signals:
    void success_switch();           // 登录成功信号
    void failed_switch();            // 登录失败信号


private:
    QString IP;
    quint16 port;
};

#endif // CLIENT_H
