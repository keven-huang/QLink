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

const int Left = 100;
const int Top = 100;
const int ItemTime=10000;
const int DizTime=1000;
const int PlayTime = 300;
/* MainWindow
 * initQlink()初始化Qlink中设定
 * private slots分别是保存，暂停，退出的槽函数
 * public slots分别是计时器，道具出现，眩晕函数
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
    void DizEvent1();
    void DizEvent2();
    void hide();
public:
    MainWindow(QWidget *parent = nullptr,int mode = 1);
    ~MainWindow();
    int mode;

    void load();
    void loadmap();
    void initQlink();

    void fig_op(int f,char op);
    QLink* game;
    QLabel* label[MapCol*MapRow];
    QLabel* figure1;
    QLabel* figure2;
    QTimer* GameTimer;
    QTimer* ItemTimer;
    bool dizTime1,dizTime2;
    QTimer* dizTimer1;
    QTimer* dizTimer2;
    QFile save_file;

private:
    bool pause;
    Ui::MainWindow *ui;
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
private slots:
    void on_pause_clicked();
    void on_save_clicked();
    void on_exit_clicked();
signals:
    //退出时释放信号，重新显示Menu
    void ExitWin();
};
#endif // MAINWINDOW_H
