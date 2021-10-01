#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"game_model.h"
#include <QMainWindow>
#include<QLabel>
#include<QPainter>
#include<QTime>
#include<QDebug>
#include<QTimer>
#include<QProgressBar>
#include<QFile>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void timerevent();
    void ItemEvent();
    void hide();
public:
    //MainWindow(QWidget *parent = nullptr);
    MainWindow(QWidget *parent = nullptr,int mode = 1);
    ~MainWindow();
    void loadmap();
    void load();
    void initGame();
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void fig_op(int f,char op);
    int mode;
    game_model* game;
    QLabel* label[Map_Col*Map_Row];
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
    void ExitWin();
};
#endif // MAINWINDOW_H
