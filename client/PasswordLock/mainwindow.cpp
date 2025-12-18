#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);        // 设置无窗口框架边界
    setAttribute(Qt::WA_TranslucentBackground);     // 设置背景透明
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedWidget->setAnimationMode(U2D);
    ui->stackedWidget->setAnimationDuration(300);
    ui->stackedWidget_2->setAnimationDuration(300);
    ui->stackedWidget->StartStackedWidgetAnimation(1,0);
    ui->stackedWidget_2->setCurrentIndex(0);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);                    // 设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(Qt::black);            // 设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(20);                  // 设定阴影的模糊半径，数值越大越模糊
    ui->frame->setGraphicsEffect(effect);
    setTime();
    connect(ui->closeButton,&QPushButton::clicked,this,&MainWindow::closeWindow);
}


// 设置首页问候
void MainWindow::setTime()
{
    time_t timep;
    time(&timep); //获取从1970至今过了多少秒，存入time_t类型的timep
    int time_now_hour = localtime(&timep)->tm_hour;//用localtime将秒数转化为struct tm结构体
    if(time_now_hour>=0&&time_now_hour<12)ui->helloLabel->setText("Good Morning");
    else if(time_now_hour>=12&&time_now_hour<18)ui->helloLabel->setText("Good Afternoon");
    else if(time_now_hour>=18&&time_now_hour<24)ui->helloLabel->setText("Good Evening");
    qDebug()<<"当前时间："<<time_now_hour;
}


MainWindow::~MainWindow()
{
    delete ui;
    qDebug()<<"析构MainWindow";
}



//注册
void MainWindow::on_btn_sureadd_clicked()
{

    password=ui->lineEdit_newpass->text();
    surepass=ui->lineEdit_surepass->text();
    username = ui->lineEdit_newname->text();
    QRegularExpression exp("[a-zA-Z0-9-_]+@[a-zA-Z0-9-_]+\\.[a-zA-Z]+");//正则表达式
    QRegularExpressionValidator validator(exp);
    int pos = 0;
    QValidator::State result = validator.validate(username,pos);
    if (result != QValidator::Acceptable)
    {
        ui->lineEdit_newname->clear();
        ui->lineEdit_newpass->clear();
        ui->lineEdit_surepass->clear();
        ui->label_signal_2->setText("邮箱格式不正确");
        return;
    }
    if(password!=surepass)
    {
        ui->lineEdit_newpass->clear();
        ui->lineEdit_surepass->clear();
        ui->label_signal_2->setText("两次密码不一致!");
        return;
    }
    /* socket */
    username = "@" + username;  // 注册
    client = new Client{TARGET_IP,TARGET_PORT};
    connect(client,&Client::success_switch,this,&MainWindow::login_test);
    connect(client,&Client::failed_switch,this,&MainWindow::register_return);
    client->connectToServer();
}



//登录
void MainWindow::on_btn_login_clicked()
{
      username = ui->lineEdit_username->text();
      password = ui->lineEdit_password->text();
      QRegularExpression regExp("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}");
      QRegularExpressionValidator validator(regExp);
      int flag = 0;
      QValidator::State result = validator.validate(username,flag);
      if(result != QValidator::Acceptable){
          ui->lineEdit_username->clear();
          ui->lineEdit_password->clear();
          ui->label_signal->setText("账号必须为邮箱!");
          return;
      }
      if(password.length()>16||password.length()<5){
          ui->lineEdit_password->clear();
          ui->label_signal->setText("密码必须6~16位!");
          return;
      }
        /* 登录验证 */
      client = new Client{TARGET_IP,TARGET_PORT};
      connect(client,&Client::success_switch,this,&MainWindow::login_test);
      connect(client,&Client::failed_switch,this,&MainWindow::login_return);
      client->connectToServer();
}


void MainWindow::login_test()
{
      QString ack;
      client->sendMsg(username);
      client->sendMsg(password);
      client->recvMsg(ack);
      if(ack!="fail"){
          qDebug()<<"登录成功";
          client->deleteLater();
          bool ok;
          quint16 value = ack.toUInt(&ok);
          if (ok) {
            Widget *w=new Widget{value,username,password};
            w->show();
            this->close();
          }
          else {
            qDebug()<<"登录成功，但发回来的不是int型string";
            qDebug()<<ack;
          }
      }
      else {
          client->deleteLater();
          login_return();
      }

}

void MainWindow::register_return()
{
      ui->lineEdit_newname->clear();
      ui->lineEdit_newpass->clear();
      ui->lineEdit_surepass->clear();
      ui->label_signal_2->setText("注册失败!");
}

void MainWindow::login_return()
{
      ui->lineEdit_username->clear();
      ui->lineEdit_password->clear();
      ui->label_signal->setText("登录失败!");
}

/* 重写父类移动窗口 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
      if (event->button() == Qt::LeftButton)
      {
          m_Drag = true;
          m_DragPosition = event->globalPosition().toPoint() - this->pos();
          event->accept();
      }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
      if (m_Drag && (event->buttons() & Qt::LeftButton))
      {
          move(event->globalPosition().toPoint() - m_DragPosition);
          event->accept();
      }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
      m_Drag=false;
}

void MainWindow::closeWindow()      // 关闭按钮绑定函数
{
      ui->stackedWidget->setAnimationMode(D2U);
      connect(ui->stackedWidget,&QAnimationStackedWidget::sStackedWidgetAnimationFinished,this,[this](){
          this->close();
      });
      ui->stackedWidget->StartStackedWidgetAnimation(0,1);
}

/* 回车绑定事件 */
void MainWindow::keyPressEvent(QKeyEvent* event)
{
      if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
          if(ui->stackedWidget_2->currentIndex()==0){
            ui->btn_login->click();
          } else {
            ui->btn_sureadd->click();
          }
      } else {

      }
}



// 转到登录窗口函数
void MainWindow::on_btn_login_switch_clicked()
{
  ui->lineEdit_newname->clear();
  ui->lineEdit_newpass->clear();
  ui->lineEdit_surepass->clear();
  ui->label_signal_2->clear();
  ui->label_signal->clear();
  if(ui->stackedWidget_2->currentIndex()==0){
          return;
  }
  ui->stackedWidget_2->setAnimationMode(R2L);
  ui->stackedWidget_2->StartStackedWidgetAnimation(1,0);
}

// 转到注册窗口函数
void MainWindow::on_btn_new_switch_clicked()
{
  ui->lineEdit_username->clear();
  ui->lineEdit_password->clear();
  ui->label_signal->clear();
  ui->label_signal_2->clear();
  if(ui->stackedWidget_2->currentIndex()==1){
          return;
  }
  ui->stackedWidget_2->setAnimationMode(L2R);
  ui->stackedWidget_2->StartStackedWidgetAnimation(0,1);
}




