#ifndef WINDOW_SIGNUP_H
#define WINDOW_SIGNUP_H

#include <QDialog>

namespace Ui {
class SignupWindow;
}

class SignupWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SignupWindow(QWidget *parent = 0);
    ~SignupWindow();

signals:
    void signupSucceed(QString);

private slots:
    void showSignup();
    void on_pushButton_clicked();
    void slot_signuRequestFinished(int, const QString&);

private:
    Ui::SignupWindow *ui;
};

#endif // WINDOW_SIGNUP_H
