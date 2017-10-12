#include "window_login.h"
#include "ui_loginwindow.h"
#include "util_http.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>


QLabel *signup;
QPushButton *login;
QLineEdit *username, *password;
QLabel *tittle;
QString str_user, str_pass;
LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    signup = ui->btn_signup;
    login = ui->btn_login;
    username = ui ->edit_username;
    password = ui ->edit_password;
    tittle = ui ->text_tittle;
    signup->installEventFilter(this);


}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_btn_login_clicked()
{
    str_user = username->text();
    str_pass = password->text();
    if (str_user.isEmpty() || str_pass.isEmpty())
    {
        tittle->setText("用户名和密码不能为空");
    }
    else
    {
        tittle->setText("正在登陆请稍等...");
        HttpUtil* pHttpFun = new HttpUtil();
        connect(pHttpFun,SIGNAL(signal_requestFinished(int,const QString&)),this,
                SLOT(slot_requestFinished(int, const QString&))); //关联请求完成信号
        QString strUrl = "https://fijosi0r.api.lncld.net/1.1/login";
        QJsonObject json;
        json.insert("username", str_user);
        json.insert("password", str_pass);
        QJsonDocument document;
        document.setObject(json);
        QByteArray dataArray = document.toJson(QJsonDocument::Compact);
        pHttpFun->sendPostRequest(strUrl,"application/json",dataArray);

    }
}

bool LoginWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == signup && event->type() == QEvent::MouseButtonPress)
    {;
        emit jumpToSignup();
        return true;
    } else return QDialog::eventFilter(obj, event);
}


void LoginWindow::slot_requestFinished(int code, const QString &strResult)
{
    switch (code) {
    case 200:
        emit jumpToMian(str_user);
        this->hide();
        break;
    case 400:
        tittle->setText(tr("账号或密码错误"));
        break;
    default:
        qDebug()<<code;
        tittle->setText(tr("登录失败"));
        break;
    }
}
void LoginWindow::signupSucceed(QString text)
{
    tittle->setText(tr("注册成功"));
    username->setText(text);
}

