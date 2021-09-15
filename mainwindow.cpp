#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QKeyEvent>
#include<QPalette>
#include<QMovie>
const int LeftMargin = 50;
const int TopMargin = 70;
const int InitTime = 200;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //resize(1200,1000);
    //this->setBackgroundRole("white");
    mode = 1;
    initGame();
}

MainWindow::~MainWindow()
{
    delete game;
    for(int i = 0;i<MAX_COL*MAX_ROW;i++){
        delete label[i];
    }
    delete figure1;
    delete ItemTimer;
    delete GameTimer;
    delete ui;
}

void MainWindow::initGame()
{
    pause = false;
    if(mode==1){
        game = new game_model;
        for(int i = 0;i<MAX_COL*MAX_ROW-1;i++){
            label[i] = new QLabel(this);
            label[i]->setGeometry(LeftMargin + (i % MAX_COL) * ICON_SIZE, TopMargin + (i / MAX_COL) * ICON_SIZE, ICON_SIZE, ICON_SIZE);
            label[i]->show();
            QString Icon_name=QString("://res\\image/%1.jfif").arg(game->Get_gamemap()[i]);
            //Icon_name.asprintf("://res\\image/%1.jfif",game->Get_gamemap()[i]);
            if(game->Get_gamemap()[i]){
                label[i]->setPixmap(QPixmap(Icon_name));
                label[i]->setScaledContents(true);
            }else
                label[i]->hide();
        }

        figure1 = new QLabel(this);
        figure1->setGeometry(LeftMargin + game->fig1.Posy * ICON_SIZE, TopMargin + game->fig1.Posx * ICON_SIZE, ICON_SIZE, ICON_SIZE);
        QMovie *fig = new QMovie("://res\\image/fig1.gif");
        fig->start();
        figure1->setMovie(fig);
        figure1->setScaledContents(true);
        //时间
        ui->Timer->setDigitCount(3);//显示位数
        ui->Timer->setMode(QLCDNumber::Dec);//设置显示模式为十进制
        ui->Timer->setSegmentStyle(QLCDNumber::Filled);//设置显示外观Outline、Filled、Flat三种
        ui->Timer->display("200");
        GameTimer = new QTimer(this);
        connect(GameTimer,SIGNAL(timeout()),this,SLOT(timerevent()));

        GameTimer->start(1000);
        ItemTimer = new QTimer(this);
        connect(ItemTimer,SIGNAL(timeout()),this,SLOT(ItemEvent()));
        ItemTimer->start(30000);
    }
    else if(mode==2){

    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // W A S D控制
    if(pause){
        return;
    }
    switch (event->key()) {
    case 0x57:
        //if(game->fig1_W()==1){
          //  paintline();
        //}
        game->fig1_W();
        figure1->setGeometry(LeftMargin + game->fig1.Posy * ICON_SIZE, TopMargin + game->fig1.Posx * ICON_SIZE, ICON_SIZE, ICON_SIZE);
        if(!game->points.empty()){
            update();
            QTimer::singleShot(300, this, SLOT(hide()));
        }
        else{
            for(int i=0;i<MAX_COL*MAX_ROW-1;i++)
                if(game->Get_gamemap()[i]==0)
                    label[i]->hide();
        }
        break;
    case 0x41:
        game->fig1_A();
        figure1->setGeometry(LeftMargin + game->fig1.Posy * ICON_SIZE, TopMargin + game->fig1.Posx * ICON_SIZE, ICON_SIZE, ICON_SIZE);
        if(!game->points.empty()){
            update();
            QTimer::singleShot(300, this, SLOT(hide()));
        }
        else{
            for(int i=0;i<MAX_COL*MAX_ROW-1;i++)
                if(game->Get_gamemap()[i]==0)
                    label[i]->hide();
        }
        break;
    case 0x53:
        game->fig1_S();
        figure1->setGeometry(LeftMargin + game->fig1.Posy * ICON_SIZE, TopMargin + game->fig1.Posx * ICON_SIZE, ICON_SIZE, ICON_SIZE);
        if(!game->points.empty()){
            update();
            QTimer::singleShot(300, this, SLOT(hide()));
        }
        else{
            for(int i=0;i<MAX_COL*MAX_ROW-1;i++)
                if(game->Get_gamemap()[i]==0)
                    label[i]->hide();
        }
        break;
    case 0x44:
        game->fig1_D();
        figure1->setGeometry(LeftMargin + game->fig1.Posy * ICON_SIZE, TopMargin + game->fig1.Posx * ICON_SIZE, ICON_SIZE, ICON_SIZE);
        if(!game->points.empty()){
            update();
            QTimer::singleShot(300, this, SLOT(hide()));
        }
        else{
            for(int i=0;i<MAX_COL*MAX_ROW-1;i++)
                if(game->Get_gamemap()[i]==0)
                    label[i]->hide();
        }
        break;
    }
}


//flash技能
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //flash
    if(game->flash_item){
        if(event->button()==Qt::LeftButton){
            qDebug()<<event->pos().x()<<event->pos().y();
            if(event->pos().x()<LeftMargin||event->pos().x()>LeftMargin+(MAX_COL - 1)*ICON_SIZE)
                return;
            else if (event->pos().y()<TopMargin||event->pos().y()>TopMargin+(MAX_ROW-1)*ICON_SIZE) {
                return;
            }
            else{
                int x = int((event->pos().y()-TopMargin)/ICON_SIZE+0.5);
                int y = int((event->pos().x()-LeftMargin)/ICON_SIZE+0.5);
                if(game->Get_gamemap()[x*MAX_COL+y]==0){
                    game->fig1.Posx = int((event->pos().y()-TopMargin)/ICON_SIZE+0.5);
                    game->fig1.Posy = int((event->pos().x()-LeftMargin)/ICON_SIZE+0.5);
                    figure1->setGeometry(LeftMargin + game->fig1.Posy * ICON_SIZE, TopMargin + game->fig1.Posx * ICON_SIZE, ICON_SIZE, ICON_SIZE);
                    game->flash_item = false;
                }

            }
        }
    }
}

//消除方块
void MainWindow::hide()
{
    for(int i=0;i<MAX_COL*MAX_ROW-1;i++)
        if(game->Get_gamemap()[i]==0)
            label[i]->hide();
    update();

}

void MainWindow::paintline()
{
    QPainter painter(this);
    QPen pen;
    QColor color(rand()%256,rand()%256,rand()%256);
    pen.setColor(color);
    pen.setWidth(5);
    painter.setPen(pen);
    if(!game->points.empty()){
    for(int i = 0;i<int(game->points.size())-1;i++){
        point p_1 = game->points[i];
        point p_2 = game->points[i+1];
        QPoint pos1,pos2;
        pos1 = QPoint(LeftMargin+p_1.y*ICON_SIZE+ICON_SIZE/2,TopMargin+p_1.x*ICON_SIZE+ICON_SIZE/2);
        pos2 = QPoint(LeftMargin+p_2.y*ICON_SIZE+ICON_SIZE/2,TopMargin+p_2.x*ICON_SIZE+ICON_SIZE/2);
        painter.drawLine(pos1,pos2);
    }

    game->points.clear();
    }

}

//用来更新倒计时，倒计时完成，则显示游戏结束
void MainWindow::timerevent()
{
    if(ui->Timer->value()>0){
        if(game->Time_item){
            QString Time = QString::number(ui->Timer->value()+1);
            ui->Timer->display(Time);
            game->Time_item = false;
        }
        else{
            QString Time = QString::number(ui->Timer->value()-1);
            ui->Timer->display(Time);
        }

    }
    else if(ui->Timer->value()==0){
        GameTimer->stop();
        ItemTimer->stop();
        QMessageBox::information(this, "game over", "play again>_<");
    }

}
//用来更新道具
void MainWindow::ItemEvent()
{
    int Item = (rand()%3 +2)*50;
    QString Item_name=QString("://res\\image/%1.jfif").arg(Item);
    while (true) {
        int pos = rand()%(MAX_COL*MAX_ROW);
        if(game->Get_gamemap()[pos]==0){
            game->Get_gamemap()[pos]= Item;
            label[pos]->show();
            label[pos]->setPixmap(QPixmap(Item_name));
            label[pos]->setScaledContents(true);
            break;
        }
        else
            continue;
    }

}


void MainWindow::paintEvent(QPaintEvent *)
{

    if(!game->points.empty()){
        QPainter painter(this);
        QPen pen;
        QColor color(rand()%256,rand()%256,rand()%256);
        pen.setColor(color);
        pen.setWidth(5);
        painter.setPen(pen);
        for(int i = 0;i<int(game->points.size())-1;i++){
            point p_1 = game->points[i];
            point p_2 = game->points[i+1];
            QPoint pos1(LeftMargin+p_1.y*ICON_SIZE+ICON_SIZE/2,TopMargin+p_1.x*ICON_SIZE+ICON_SIZE/2);
            QPoint pos2(LeftMargin+p_2.y*ICON_SIZE+ICON_SIZE/2,TopMargin+p_2.x*ICON_SIZE+ICON_SIZE/2);
            painter.drawLine(pos1,pos2);
        }
        game->points.clear();
    }
    //figure->setGeometry(LeftMargin + game->fig1.Posy * ICON_SIZE, TopMargin + game->fig1.Posx * ICON_SIZE, ICON_SIZE, ICON_SIZE);
    //if(game->activated_block[0]!=-1)     //   label[game->activated_block[0]*MAX_COL+game->activated_block[1]]->setStyleSheet("Qlabel{background:yellow}");

}


void MainWindow::on_pause_clicked()
{
    if(!pause){
        GameTimer->stop();
        ItemTimer->stop();
        ui->pause->setText(QString::fromUtf8("继续"));
        pause = true;
    }
    else{
        GameTimer->start(1000);
        ItemTimer->start(30000);
        ui->pause->setText(QString::fromUtf8("暂停"));
        pause = false;
    }

}

void MainWindow::on_save_clicked()
{
    GameTimer->stop();
    ItemTimer->stop();
    int remain_time = ui->Timer->value();
    bool exist;
    QString filename;
    QFile Save_file("save.txt");
    QDataStream out(&Save_file);
    for(int i = 0;i<MAX_COL*MAX_ROW;i++){
        out<<game->Get_gamemap()[i];
    }
    out<<remain_time;
    out<<game->fig1.Posx;
    out<<game->fig1.Posy;
    //restart
    
    QFile file("file.xxx");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    



}
