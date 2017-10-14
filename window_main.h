#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setOrderTable();
    void askOrderList();
    void setMemberTable();
    void askMemberList();
    void setManageTable();
    void askManageList();
    void setOrderSearch(QJsonArray);

private:
    Ui::MainWindow *ui;

signals:
    void showDialog(QString);
    void changeDialogContent(QString);

private slots:
    void showMain(QString username);
    void on_checkBox_clicked(bool checked);
    void on_checkBox_2_clicked(bool checked);
    void on_checkBox_3_clicked(bool checked);
    void on_spinBox_valueChanged(int arg1);
    void slot_sendLaundryFinished(int, const QString&);
    void slot_getLaundryFinished(int, const QString&);
    void on_btn_confirm_clicked();
    void on_radioButton_toggled(bool checked);
    void on_radioButton_2_toggled(bool checked);
    void on_radioButton_3_toggled(bool checked);
    void on_radioButton_4_toggled(bool checked);
    void on_radioButton_5_toggled(bool checked);
    void on_btn_order_clicked();
    void on_btn_history_clicked();
    void on_tabWidget_currentChanged(int index);
    void slot_getMemberFinished(int, const QString&);
    void on_exit_clicked();
    void slot_getManageFinished(int, const QString&);
    void on_btn_search_order_clicked();
    void on_btn_right_clicked();
    void on_btn_left_clicked();
};

#endif // WINDOW_MAIN_H
