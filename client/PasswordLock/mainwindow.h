#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpressionValidator>
#include <qanimationstackedwidget.h>
#include "menu.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_login_clicked();
    void on_btn_sureadd_clicked();
    void on_btn_login_switch_clicked();
    void on_btn_new_switch_clicked();
    void login_test();
    void login_return();
    void register_return();

private:
    Ui::MainWindow *ui;
    QString username;
    QString password;
    QString surepass;

    Client *client;
    bool m_Drag;                //记录鼠标是否按下
    QPoint m_DragPosition;      //记录鼠标位置
    //重写三个鼠标事件来实现窗口移动
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent* event);
    void closeWindow();
    void setTime();


};

#endif // MAINWINDOW_H
