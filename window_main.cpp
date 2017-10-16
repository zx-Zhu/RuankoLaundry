#include "window_main.h"
#include "ui_mainwindow.h"
#include "util_http.h"
#include "dialog.h"
#include <iostream>

#include <QJsonArray>
#include <QStandardItemModel>
#include <qheaderview.h>

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlabel.h>
using namespace std;
int total_money = 0, num = 1, type_num = 1;
QString money, type, service;
QTableView *table, *memberTable, *manageTable;
QStandardItemModel  *model, *memberModel, *manageModel;
QJsonArray orderArray, memberArray, manageArray;
QList<QString> selects;
int orderSize, position;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = ui->orderTable;
    memberTable = ui->table_member;
    manageTable = ui->table_manage;
    this->setOrderTable();
    this->askOrderList();
    this->setMemberTable();
    this->askMemberList();
    this->setManageTable();
    this->askManageList();

}

MainWindow::~MainWindow()//析构
{
    delete ui;
}

void MainWindow::setOrderTable()
{
    model = new QStandardItemModel();
    model->setColumnCount(7);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("洗衣类型"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("服务"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("衣物数量"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("联系方式"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("备注"));
    model->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("金额"));
    table->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
}


void MainWindow::slot_getLaundryFinished(int code, const QString &result)
{
    if(code != 200) return;
    QJsonParseError jsonParseError;
    QJsonDocument document = QJsonDocument::fromJson(result.toUtf8(), &jsonParseError);
    if(jsonParseError.error == QJsonParseError::NoError)
    {
        QJsonObject result = document.object();
        QJsonValue resultValue = result.take("results");
        if(resultValue.isArray())
        {
            qDebug()<<resultValue;
            orderArray = resultValue.toArray();
            orderSize = orderArray.size();
            for(int i=0;i<orderArray.size();i++)
            {
                int a = orderArray.size() - i -1;
                QJsonObject jsonObj = orderArray.at(a).toObject();
                if (jsonObj.contains("username")) {
                    QJsonValue value = jsonObj.take("username");
                    qDebug()<<value.toString();
                    model->setItem(i, 0, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("type")) {
                    QJsonValue value = jsonObj.take("type");
                    qDebug()<<value.toString();
                    model->setItem(i, 1, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("service")) {
                    QJsonValue value = jsonObj.take("service");
                    qDebug()<<value.toString();
                    model->setItem(i, 2, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("num")) {
                    QJsonValue value = jsonObj.take("num");
                    qDebug()<<value.toString();
                    model->setItem(i, 3, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("phone")) {
                    QJsonValue value = jsonObj.take("phone");
                    qDebug()<<value.toString();
                    model->setItem(i, 4, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("extra")) {
                    QJsonValue value = jsonObj.take("extra");
                    qDebug()<<value.toString();
                    model->setItem(i, 5, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("cost")) {
                    QJsonValue value = jsonObj.take("cost");
                    qDebug()<<value.toString();
                    model->setItem(i, 6, new QStandardItem(value.toString()));
                }
            }
        }
    }
    table->setModel(model);
}

//下单
void MainWindow::slot_sendLaundryFinished(int code, const QString& result)
{
    qDebug()<<code<<":"<<result;
    switch (code) {
    case 201:
        emit changeDialogContent("下单成功");
        break;
    case 0:
        emit changeDialogContent("请求超时，请检查网络");
        break;
    default:
        emit changeDialogContent("下单失败");
        break;
    }

}
void MainWindow::on_checkBox_clicked(bool checked)
{
    if(checked) total_money += 3;
    else total_money -= 3;
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_checkBox_2_clicked(bool checked)
{
    if(checked) total_money += 1;
    else total_money -= 1;
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_checkBox_3_clicked(bool checked)
{
    if(checked) total_money += 1;
    else total_money -= 1;
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_spinBox_valueChanged(int arg1)
{
    num = arg1;
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_btn_confirm_clicked()//确认下单 插入数据
{
    emit showDialog("正在下单...");
    if(ui->checkBox->isChecked()) service += "普通洗涤 ";
    if(ui->checkBox_2->isChecked()) service += "杀菌 ";
    if(ui->checkBox_3->isChecked()) service += "烘干 ";
    HttpUtil* pHttpFun = new HttpUtil();
    connect(pHttpFun,SIGNAL(signal_requestFinished(int,const QString&)),this,
            SLOT(slot_sendLaundryFinished(int, const QString&))); //关联请求完成信号
    QString strUrl = "https://fijosi0r.api.lncld.net/1.1/classes/Laundry";
    QJsonObject json;
    json.insert("phone", ui->edit_phone->text());
    json.insert("extra", ui->edit_extra->toPlainText());
    json.insert("username",ui->username->text());
    json.insert("num",QString::number(num, 10));
    json.insert("service",service);
    json.insert("type",type);
    json.insert("cost",money);
    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);
    pHttpFun->sendPostRequest(strUrl,"application/json",dataArray);
    service = "";
}
void MainWindow::on_radioButton_toggled(bool checked)
{
    if(checked) {
        type = "普通衣服";
        type_num = 1;
    }
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_radioButton_2_toggled(bool checked)
{
    if(checked) {
        type = "大件";
        type_num = 2;
    }
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_radioButton_3_toggled(bool checked)
{
    if(checked) {
        type = "床上用品";
        type_num = 2;
    }
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_radioButton_4_toggled(bool checked)
{
    if(checked) {
        type = "鞋子";
        type_num = 2;
    }
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_radioButton_5_toggled(bool checked)
{
    if(checked) {
        type = "干洗";
        type_num = 3;
    }
    money = QString::number(total_money*num*type_num);
    ui->money_total->setText("总金额："+money);
}
void MainWindow::on_btn_order_clicked()
{
    ui->page_order->setCurrentIndex(0);
}
void MainWindow::on_btn_history_clicked()
{
    ui->page_order->setCurrentIndex(1);
    this->askOrderList();
}

void MainWindow::showMain(QString username)
{
    this->show();
    ui->username->setText(username);
}

void MainWindow::askOrderList()
{
    HttpUtil* pHttpFun = new HttpUtil();
    connect(pHttpFun,SIGNAL(signal_requestFinished(int,const QString&)),this,
            SLOT(slot_getLaundryFinished(int, const QString&))); //关联请求完成信号
    QString strUrl = "https://fijosi0r.api.lncld.net/1.1/classes/Laundry";
    pHttpFun->sendGetRequest(strUrl);
}

void MainWindow::askMemberList()
{
    HttpUtil* pHttpFun = new HttpUtil();
    connect(pHttpFun,SIGNAL(signal_requestFinished(int,const QString&)),this,
            SLOT(slot_getMemberFinished(int, const QString&))); //关联请求完成信号
    QString strUrl = "https://fijosi0r.api.lncld.net/1.1/classes/Member";
    pHttpFun->sendGetRequest(strUrl);
}

void MainWindow::askManageList()
{
    HttpUtil* pHttpFun = new HttpUtil();
    connect(pHttpFun,SIGNAL(signal_requestFinished(int,const QString&)),this,
            SLOT(slot_getManageFinished(int, const QString&))); //关联请求完成信号
    QString strUrl = "https://fijosi0r.api.lncld.net/1.1/classes/Shops";
    pHttpFun->sendGetRequest(strUrl);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index == 1)
    {
        this->askMemberList();
    }
    if(index == 2)
    {
        this->askManageList();
    }
    if(index == 0)
    {
        this->askOrderList();
    }
}

void MainWindow::slot_getMemberFinished(int code, const QString &result)
{
    if(code != 200) return;
    QJsonParseError jsonParseError;
    QJsonDocument document = QJsonDocument::fromJson(result.toUtf8(), &jsonParseError);
    if(jsonParseError.error == QJsonParseError::NoError)
    {
        QJsonObject result = document.object();
        QJsonValue resultValue = result.take("results");
        if(resultValue.isArray())
        {
            qDebug()<<resultValue;
            memberArray = resultValue.toArray();
            for(int i=0;i<memberArray.size();i++)
            {
                int a = memberArray.size() - i -1;
                QJsonObject jsonObj = memberArray.at(a).toObject();
                if (jsonObj.contains("name")) {
                    QJsonValue value = jsonObj.take("name");
                    qDebug()<<value.toString();
                    memberModel->setItem(i, 0, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("phone")) {
                    QJsonValue value = jsonObj.take("phone");
                    qDebug()<<value.toString();
                    memberModel->setItem(i, 1, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("address")) {
                    QJsonValue value = jsonObj.take("address");
                    qDebug()<<value.toString();
                    memberModel->setItem(i, 2, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("createdAt")) {
                    QJsonValue value = jsonObj.take("createdAt");
                    qDebug()<<value.toString();
                    QString time = value.toString().remove(19,5).replace("T","  ");
                    memberModel->setItem(i, 3, new QStandardItem(time));
                }
            }
        }
    }
    memberTable->setModel(memberModel);
}

void MainWindow::slot_getManageFinished(int code, const QString &result)
{
    if(code != 200) return;
    QJsonParseError jsonParseError;
    QJsonDocument document = QJsonDocument::fromJson(result.toUtf8(), &jsonParseError);
    if(jsonParseError.error == QJsonParseError::NoError)
    {
        QJsonObject result = document.object();
        QJsonValue resultValue = result.take("results");
        if(resultValue.isArray())
        {

            manageArray = resultValue.toArray();
            for(int i=0;i<manageArray.size();i++)
            {
                int a = manageArray.size() - i -1;
                QJsonObject jsonObj = manageArray.at(a).toObject();
                if (jsonObj.contains("city")) {
                    QJsonValue value = jsonObj.take("city");
                    qDebug()<<value.toString();
                    manageModel->setItem(i, 0, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("address")) {
                    QJsonValue value = jsonObj.take("address");
                    qDebug()<<value.toString();
                    manageModel->setItem(i, 1, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("owner")) {
                    QJsonValue value = jsonObj.take("owner");
                    qDebug()<<value.toString();
                    manageModel->setItem(i, 2, new QStandardItem(value.toString()));
                }
                if(jsonObj.contains("phone"))
                {
                    QJsonValue value = jsonObj.take("phone");
                    manageModel->setItem(i, 3, new QStandardItem(value.toString()));
                }
                if(jsonObj.contains("type"))
                {
                    QJsonValue value = jsonObj.take("type");
                    manageModel->setItem(i, 4, new QStandardItem(value.toString()));
                }
                if (jsonObj.contains("createdAt")) {
                    QJsonValue value = jsonObj.take("createdAt");
                    qDebug()<<value.toString();
                    QString time = value.toString().remove(19,5).replace("T","  ");
                    manageModel->setItem(i, 5, new QStandardItem(time));
                }
            }
        }
    }
    manageTable->setModel(manageModel);
}

void MainWindow::setMemberTable()
{
    memberModel = new QStandardItemModel();
    memberModel->setColumnCount(4);
    memberModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户"));
    memberModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("联系方式"));
    memberModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("地址"));
    memberModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("注册时间"));
    memberTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
}

void MainWindow::setManageTable()
{
    manageModel = new QStandardItemModel();
    manageModel->setColumnCount(6);
    manageModel->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("城市"));
    manageModel->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("地址"));
    manageModel->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("店主"));
    manageModel->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("联系方式"));
    manageModel->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("店铺类型"));
    manageModel->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("开业时间"));
    manageTable->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
}

void MainWindow::on_exit_clicked()
{
    QCoreApplication::exit();
}

void MainWindow::on_btn_search_order_clicked()

{
    selects.clear();

    QJsonArray tem = orderArray;
    if(ui->search_order->text().isEmpty())
    {
        ui->num_search->setText("输入为空");
        return;

    }
    if(orderArray.size() == 0) return;
    QString str = ui->search_order->text();
    qDebug()<<"ooooooo"<<orderArray;

    for(int i=0;i<orderSize;i++)
    {

        int a = orderSize - i -1;
        QJsonObject jsonObj = orderArray.takeAt(a).toObject();
        QString item = QString(QJsonDocument(jsonObj).toJson());

        if(item.contains(str,Qt::CaseSensitive))
        {
            qDebug()<<orderSize<<"zzzzzz:"<<item<<i;
            selects.append(QString::number(i));
        }
    }
    position = 0;
    orderArray = tem;
    if(selects.size()!=0)
    {
        table->selectRow(selects.at(0).toInt());
        ui->num_search->setText(QString::number(position+1)+" / "+QString::number(selects.size()));
    }
    else {
        ui->num_search->setText("没有找到结果");
    }
}

void MainWindow::on_btn_right_clicked()
{
    if(selects.size() == 0) return;
    if(selects.size()==position+1) position = -1;
    table->selectRow(selects.at(position+1).toInt());
    position+=1;
    ui->num_search->setText(QString::number(position+1)+" / "+QString::number(selects.size()));
}

void MainWindow::on_btn_left_clicked()
{
    if(selects.size() == 0) return;
    if(position == 0) position = selects.size();
    table->selectRow(selects.at(position-1).toInt());
    position-=1;
    ui->num_search->setText(QString::number(position+1)+" / "+QString::number(selects.size()));
}
