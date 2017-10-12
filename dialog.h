#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_btn_confirm_clicked();
    void showThis(QString);
    void changeContent(QString);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
