#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include"mainwindow.h"
namespace Ui {
class Menu;
}

class Menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
    void on_single_btn_clicked();

    void on_exit_btn_clicked();

    void on_double_btn_clicked();

    void on_contiue_btn_clicked();

private:
    Ui::Menu *ui;
    MainWindow *mw;
};

#endif // MENU_H
