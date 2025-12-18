#include "menu.h"
#include "ui_menu.h"





Widget::Widget(quint16 connectPort,QString username,QString password,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),password(password),username(username)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);        //设置无窗口框架边界
    setAttribute(Qt::WA_TranslucentBackground);     //设置背景透明
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0, 0);                    // 设置向哪个方向产生阴影效果(dx,dy)，特别地，(0,0)代表向四周发散
    effect->setColor(Qt::black);            // 设置阴影颜色，也可以setColor(QColor(220,220,220))
    effect->setBlurRadius(20);                  // 设定阴影的模糊半径，数值越大越模糊
    ui->mainFrame->setGraphicsEffect(effect);
    connect(ui->showButton,&QPushButton::clicked,this,&Widget::on_showButton_clicked);
    connect(ui->attentionButton,&QPushButton::clicked,this,&Widget::on_attentionButton_clicked);
    connect(ui->warnButton,&QPushButton::clicked,this,&Widget::on_warnButton_clicked);
    connect(ui->meButton,&QPushButton::clicked,this,&Widget::on_meButton_clicked);
    ui->mainStackWidget->setAnimationDuration(300);
    ui->accountMe->setText(username);
    ui->passwordMe->setText(password);

    //右键菜单功能
    ui->tableView_1->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView_2->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView_3->setContextMenuPolicy(Qt::CustomContextMenu);
    tableviewMenue = new QMenu(ui->tableView_1);
    tableviewMenue2 =new QMenu(ui->tableView_2);
    tableviewMenue3 = new QMenu(ui->tableView_3);
    Action1 = new QAction("关注",ui->tableView_1);
    Action2 = new QAction("取消关注",ui->tableView_2);
    Action3 = new QAction("删除",ui->tableView_3);

    tableviewMenue->addAction(Action1);
    tableviewMenue2->addAction(Action2);
    tableviewMenue3->addAction(Action3);

    connect(Action1,SIGNAL(triggered()),this,SLOT(appendAction_triggered()));
    connect(Action2,SIGNAL(triggered()),this,SLOT(prependAction_triggered()));
    connect(Action3,SIGNAL(triggered()),this,SLOT(removeAction_triggered()));
    connect(ui->tableView_1,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(Men_Slot(QPoint)));
    connect(ui->tableView_2,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(Men_Slot2(QPoint)));
    connect(ui->tableView_3,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(Men_Slot3(QPoint)));

    // tableView设置
    model = new QStandardItemModel;
    Model = new QStandardItemModel;
    Model->setColumnCount(5);
    model->setColumnCount(6);
    ui->tableView_1->setModel(model);
    ui->tableView_3->setModel(Model);
    model->setHeaderData(0, Qt::Horizontal, QString("序号"));     //设置表1表头内容
    model->setHeaderData(1, Qt::Horizontal, QString("合约代码"));
    model->setHeaderData(2, Qt::Horizontal, QString("最新价"));
    model->setHeaderData(3, Qt::Horizontal, QString("最高价"));
    model->setHeaderData(4, Qt::Horizontal, QString("最低价"));
    model->setHeaderData(5, Qt::Horizontal, QString("持仓量"));
    Model->setHeaderData(0, Qt::Horizontal, QString("合约代码"));     //设置表3表头内容
    Model->setHeaderData(1, Qt::Horizontal, QString("条件"));
    Model->setHeaderData(2, Qt::Horizontal, QString("阈值"));
    Model->setHeaderData(3, Qt::Horizontal, QString("时间"));
    Model->setHeaderData(4, Qt::Horizontal, QString("触发状态"));

    /* style */
    ui->tableView_1->setWindowFlags(Qt::FramelessWindowHint);
    ui->tableView_1->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:black;color:white;}");  //表头背景色
    ui->tableView_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //列自适应长度，均分
    ui->tableView_1->horizontalHeader()->setStretchLastSection(false);

    ui->tableView_3->setWindowFlags(Qt::FramelessWindowHint);
    ui->tableView_3->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:black;color:white;}");  //表头背景色
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);  //列自适应长度，均分
    ui->tableView_3->horizontalHeader()->setStretchLastSection(false);
    /* endstyle */
    // socket
    submitSocket = new Client{TARGET_IP,connectPort};
    connect(submitSocket,&Client::success_switch,this,&Widget::forwarn_succ);
    connect(submitSocket,&Client::failed_switch,this,&Widget::forwarn_fail);
    submitSocket->connectToServer();
    ui->showButton->clicked();
    server = new QTcpServer;
    server->listen(QHostAddress::AnyIPv4,FLOOD_API);
    connect(server,&QTcpServer::newConnection,this,&Widget::newclientConnect);
    qDebug()<<"构造函数执行完成";
}

Widget::~Widget()
{
    delete ui;
    qDebug()<<"析构menu";
}

void Widget::newclientConnect()
{
    // 建立TCP连接
    QTcpSocket *socket = server->nextPendingConnection();
    listenThread *th = new listenThread(socket);
    th->start();
    connect(th,&listenThread::sendToWidget,this,&Widget::threadSlot);
}

void Widget::threadSlot(QString b)
{
    QStringList list = b.split('\t');
    for (int i = 0; i < 70; i++)  //设置表格具体内容
    {
        QString str= model->data(model->index(i,1)).toString();
        if (str=="" || str==list[0]) {
            model->setItem(i, 0, new QStandardItem(QString::number(i+1)));
            model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
            for(int j=1;j<6;j++){
            model->setItem(i, j, new QStandardItem(list[j-1]));
            model->item(i, j)->setTextAlignment(Qt::AlignCenter);
            }
            break;
        }
    }
}


void Widget::forwarn_succ()
{
    qDebug()<<"预警线程的socket连接成功";
    QString temp;
    for(int k=0;k<2;k++){
        submitSocket->recvMsg(temp);
        if(temp=="")continue;
        qDebug()<<"temp="<<temp;
        QStringList list = temp.split('\t');
        qDebug()<<list.size();
        for(int m=0;m<list.size()-1;){
            for(int j=0;j<5;j++){
                Model->setItem(index_, j, new QStandardItem(list[m++]));
                Model->item(index_, j)->setTextAlignment(Qt::AlignCenter);
            }
            index_++;
        }

    }
}

void Widget::forwarn_fail()
{
    qDebug()<<"预警线程的socket连接失败";
}




/* 下面三个是窗口移动函数 */
void Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_Drag = true;
        m_DragPosition = event->globalPosition().toPoint() - this->pos();
        event->accept();
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_Drag && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPosition().toPoint() - m_DragPosition);
        event->accept();
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    m_Drag=false;
}
/* 翻页函数 */
void Widget::on_showButton_clicked()        // 翻页到展示股票涨跌界面（主界面）
{
    int current = ui->mainStackWidget->currentIndex();
    qDebug()<<current;
    if(current!=0){
        ui->mainStackWidget->setAnimationMode(FromL);
        ui->mainStackWidget->StartStackedWidgetAnimation(current,0);
        ui->mainStackWidget->setCurrentIndex(0);
    }
}

void Widget::on_attentionButton_clicked()   // 翻页到关注的股票界面
{
    int current = ui->mainStackWidget->currentIndex();
    qDebug()<<current;
    if(current!=1){
        if(current>1){
            ui->mainStackWidget->setAnimationMode(L2R);
        }else{
            ui->mainStackWidget->setAnimationMode(R2L);
        }
        ui->mainStackWidget->StartStackedWidgetAnimation(current,1);
        ui->mainStackWidget->setCurrentIndex(1);
    }
}

void Widget::on_warnButton_clicked()        // 翻页到预警管理界面
{
    int current = ui->mainStackWidget->currentIndex();
    qDebug()<<current;
    if(current!=2){
        if(current>2){
            ui->mainStackWidget->setAnimationMode(L2R);
        }else{
            ui->mainStackWidget->setAnimationMode(R2L);
        }
        ui->mainStackWidget->StartStackedWidgetAnimation(current,2);
        ui->mainStackWidget->setCurrentIndex(2);
    }
}

void Widget::on_meButton_clicked()          // 翻页到个人信息界面
{
    int current = ui->mainStackWidget->currentIndex();
    qDebug()<<current;
    if(current!=3){
        ui->mainStackWidget->setAnimationMode(FromR);
        ui->mainStackWidget->StartStackedWidgetAnimation(current,3);
        ui->mainStackWidget->setCurrentIndex(3);
    }
}


void Widget::onTableClicked(const QModelIndex &index)
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->tableView_1->model());
    int row = index.row();   // 获取行号
    qDebug() << row;
    for (int i = 0; i < 6; i++)
    {
        QModelIndex index1 = model->index(row, i);  // 行索引
        QString id = model->data(index1).toString();
        qDebug() << id << " ";
    }
}

void Widget::appendAction_triggered()
{
    //    QStandardItemModel *model1 = new QStandardItemModel;
    //    ui->tableView_2->setModel(model1);
    QModelIndexList selectedIndexes = ui->tableView_1->selectionModel()->selectedRows();
    if (!selectedIndexes.isEmpty()) {
        int selectedRow = selectedIndexes.first().row();
        QStandardItemModel* sourceModel = qobject_cast<QStandardItemModel*>(ui->tableView_1->model());
        QList<QStandardItem*> rowData;
        for (int column = 0; column < sourceModel->columnCount(); ++column) {
            QStandardItem* item = sourceModel->item(selectedRow, column);
            rowData.append(new QStandardItem(item->text()));
        }
        QStandardItemModel* targetModel = qobject_cast<QStandardItemModel*>(ui->tableView_2->model());
        if (!targetModel) {
            targetModel = new QStandardItemModel(this); // 创建一个新的模型
            ui->tableView_2->setModel(targetModel);
        }
        targetModel->appendRow(rowData);
    }
}

void Widget::prependAction_triggered()
{
    QItemSelectionModel *selectionModel = ui->tableView_2->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (!selectedRows.isEmpty()) {
        QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView_2->model());

        if (model) {
            std::sort(selectedRows.begin(), selectedRows.end(), [this](const QModelIndex& a, const QModelIndex& b) {
                return a.row() > b.row();
            });

            for (const QModelIndex &index : selectedRows) {
                model->removeRow(index.row());
            }
        } else {
            qDebug() << "Model is null";
        }
    } else {
        qDebug() << "No selected rows";
    }
}


void Widget::Men_Slot(QPoint p)
{
    QModelIndex index = ui->tableView_1->indexAt(p);    // 获取鼠标点击位置项的索引
    if(index.isValid())                                 // 数据项是否有效，空白处点击无菜单
    {
        QItemSelectionModel* selections = ui->tableView_1->selectionModel();   // 获取当前的选择模型
        QModelIndexList selected = selections->selectedIndexes();               // 返回当前选择的模型索引
        if(selected.count() ==1)    // 右键菜单显示关注
        {
            Action1->setVisible(true);
        }
        else   //右键菜单显示取消关注
        {
            Action2->setVisible(true);
        }
        tableviewMenue->exec(QCursor::pos());//数据项有效才显示菜单
    }
}




void Widget::removeAction_triggered()
{
    // 获取当前选择的行号
    int selectedRow = ui->tableView_3->selectionModel()->currentIndex().row();
    // 获取模型
    QStandardItemModel *model = qobject_cast<QStandardItemModel*>(ui->tableView_3->model());
    QString flag = model->data(model->index(selectedRow,2)).toString();
    QString ID = model->data(model->index(selectedRow,0)).toString();

    if (model) {
        if(flag=="-"){
            QString timeout = model->data(model->index(selectedRow,3)).toString();
            submitSocket->sendMsg("4");
            submitSocket->sendMsg(ID);
            submitSocket->sendMsg(timeout);
        }else{
            QString rela = model->data(model->index(selectedRow,1)).toString();
            submitSocket->sendMsg("3");
            submitSocket->sendMsg(ID);
            submitSocket->sendMsg(rela);
            submitSocket->sendMsg(flag);
        }
        model->removeRow(selectedRow);
        index_--;
    }
}



void Widget::Men_Slot2(QPoint p)
{
    QModelIndex index = ui->tableView_2->indexAt(p);//获取鼠标点击位置项的索引
    if(index.isValid())//数据项是否有效，空白处点击无菜单
    {
        QItemSelectionModel* selections = ui->tableView_2->selectionModel();//获取当前的选择模型
        QModelIndexList selected = selections->selectedIndexes();//返回当前选择的模型索引
        Action2->setVisible(true);
        tableviewMenue2->exec(QCursor::pos());//数据项有效才显示菜单
    }
}


void Widget::Men_Slot3(QPoint p)
{
    QModelIndex index = ui->tableView_3->indexAt(p);//获取鼠标点击位置项的索引
    if(index.isValid())//数据项是否有效，空白处点击无菜单
    {
        QItemSelectionModel* selections = ui->tableView_3->selectionModel();//获取当前的选择模型
        QModelIndexList selected = selections->selectedIndexes();//返回当前选择的模型索引
        Action3->setVisible(true);
        tableviewMenue3->exec(QCursor::pos());//数据项有效才显示菜单
    }
}



void Widget::on_timeSubmit_clicked()
{
    ui->label_sign->clear();
    QString timeout,ID;
    ID = ui->IDEdit2->text();
    timeout = ui->timeEdit->text();
    if(ID!=""&&timeout!=""){
        // 判断
        QRegularExpression regExp("^\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}$");
        QRegularExpressionValidator validator(regExp);
        int flag = 0;
        QValidator::State result = validator.validate(ID,flag);
        if(result != QValidator::Acceptable && false){
            ui->IDEdit2->clear();
            ui->label_sign->setText("日期格式错误!");

        } else {
            // 所有条件满足
            submitSocket->sendMsg("2");
            submitSocket->sendMsg(ID);
            submitSocket->sendMsg(timeout);
            QStringList list={ID,"-","-",timeout,"未知"};
            for(int j=0;j<5;j++){
                Model->setItem(index_, j, new QStandardItem(list[j]));
                Model->item(index_, j)->setTextAlignment(Qt::AlignCenter);
            }
            index_++;
        }
    }else{
            ui->label_sign->setText("内容不能有空!");
    }
    ui->IDEdit2->clear();
    ui->timeEdit->clear();
}


void Widget::on_priceSubmit_clicked()
{
    ui->label_sign->clear();
    QString ID,rela,price;
    ID = ui->IDEdit->text();

    price = ui->priceEdit->text();
    if(ID!="" && price!=""){
        rela = ui->comboBox->currentText()=="大于时告诉我"?">=":"<=";
        submitSocket->sendMsg("1");
        submitSocket->sendMsg(ID);
        submitSocket->sendMsg(rela);
        submitSocket->sendMsg(price);
        QStringList list={ID,rela,price,"-","未知"};
        for(int j=0;j<5;j++){
            Model->setItem(index_, j, new QStandardItem(list[j]));
            Model->item(index_, j)->setTextAlignment(Qt::AlignCenter);
        }
        index_++;
    }else{
        ui->label_sign->setText("内容不能有空!");
    }
    ui->IDEdit->clear();
    ui->priceEdit->clear();
    ui->comboBox->setCurrentIndex(0);
}


void Widget::on_exchangePassword_clicked()
{
    QString password_new = ui->passwordMe->text();
    if(password_new.length()<6){
        ui->passwordMe->clear();
        ui->label_4->setText("密码必须6~16位!");
        return;
    }
    if(password!=password_new){
        qDebug()<<password_new;
        submitSocket->sendMsg("0");
        submitSocket->sendMsg(password_new);
    }
}

