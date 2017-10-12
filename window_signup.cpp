#include "window_signup.h"
#include "ui_signupwindow.h"
#include "util_http.h"
#include "iostream"

#include <qjsondocument.h>
#include <qjsonobject.h>
using namespace std;


SignupWindow::SignupWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignupWindow)
{
    ui->setupUi(this);
}

SignupWindow::~SignupWindow()
{
    delete ui;
}

void SignupWindow::on_pushButton_clicked()
{
    QString str_user = ui->sign_username->text(), str_pass = ui->sign_password->text(), str_pass_repeat = ui->sign_password_repeat->text();
    if(str_user.isEmpty()||str_pass.isEmpty()||str_pass_repeat.isEmpty())
    {
        ui->text->setText("用户名和密码不能为空");
    }
    if(ui->sign_password->text()==ui->sign_password_repeat->text())
    {
        ui->text->setText("正在注册...");
        HttpUtil* pHttpFun = new HttpUtil();
        connect(pHttpFun,SIGNAL(signal_requestFinished(int,const QString&)),this,
                SLOT(slot_signuRequestFinished(int, const QString&))); //关联请求完成信号
        QString strUrl = "https://fijosi0r.api.lncld.net/1.1/users";
        QJsonObject json;
        json.insert("username", ui->sign_username->text());
        json.insert("password", ui->sign_password->text());
        QJsonDocument document;
        document.setObject(json);
        QByteArray dataArray = document.toJson(QJsonDocument::Compact);
        pHttpFun->sendPostRequest(strUrl,"application/json",dataArray);
    } else {
        ui->text->setText("两次输入密码不相等");
    }
}

void SignupWindow::showSignup()
{
    this->show();
}

void SignupWindow::slot_signuRequestFinished(int code, const QString& result)
{
    qDebug()<<code;
    switch (code) {
    case 201:
        ui->text->setText("注册成功");
        emit signupSucceed(ui->sign_username->text());
        this->hide();
        break;
    case 400:
        ui->text->setText("该用户名已被注册");
        break;
    default:
        ui->text->setText("注册失败");
        break;
    }
}
