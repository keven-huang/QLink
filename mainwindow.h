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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initGame();
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    int mode;
    game_model* game;
    QLabel* label[MAX_COL*MAX_ROW];
    QLabel* figure1;
    QLabel* figure2;
    QTimer* GameTimer;
    QTimer* ItemTimer;
    void paintline();

private:
    bool pause;
    Ui::MainWindow *ui;
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void on_pause_clicked();
    void on_save_clicked();
};
#endif // MAINWINDOW_H
