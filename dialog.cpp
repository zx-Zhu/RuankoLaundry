#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>
using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showThis(QString str)
{
    cout<<"zzxzzx"<<endl;
    this->show();
    ui->text->setText(str);
}
void Dialog::on_btn_confirm_clicked()
{
    this->hide();
}
void Dialog::changeContent(QString str)
{
    ui->text->setText(str);
}
