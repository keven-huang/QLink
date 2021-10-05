#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"qlink.h"
#include <QMainWindow>
#include<QLabel>
#include<QPainter>
#include<QTime>
#include<QDebug>
#include<QTimer>
#include<QProgressBar>
#include<QFile>

/* 游戏模块类
 * 初始化共做5件事情
 * 1.初始化activated block
 * 2.将激活状态设置为false
 * 3.将激活方块数组初始化
 * 4.将绘图用点集合初始化
 * 5.将Point初始化为0
 */

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void TimerEvent();
    void ItemEvent();
    void hide();
public:
    MainWindow(QWidget *parent = nullptr,int mode = 1);
    ~MainWindow();
    void loadmap();
    void load();
    void initGame();
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void fig_op(int f,char op);
    int mode;
    QLink* game;
    QLabel* label[MapCol*MapRow];
    QLabel* figure1;
    QLabel* figure2;
    QTimer* GameTimer;
    QTimer* ItemTimer;
    QFile save_file;

private:
    bool pause;
    Ui::MainWindow *ui;
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_pause_clicked();
    void on_save_clicked();
    void on_exit_clicked();
signals:
    //退出时释放信号，重新显示Menu
    void ExitWin();
};
#endif // MAINWINDOW_H
