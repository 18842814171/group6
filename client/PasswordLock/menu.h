#ifndef MENU_H
#define MENU_H
#include <windows.h>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QTableView>
#include <QMenu>
#include <time.h>
#include <QAction>
#include <listenthread.h>
#include <client.h>
#include <QStandardItemModel>
#include <QTcpServer>


#define TARGET_IP "127.0.0.1"
#define TARGET_PORT 8001

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(quint16,QString,QString,QWidget *parent = nullptr);
    void threadSlot(QString);
    void newclientConnect();
    QMenu *tableviewMenue;//菜单
    QMenu *tableviewMenue2;
    QMenu *tableviewMenue3;
    QAction *Action1;//菜单项
    QAction *Action2;
    QAction *Action3;
    QTableView *tableview;
    ~Widget();


private slots:
    void on_showButton_clicked();
    void on_attentionButton_clicked();
    void on_warnButton_clicked();
    void on_meButton_clicked();
    void onTableClicked(const QModelIndex &index);      // 点击选中
    void appendAction_triggered();                      // 右键菜单1
    void prependAction_triggered();                     // 右键菜单2
    void removeAction_triggered();                     // 右键菜单2
    void Men_Slot(QPoint p);                            // 右键菜单槽函数
    void forwarn_succ();
    void forwarn_fail();
    void Men_Slot2(QPoint p);
    void Men_Slot3(QPoint p);

    void on_timeSubmit_clicked();

    void on_priceSubmit_clicked();

    void on_exchangePassword_clicked();

private:
    Ui::Widget *ui;
    QString password;
    QString username;
    bool m_Drag;                //记录鼠标是否按下
    QPoint m_DragPosition;      //记录鼠标位置
    QTcpServer *server;
    Client *submitSocket;
    QStandardItemModel *model;
    QStandardItemModel *Model;
    int index_=0;
    //重写三个鼠标事件来实现窗口移动
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MENU_H
