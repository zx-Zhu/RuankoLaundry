#include "window_main.h"
#include "ui_mainwindow.h"
#include "util_http.h"
#include "dialog.h"
#include <iostream>

#include <QJsonArray>
#include <QStandardItemModel>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlabel.h>
using namespace std;
int total_money = 0, num = 1, type_num = 1;
QString money, type, service;
QTableView *table;
QStandardItemModel  *model;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = ui->orderTable;
    this->setOrderTable();
    HttpUtil* pHttpFun = new HttpUtil();
    connect(pHttpFun,SIGNAL(signal_requestFinished(int,const QString&)),this,
            SLOT(slot_getLaundryFinished(int, const QString&))); //关联请求完成信号
    QString strUrl = "https://fijosi0r.api.lncld.net/1.1/classes/Laundry";
    pHttpFun->sendGetRequest(strUrl);
}

MainWindow::~MainWindow()//析构
{
    delete ui;
}

void MainWindow::setOrderTable()
{
    model = new QStandardItemModel();
    model->setColumnCount(6);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("用户"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("洗衣类型"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("服务"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("衣物数量"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("联系方式"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("备注"));

    table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
}


void MainWindow::slot_getLaundryFinished(int code, const QString &result)
{

    QStringList users,services,types,phones,extras;
    QJsonParseError jsonParseError;
    QJsonDocument document = QJsonDocument::fromJson(result.toUtf8(), &jsonParseError);
    if(jsonParseError.error == QJsonParseError::NoError)
    {
        QJsonObject result = document.object();
        QJsonValue resultValue = result.take("results");
        if(resultValue.isArray())
        {
            qDebug()<<resultValue;
            QJsonArray jsonArray = resultValue.toArray();
            for(int i=0;i<jsonArray.size();i++)
            {
                QJsonObject jsonObj = jsonArray.at(i).toObject();
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
    json.insert("num",num);
    json.insert("service",service);
    json.insert("type",type);
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
        type = "床上用品";
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
}

void MainWindow::showMain(QString username)
{
    this->show();
    ui->username->setText(username);
}
