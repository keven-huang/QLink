#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include"mainwindow.h"
namespace Ui {
class Menu;
}

/* 目录类
 * menu类实现功能
 * 1.设置按钮连接槽函数
 * 2.点击按钮可以创建相应的子窗口或退出，按钮分别为单人游戏，双人游戏，继续游戏，退出
 * 3.子窗口创建则父类窗口隐藏，子窗口通过exit退出时回到父窗口
 * 4.通过mainwindow类初始化调用函数传递mode来确定生成子窗口类型
 *   mode 1 单人游戏（默认）
 *   mode 2 双人游戏
 *   mode 3 继续游戏
 */
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
