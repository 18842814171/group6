/********************************************************************************
** Form generated from reading UI file 'menu.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_H
#define UI_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>
#include <qanimationstackedwidget.h>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QFrame *mainFrame;
    QAnimationStackedWidget *mainStackWidget;
    QWidget *showPage;
    QTableView *tableView_1;
    QWidget *attentionPage;
    QTableView *tableView_2;
    QWidget *warnPage;
    QFrame *frame;
    QComboBox *comboBox;
    QPushButton *priceSubmit;
    QLineEdit *priceEdit;
    QPushButton *timeSubmit;
    QFrame *line;
    QLineEdit *IDEdit2;
    QLineEdit *IDEdit;
    QLineEdit *timeEdit;
    QLabel *label_sign;
    QTableView *tableView_3;
    QWidget *mePage;
    QLineEdit *accountMe;
    QLineEdit *passwordMe;
    QPushButton *exchangePassword;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *showButton;
    QPushButton *attentionButton;
    QPushButton *warnButton;
    QPushButton *meButton;
    QPushButton *closeButton;
    QPushButton *minButton;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(1060, 662);
        mainFrame = new QFrame(Widget);
        mainFrame->setObjectName("mainFrame");
        mainFrame->setGeometry(QRect(60, 0, 962, 541));
        mainFrame->setStyleSheet(QString::fromUtf8("#mainFrame{\n"
"background:black;\n"
"border-radius:5px;\n"
"border-image:url(:/image/bg3.jpg);\n"
"}"));
        mainFrame->setFrameShape(QFrame::StyledPanel);
        mainFrame->setFrameShadow(QFrame::Raised);
        mainStackWidget = new QAnimationStackedWidget(mainFrame);
        mainStackWidget->setObjectName("mainStackWidget");
        mainStackWidget->setGeometry(QRect(20, 90, 921, 431));
        mainStackWidget->setStyleSheet(QString::fromUtf8("#mainStackWidget{\n"
"background-color:rgba(0, 0, 0,50);\n"
"}"));
        showPage = new QWidget();
        showPage->setObjectName("showPage");
        tableView_1 = new QTableView(showPage);
        tableView_1->setObjectName("tableView_1");
        tableView_1->setGeometry(QRect(0, 0, 921, 431));
        tableView_1->setStyleSheet(QString::fromUtf8("QTableView::item{\n"
"font:normal 400 15px;\n"
"}\n"
"QHeaderView::section\n"
"{\n"
"background:rgb(95, 95, 95);\n"
"color:white;\n"
"font:normal 400 15px;\n"
"}\n"
""));
        tableView_1->setFrameShape(QFrame::StyledPanel);
        tableView_1->setFrameShadow(QFrame::Plain);
        tableView_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView_1->setAlternatingRowColors(true);
        tableView_1->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView_1->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_1->setTextElideMode(Qt::ElideMiddle);
        tableView_1->setSortingEnabled(false);
        tableView_1->verticalHeader()->setVisible(false);
        mainStackWidget->addWidget(showPage);
        attentionPage = new QWidget();
        attentionPage->setObjectName("attentionPage");
        tableView_2 = new QTableView(attentionPage);
        tableView_2->setObjectName("tableView_2");
        tableView_2->setGeometry(QRect(0, 0, 931, 431));
        tableView_2->setStyleSheet(QString::fromUtf8(""));
        tableView_2->setFrameShape(QFrame::WinPanel);
        tableView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        mainStackWidget->addWidget(attentionPage);
        warnPage = new QWidget();
        warnPage->setObjectName("warnPage");
        frame = new QFrame(warnPage);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(30, 340, 861, 80));
        frame->setStyleSheet(QString::fromUtf8("#frame{\n"
"background:rgba(175, 175, 175,150);\n"
"border:none;\n"
"border-radius:15px;\n"
"}"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        comboBox = new QComboBox(frame);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(670, 10, 131, 25));
        comboBox->setStyleSheet(QString::fromUtf8("#comboBox{\n"
"border:2px solid white;\n"
"font-size:15px;\n"
"border-radius:4px;\n"
"}\n"
"#comboBox:hover{\n"
"background:rgba(85, 255, 127,150);\n"
"}\n"
"#comboBox::drop-down { width: 0px;\n"
"border: 0px; }\n"
"#comboBox::down-arrow\n"
" { \n"
"image: none; }"));
        comboBox->setDuplicatesEnabled(false);
        priceSubmit = new QPushButton(frame);
        priceSubmit->setObjectName("priceSubmit");
        priceSubmit->setGeometry(QRect(400, 20, 101, 41));
        priceSubmit->setStyleSheet(QString::fromUtf8("#priceSubmit{\n"
"background:red;\n"
"color:white;\n"
"font:normal 400 15px;;\n"
"border-radius:5px;\n"
"border:2px solid white;\n"
"}\n"
"#priceSubmit:hover{\n"
"background:white;\n"
"color:red;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid red;\n"
"}"));
        priceEdit = new QLineEdit(frame);
        priceEdit->setObjectName("priceEdit");
        priceEdit->setGeometry(QRect(531, 50, 108, 25));
        priceEdit->setStyleSheet(QString::fromUtf8("#priceEdit{\n"
"border:2px solid white;\n"
"color:white;\n"
"background:rgba(255, 255, 255,0);\n"
"font-size:15px;\n"
"border-radius:4px;\n"
"}\n"
"#priceEdit:hover{\n"
"border:3px solid white;\n"
"padding:2px;\n"
"}"));
        timeSubmit = new QPushButton(frame);
        timeSubmit->setObjectName("timeSubmit");
        timeSubmit->setGeometry(QRect(20, 20, 101, 41));
        timeSubmit->setStyleSheet(QString::fromUtf8("#timeSubmit{\n"
"background:red;\n"
"color:white;\n"
"font:normal 400 15px;;\n"
"border-radius:5px;\n"
"border:2px solid white;\n"
"}\n"
"#timeSubmit:hover{\n"
"background:white;\n"
"color:red;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid red;\n"
"}"));
        line = new QFrame(frame);
        line->setObjectName("line");
        line->setGeometry(QRect(370, 2, 9, 79));
        line->setFrameShadow(QFrame::Sunken);
        line->setLineWidth(0);
        line->setMidLineWidth(3);
        line->setFrameShape(QFrame::VLine);
        IDEdit2 = new QLineEdit(frame);
        IDEdit2->setObjectName("IDEdit2");
        IDEdit2->setGeometry(QRect(140, 10, 108, 25));
        IDEdit2->setAutoFillBackground(false);
        IDEdit2->setStyleSheet(QString::fromUtf8("#IDEdit2{\n"
"border:2px solid white;\n"
"color:white;\n"
"background:rgba(255, 255, 255,0);\n"
"font-size:15px;\n"
"border-radius:4px;\n"
"}\n"
"#IDEdit2:hover{\n"
"border:3px solid white;\n"
"padding:2px;\n"
"}"));
        IDEdit2->setMaxLength(8);
        IDEdit = new QLineEdit(frame);
        IDEdit->setObjectName("IDEdit");
        IDEdit->setGeometry(QRect(531, 10, 108, 25));
        IDEdit->setStyleSheet(QString::fromUtf8("#IDEdit{\n"
"border:2px solid white;\n"
"color:white;\n"
"background:rgba(255, 255, 255,0);\n"
"font-size:15px;\n"
"border-radius:4px;\n"
"}\n"
"#IDEdit:hover{\n"
"border:3px solid white;\n"
"padding:2px;\n"
"}"));
        IDEdit->setMaxLength(8);
        timeEdit = new QLineEdit(frame);
        timeEdit->setObjectName("timeEdit");
        timeEdit->setGeometry(QRect(140, 50, 211, 25));
        timeEdit->setStyleSheet(QString::fromUtf8("#timeEdit{\n"
"border:2px solid white;\n"
"color:white;\n"
"background:rgba(255, 255, 255,0);\n"
"font-size:15px;\n"
"border-radius:4px;\n"
"}\n"
"#timeEdit:hover{\n"
"border:3px solid white;\n"
"padding:2px;\n"
"}"));
        label_sign = new QLabel(frame);
        label_sign->setObjectName("label_sign");
        label_sign->setGeometry(QRect(670, 50, 141, 21));
        label_sign->setStyleSheet(QString::fromUtf8("#label_sign{\n"
"color:red;\n"
"}"));
        tableView_3 = new QTableView(warnPage);
        tableView_3->setObjectName("tableView_3");
        tableView_3->setGeometry(QRect(0, 0, 921, 321));
        tableView_3->setStyleSheet(QString::fromUtf8("QTableView::item{\n"
"font:normal 400 15px;\n"
"}\n"
"QHeaderView::section\n"
"{\n"
"background:rgb(95, 95, 95);\n"
"color:white;\n"
"font:normal 400 15px;\n"
"}"));
        tableView_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView_3->setAlternatingRowColors(true);
        tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_3->setTextElideMode(Qt::ElideMiddle);
        tableView_3->verticalHeader()->setVisible(false);
        mainStackWidget->addWidget(warnPage);
        mePage = new QWidget();
        mePage->setObjectName("mePage");
        accountMe = new QLineEdit(mePage);
        accountMe->setObjectName("accountMe");
        accountMe->setGeometry(QRect(340, 120, 241, 41));
        QFont font;
        accountMe->setFont(font);
        accountMe->setStyleSheet(QString::fromUtf8("#accountMe{\n"
"border:4px solid white;\n"
"color:white;\n"
"background:rgba(255, 255, 255,0);\n"
"font-size:18px;\n"
"border-radius:4px;\n"
"}"));
        accountMe->setReadOnly(true);
        passwordMe = new QLineEdit(mePage);
        passwordMe->setObjectName("passwordMe");
        passwordMe->setGeometry(QRect(340, 200, 241, 41));
        passwordMe->setStyleSheet(QString::fromUtf8("#passwordMe{\n"
"border:4px solid white;\n"
"color:white;\n"
"background:rgba(255, 255, 255,0);\n"
"font-size:18px;\n"
"border-radius:4px;\n"
"}\n"
"#passwordMe:hover{\n"
"border:5px solid white;\n"
"padding:2px;\n"
"}"));
        passwordMe->setMaxLength(16);
        exchangePassword = new QPushButton(mePage);
        exchangePassword->setObjectName("exchangePassword");
        exchangePassword->setGeometry(QRect(330, 280, 261, 61));
        exchangePassword->setStyleSheet(QString::fromUtf8("#exchangePassword{\n"
"background:rgb(0, 62, 185);\n"
"color:white;\n"
"font:normal 400 35px;\n"
"border-radius:15px;\n"
"border:5px solid white;\n"
"}\n"
"#exchangePassword:hover{\n"
"background:white;\n"
"color:black;\n"
"font:normal 400 35px;\n"
"border-radius:15px;\n"
"border:5px solid rgb(0, 62, 185);\n"
"}"));
        label = new QLabel(mePage);
        label->setObjectName("label");
        label->setGeometry(QRect(248, 110, 91, 61));
        QFont font1;
        font1.setPointSize(16);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("#label{\n"
"color:white;\n"
"\n"
"}"));
        label_2 = new QLabel(mePage);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(264, 189, 71, 61));
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8("#label_2{\n"
"color:white;\n"
"\n"
"}"));
        label_3 = new QLabel(mePage);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(310, 40, 251, 51));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("CityBlueprint")});
        font2.setPointSize(36);
        font2.setBold(false);
        font2.setItalic(false);
        label_3->setFont(font2);
        label_3->setStyleSheet(QString::fromUtf8("#label_3{\n"
"color:white;\n"
"font:normal solid;\n"
"}"));
        label_4 = new QLabel(mePage);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(350, 250, 231, 21));
        label_4->setStyleSheet(QString::fromUtf8("#label_4{\n"
"color:red;\n"
"}"));
        mainStackWidget->addWidget(mePage);
        showButton = new QPushButton(mainFrame);
        showButton->setObjectName("showButton");
        showButton->setGeometry(QRect(30, 20, 151, 51));
        showButton->setStyleSheet(QString::fromUtf8("#showButton{\n"
"background:black;\n"
"color:white;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid white;\n"
"}\n"
"#showButton:hover{\n"
"background:white;\n"
"color:black;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid black;\n"
"}"));
        attentionButton = new QPushButton(mainFrame);
        attentionButton->setObjectName("attentionButton");
        attentionButton->setGeometry(QRect(200, 20, 151, 51));
        attentionButton->setStyleSheet(QString::fromUtf8("#attentionButton{\n"
"background:black;\n"
"color:white;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid white;\n"
"}\n"
"#attentionButton:hover{\n"
"background:white;\n"
"color:black;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid black;\n"
"}"));
        warnButton = new QPushButton(mainFrame);
        warnButton->setObjectName("warnButton");
        warnButton->setGeometry(QRect(370, 20, 151, 51));
        warnButton->setStyleSheet(QString::fromUtf8("#warnButton{\n"
"background:black;\n"
"color:white;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid white;\n"
"}\n"
"#warnButton:hover{\n"
"background:white;\n"
"color:black;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid black;\n"
"}"));
        meButton = new QPushButton(mainFrame);
        meButton->setObjectName("meButton");
        meButton->setGeometry(QRect(530, 20, 151, 51));
        meButton->setStyleSheet(QString::fromUtf8("#meButton{\n"
"background:black;\n"
"color:white;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid white;\n"
"}\n"
"#meButton:hover{\n"
"background:white;\n"
"color:black;\n"
"font:normal 400 20px;\n"
"border-radius:10px;\n"
"border:2px solid black;\n"
"}"));
        closeButton = new QPushButton(mainFrame);
        closeButton->setObjectName("closeButton");
        closeButton->setGeometry(QRect(912, 0, 51, 41));
        closeButton->setStyleSheet(QString::fromUtf8("#closeButton{\n"
"border-top-right-radius:5px;\n"
"background-color:rgb(0,0,0);\n"
"color:white;\n"
"font:normal 600 40px;\n"
"}\n"
"#closeButton:hover{\n"
"background-color:rgba(255,0,0,200);\n"
"}"));
        minButton = new QPushButton(mainFrame);
        minButton->setObjectName("minButton");
        minButton->setGeometry(QRect(870, 0, 41, 41));
        minButton->setStyleSheet(QString::fromUtf8("#minButton{\n"
"background-color:rgba(0,0,0,0);\n"
"color:white;\n"
"font:normal 600 40px;\n"
"}\n"
"#minButton:hover{\n"
"background-color:rgba(0, 85, 255,200);\n"
"}"));

        retranslateUi(Widget);
        QObject::connect(closeButton, &QPushButton::clicked, Widget, qOverload<>(&QWidget::close));
        QObject::connect(minButton, &QPushButton::clicked, Widget, qOverload<>(&QWidget::showMinimized));

        mainStackWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("Widget", "\345\244\247\344\272\216\346\227\266\345\221\212\350\257\211\346\210\221", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("Widget", "\345\260\217\344\272\216\346\227\266\345\221\212\350\257\211\346\210\221", nullptr));

        priceSubmit->setText(QCoreApplication::translate("Widget", "\346\267\273\345\212\240\351\242\204\350\255\246", nullptr));
        priceEdit->setPlaceholderText(QCoreApplication::translate("Widget", "\351\242\204\350\255\246\344\273\267\346\240\274", nullptr));
        timeSubmit->setText(QCoreApplication::translate("Widget", "\346\267\273\345\212\240\351\242\204\350\255\246", nullptr));
        IDEdit2->setPlaceholderText(QCoreApplication::translate("Widget", "\344\272\244\346\230\223\344\273\243\347\240\201", nullptr));
        IDEdit->setPlaceholderText(QCoreApplication::translate("Widget", "\344\272\244\346\230\223\344\273\243\347\240\201", nullptr));
        timeEdit->setPlaceholderText(QCoreApplication::translate("Widget", "\346\227\266\351\227\264:xxxx-xx-xx xx:xx:xx", nullptr));
        label_sign->setText(QString());
        accountMe->setText(QCoreApplication::translate("Widget", "123", nullptr));
        passwordMe->setText(QCoreApplication::translate("Widget", "456", nullptr));
        exchangePassword->setText(QCoreApplication::translate("Widget", "\344\277\256\346\224\271", nullptr));
        label->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\">\351\202\256\347\256\261</p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\">\345\257\206\347\240\201</p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\">WENHUA</p></body></html>", nullptr));
        label_4->setText(QString());
        showButton->setText(QCoreApplication::translate("Widget", "\350\202\241\347\245\250\350\241\250\345\215\225", nullptr));
        attentionButton->setText(QCoreApplication::translate("Widget", "\346\210\221\347\232\204\345\205\263\346\263\250", nullptr));
        warnButton->setText(QCoreApplication::translate("Widget", "\351\242\204\350\255\246\347\256\241\347\220\206", nullptr));
        meButton->setText(QCoreApplication::translate("Widget", "\344\270\252\344\272\272\344\277\241\346\201\257", nullptr));
        closeButton->setText(QCoreApplication::translate("Widget", "\303\227", nullptr));
        minButton->setText(QCoreApplication::translate("Widget", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_H
