#ifndef WINDOW_LOGIN_H
#define WINDOW_LOGIN_H

#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_btn_login_clicked();
    void slot_requestFinished(int code, const QString &strResult);
    void signupSucceed(QString);

private:
    Ui::LoginWindow *ui;
    void sendLoginRequest(QObject);

signals:
    void jumpToMian(QString);
    void jumpToSignup();
protected:
     bool eventFilter(QObject *obj, QEvent *ev);
};

#endif // WINDOW_LOGIN_H
