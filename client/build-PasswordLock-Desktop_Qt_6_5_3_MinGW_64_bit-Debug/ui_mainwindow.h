/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <qanimationstackedwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QAnimationStackedWidget *stackedWidget;
    QWidget *mainwindow;
    QFrame *frame;
    QLabel *label;
    QPushButton *closeButton;
    QPushButton *btn_login_switch;
    QLabel *helloLabel;
    QPushButton *pushButton_2;
    QAnimationStackedWidget *stackedWidget_2;
    QWidget *loginPage;
    QLineEdit *lineEdit_username;
    QLineEdit *lineEdit_password;
    QPushButton *btn_login;
    QLabel *label_signal;
    QWidget *registerPage;
    QLineEdit *lineEdit_newpass;
    QLineEdit *lineEdit_newname;
    QLineEdit *lineEdit_surepass;
    QPushButton *btn_sureadd;
    QLabel *label_signal_2;
    QPushButton *btn_new_switch;
    QFrame *line;
    QLabel *label_2;
    QWidget *block;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(803, 457);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        stackedWidget = new QAnimationStackedWidget(centralWidget);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 10, 781, 381));
        stackedWidget->setStyleSheet(QString::fromUtf8("#stackWidget{\n"
"background-color:rgba(255, 255, 255,0);\n"
"}"));
        stackedWidget->setInputMethodHints(Qt::ImhEmailCharactersOnly);
        mainwindow = new QWidget();
        mainwindow->setObjectName("mainwindow");
        frame = new QFrame(mainwindow);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(100, 60, 531, 281));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"border-radius:30px;\n"
"background:white;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 0, 271, 281));
        label->setStyleSheet(QString::fromUtf8("#label{\n"
"border-image:url(:/image/bg2.jpg);\n"
"border-top-left-radius:30px;\n"
"border-bottom-left-radius:30px;\n"
"padding:2px 4px;\n"
"}"));
        closeButton = new QPushButton(frame);
        closeButton->setObjectName("closeButton");
        closeButton->setGeometry(QRect(490, 0, 41, 38));
        QFont font;
        font.setPointSize(23);
        closeButton->setFont(font);
        closeButton->setStyleSheet(QString::fromUtf8("#closeButton{\n"
"border-top-right-radius:30px;\n"
"}\n"
"#closeButton:hover{\n"
"background:red;\n"
"}"));
        closeButton->setFlat(true);
        btn_login_switch = new QPushButton(frame);
        btn_login_switch->setObjectName("btn_login_switch");
        btn_login_switch->setGeometry(QRect(344, 40, 38, 38));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Microsoft YaHei UI")});
        font1.setPointSize(25);
        font1.setBold(false);
        font1.setItalic(false);
        btn_login_switch->setFont(font1);
        btn_login_switch->setStyleSheet(QString::fromUtf8("#btn_login_switch{\n"
"border:none;\n"
"border-image:url(:/image/login.png);\n"
"border-radius:5px;\n"
"}\n"
"#btn_login_switch:hover{\n"
"background:rgb(121, 121, 121);\n"
"}"));
        helloLabel = new QLabel(frame);
        helloLabel->setObjectName("helloLabel");
        helloLabel->setGeometry(QRect(30, 90, 211, 61));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Brush Script MT")});
        font2.setPointSize(30);
        font2.setBold(false);
        font2.setItalic(true);
        helloLabel->setFont(font2);
        helloLabel->setStyleSheet(QString::fromUtf8("#helloLabel{\n"
"color:white;\n"
"}"));
        helloLabel->setAlignment(Qt::AlignCenter);
        pushButton_2 = new QPushButton(frame);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(452, 0, 38, 38));
        QFont font3;
        font3.setPointSize(29);
        pushButton_2->setFont(font3);
        pushButton_2->setStyleSheet(QString::fromUtf8("#pushButton_2{\n"
"border:none;\n"
"}\n"
"#pushButton_2:hover{\n"
"background:rgb(0, 170, 255);\n"
"}"));
        stackedWidget_2 = new QAnimationStackedWidget(frame);
        stackedWidget_2->setObjectName("stackedWidget_2");
        stackedWidget_2->setGeometry(QRect(284, 79, 231, 191));
        loginPage = new QWidget();
        loginPage->setObjectName("loginPage");
        lineEdit_username = new QLineEdit(loginPage);
        lineEdit_username->setObjectName("lineEdit_username");
        lineEdit_username->setGeometry(QRect(10, 23, 211, 41));
        lineEdit_username->setStyleSheet(QString::fromUtf8("#lineEdit_username{\n"
"border:3px solid black;\n"
"font-size:15px;\n"
"border-radius:10px;\n"
"}\n"
"#lineEdit_username:hover{\n"
"border:4px solid black;\n"
"padding:5px;\n"
"}"));
        lineEdit_username->setInputMethodHints(Qt::ImhEmailCharactersOnly);
        lineEdit_username->setMaxLength(32767);
        lineEdit_username->setReadOnly(false);
        lineEdit_password = new QLineEdit(loginPage);
        lineEdit_password->setObjectName("lineEdit_password");
        lineEdit_password->setGeometry(QRect(10, 78, 211, 41));
        lineEdit_password->setStyleSheet(QString::fromUtf8("#lineEdit_password{\n"
"border:3px solid black;\n"
"font-size:15px;\n"
"border-radius:10px;\n"
"}\n"
"\n"
"#lineEdit_password:hover{\n"
"border:4px solid black;\n"
"padding:5px;\n"
"}"));
        lineEdit_password->setMaxLength(16);
        lineEdit_password->setEchoMode(QLineEdit::Password);
        btn_login = new QPushButton(loginPage);
        btn_login->setObjectName("btn_login");
        btn_login->setGeometry(QRect(20, 139, 191, 41));
        btn_login->setStyleSheet(QString::fromUtf8("#btn_login{\n"
"background:black;\n"
"color:white;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:none;\n"
"}\n"
"#btn_login:hover{\n"
"background:white;\n"
"color:black;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:4px solid black;\n"
"}"));
        label_signal = new QLabel(loginPage);
        label_signal->setObjectName("label_signal");
        label_signal->setGeometry(QRect(20, 120, 191, 20));
        QFont font4;
        font4.setFamilies({QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221")});
        font4.setPointSize(10);
        label_signal->setFont(font4);
        label_signal->setStyleSheet(QString::fromUtf8("#label_signal{\n"
"color:red;\n"
"}"));
        label_signal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        stackedWidget_2->addWidget(loginPage);
        registerPage = new QWidget();
        registerPage->setObjectName("registerPage");
        lineEdit_newpass = new QLineEdit(registerPage);
        lineEdit_newpass->setObjectName("lineEdit_newpass");
        lineEdit_newpass->setGeometry(QRect(10, 51, 211, 36));
        lineEdit_newpass->setStyleSheet(QString::fromUtf8("#lineEdit_newpass{\n"
"border:3px solid black;\n"
"font-size:15px;\n"
"border-radius:10px;\n"
"}\n"
"#lineEdit_newpass:hover{\n"
"border:4px solid black;\n"
"padding:5px;\n"
"}"));
        lineEdit_newpass->setEchoMode(QLineEdit::Password);
        lineEdit_newname = new QLineEdit(registerPage);
        lineEdit_newname->setObjectName("lineEdit_newname");
        lineEdit_newname->setGeometry(QRect(10, 9, 211, 36));
        lineEdit_newname->setStyleSheet(QString::fromUtf8("#lineEdit_newname{\n"
"border:3px solid black;\n"
"font-size:15px;\n"
"border-radius:10px;\n"
"}\n"
"\n"
"#lineEdit_newname:hover{\n"
"border:4px solid black;\n"
"padding:5px;\n"
"}"));
        lineEdit_surepass = new QLineEdit(registerPage);
        lineEdit_surepass->setObjectName("lineEdit_surepass");
        lineEdit_surepass->setGeometry(QRect(10, 93, 211, 36));
        lineEdit_surepass->setStyleSheet(QString::fromUtf8("#lineEdit_surepass{\n"
"border:3px solid black;\n"
"font-size:15px;\n"
"border-radius:10px;\n"
"}\n"
"#lineEdit_surepass:hover{\n"
"border:4px solid black;\n"
"padding:5px;\n"
"}"));
        lineEdit_surepass->setEchoMode(QLineEdit::Password);
        btn_sureadd = new QPushButton(registerPage);
        btn_sureadd->setObjectName("btn_sureadd");
        btn_sureadd->setGeometry(QRect(20, 148, 191, 41));
        btn_sureadd->setStyleSheet(QString::fromUtf8("#btn_sureadd{\n"
"background:black;\n"
"color:white;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:none;\n"
"}\n"
"#btn_sureadd:hover{\n"
"background:white;\n"
"color:black;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:4px solid black;\n"
"}"));
        label_signal_2 = new QLabel(registerPage);
        label_signal_2->setObjectName("label_signal_2");
        label_signal_2->setGeometry(QRect(110, 130, 101, 20));
        label_signal_2->setStyleSheet(QString::fromUtf8("#label_signal_2{\n"
"color:red;\n"
"}"));
        label_signal_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        stackedWidget_2->addWidget(registerPage);
        btn_new_switch = new QPushButton(frame);
        btn_new_switch->setObjectName("btn_new_switch");
        btn_new_switch->setGeometry(QRect(416, 40, 36, 36));
        btn_new_switch->setStyleSheet(QString::fromUtf8("#btn_new_switch{\n"
"border:none;\n"
"border-image:url(:/image/register.png);\n"
"border-radius:5px;\n"
"}\n"
"#btn_new_switch:hover{\n"
"background:rgb(121, 121, 121);\n"
"}"));
        line = new QFrame(frame);
        line->setObjectName("line");
        line->setGeometry(QRect(389, 40, 16, 31));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 20, 281, 61));
        label_2->setStyleSheet(QString::fromUtf8("#label_2{\n"
"border-image:url(:/image/logo.png);\n"
"}"));
        label->raise();
        btn_login_switch->raise();
        helloLabel->raise();
        closeButton->raise();
        pushButton_2->raise();
        stackedWidget_2->raise();
        btn_new_switch->raise();
        line->raise();
        label_2->raise();
        stackedWidget->addWidget(mainwindow);
        block = new QWidget();
        block->setObjectName("block");
        stackedWidget->addWidget(block);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);
        QObject::connect(pushButton_2, &QPushButton::clicked, MainWindow, qOverload<>(&QMainWindow::showMinimized));

        stackedWidget->setCurrentIndex(0);
        stackedWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QString());
        closeButton->setText(QCoreApplication::translate("MainWindow", "\303\227", nullptr));
        btn_login_switch->setText(QString());
        helloLabel->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">Good Evening</p></body></html>", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        lineEdit_username->setText(QString());
        lineEdit_username->setPlaceholderText(QCoreApplication::translate("MainWindow", "Input Account", nullptr));
        lineEdit_password->setPlaceholderText(QCoreApplication::translate("MainWindow", "Input Password", nullptr));
        btn_login->setText(QCoreApplication::translate("MainWindow", "Sign in", nullptr));
#if QT_CONFIG(shortcut)
        btn_login->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S, Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        label_signal->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"right\"><br/></p></body></html>", nullptr));
        lineEdit_newpass->setPlaceholderText(QCoreApplication::translate("MainWindow", "Your Password", nullptr));
        lineEdit_newname->setPlaceholderText(QCoreApplication::translate("MainWindow", "New Account", nullptr));
        lineEdit_surepass->setText(QString());
        lineEdit_surepass->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ensure Password", nullptr));
        btn_sureadd->setText(QCoreApplication::translate("MainWindow", "Register", nullptr));
        label_signal_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"right\"><br/></p></body></html>", nullptr));
        btn_new_switch->setText(QString());
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
