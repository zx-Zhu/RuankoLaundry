#include "dialog.h"
#include "window_login.h"
#include "window_main.h"
#include "window_signup.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow l;
    MainWindow m;
    Dialog d;
    d.setFixedSize(200,100);
    l.setFixedSize(410, 305);
    m.setFixedSize(808, 600);
    SignupWindow s;
    s.setFixedSize(410, 305);
    l.show();
    QObject::connect(&m,SIGNAL(showDialog(QString)),&d,SLOT(showThis(QString)));
    QObject::connect(&m,SIGNAL(changeDialogContent(QString)),&d,SLOT(changeContent(QString)));
    QObject::connect(&l,SIGNAL(jumpToMian(QString)),&m,SLOT(showMain(QString)));
    QObject::connect(&l,SIGNAL(jumpToSignup()),&s,SLOT(showSignup()));
    QObject::connect(&s,SIGNAL(signupSucceed(QString)),&l,SLOT(signupSucceed(QString)));

    return a.exec();
}
