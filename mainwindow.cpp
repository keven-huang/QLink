#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMessageBox>
#include<QKeyEvent>
#include<QPalette>
#include<QMovie>



MainWindow::MainWindow(QWidget *parent, int m)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1100,650);
    mode = m;
    initQlink();
}

MainWindow::~MainWindow()
{
    delete game;
    for(int i = 0;i<MapCol*MapRow;i++){
        delete label[i];
    }
    delete figure1;
    if(mode==2)
        delete figure2;
    delete ItemTimer;
    delete GameTimer;
    delete ui;
}
//创建子窗口时调用，人物模型以及箱子图片加载
void MainWindow::loadmap()
{
    if(game){
        for(int i = 0;i<MapCol*MapRow;i++){
            label[i] = new QLabel(this);
            label[i]->setGeometry(Left + (i % MapCol) * IconSize, Top + (i / MapCol) * IconSize, IconSize, IconSize);
            label[i]->show();
            QString Icon_name=QString("://res\\image/%1.jfif").arg(game->Get_gamemap()[i]);
            if(game->Get_gamemap()[i]){
                label[i]->setPixmap(QPixmap(Icon_name));
                label[i]->setScaledContents(true);
            }else
                label[i]->hide();
        }
        figure1 = new QLabel(this);
        figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
        QMovie *fig = new QMovie("://res\\image/fig1.gif");
        fig->start();
        figure1->setMovie(fig);
        figure1->setScaledContents(true);
        ui->Point_2->hide();
        ui->Point_1->setText("Points:0");
        if(mode==2){
            figure2 = new QLabel(this);
            figure2->setGeometry(Left + game->fig2.Posy * IconSize, Top + game->fig2.Posx * IconSize, IconSize, IconSize);
            QMovie *fig2 = new QMovie("://res\\image/fig1.gif");
            fig2->start();
            figure2->setMovie(fig2);
            figure2->setScaledContents(true);
            ui->Point_1->show();
            ui->Point_2->show();
            ui->Point_1->setText("Player 1 Points:0");
            ui->Point_2->setText("Player 2 Points:0");
        }
    }
}

/*load（）
 * 将map内的图片，人物模型重新加载
 * 1.shuffle时重新加载图片时使用
 * 2.倒计时结束或者胜利对话框出现后选择Retry重新加载游戏使用（不用init，不用删除并重新创建指针）
 */
void MainWindow::load()
{
    qDebug()<<"shuffle";
    for(int i = 0;i<MapCol*MapRow;i++)
        label[i]->hide();
    if(game){
        for(int i = 0;i<MapCol*MapRow;i++){
            label[i]->setGeometry(Left + (i % MapCol) * IconSize, Top + (i / MapCol) * IconSize, IconSize, IconSize);
            label[i]->show();
            QString Icon_name=QString("://res\\image/%1.jfif").arg(game->Get_gamemap()[i]);
            if(game->Get_gamemap()[i]){
                label[i]->setPixmap(QPixmap(Icon_name));
                label[i]->setScaledContents(true);
            }else
                label[i]->hide();
        }
        figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
        if(mode==2)
            figure2->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
    }
}

/*initQlink()
 * 初始化游戏界面
 * 根据mode来进行界面初始化并且启动Timer
 * mode = 1 2时载入地图，
 * mode = 3时从文件载入地图
 */
void MainWindow::initQlink()
{
    pause = false;
    if(mode==1||mode==2){
        game = new QLink(mode);
        loadmap();
        //倒计时
        ui->Timer->setDigitCount(3);
        ui->Timer->setMode(QLCDNumber::Dec);
        ui->Timer->setSegmentStyle(QLCDNumber::Filled);
        ui->Timer->display(QString::number(PlayTime));
        GameTimer = new QTimer(this);
        connect(GameTimer,SIGNAL(timeout()),this,SLOT(TimerEvent()));
        GameTimer->start(1000);
        ItemTimer = new QTimer(this);
        connect(ItemTimer,SIGNAL(timeout()),this,SLOT(ItemEvent()));
        ItemTimer->start(ItemTime);
        if(mode==2){
            dizTimer1 = new QTimer(this);
            dizTime1 = false;
            dizTimer2 = new QTimer(this);
            dizTime2 = false;
            connect(dizTimer1,SIGNAL(timeout()),this,SLOT(DizEvent1()));
            connect(dizTimer1,SIGNAL(timeout()),this,SLOT(DizEvent2()));
        }
    }
    else if(mode==3){
        game = new QLink;
        QFile file("save.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        in>>mode;
        for(int i = 0;i<MapCol*MapRow;i++){
            in>>game->Get_gamemap()[i];
        }
        int remain_time;
        in>>remain_time;
        in>>game->fig1.Posx;
        in>>game->fig1.Posy;
        if(mode==2){
            in>>game->fig2.Posx;
            in>>game->fig2.Posy;
        }
        ui->Timer->display(QString::number(remain_time));
        loadmap();
        GameTimer = new QTimer(this);
        connect(GameTimer,SIGNAL(timeout()),this,SLOT(TimerEvent()));
        GameTimer->start(1000);
        ItemTimer = new QTimer(this);
        connect(ItemTimer,SIGNAL(timeout()),this,SLOT(ItemEvent()));
        ItemTimer->start(ItemTime);
    }
    //初始化游戏时无解
    if(!game->StillCanLink()){
        int ret = QMessageBox::information(this, "QLink", "no linkable block,shuffle?",QMessageBox::Yes,QMessageBox::No);
        if(ret==QMessageBox::Yes){
            game->shuffle();
            load();
        }
    }
}

//按键函数
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // W A S D控制
    if(pause){
        return;
    }
    switch (event->key()) {
    case 0x57:
        fig_op(1,'W');
        break;
    case 0x41:
        fig_op(1,'A');
        break;
    case 0x53:
        fig_op(1,'S');
        break;
    case 0x44:
        fig_op(1,'D');
        break;
    // 上下左右
    case Qt::Key_I:
        if(mode != 2)
            break;
        else{
            fig_op(2,'W');
        }
        break;
    case Qt::Key_J:
        if(mode!=2)
            break;
        else{
            fig_op(2,'A');
        }
        break;
    case Qt::Key_K:
        if(mode!=2)
            break;
        else{
            fig_op(2,'S');
        }
        break;
    case Qt::Key_L:
        if(mode!=2)
            break;
        else{
            fig_op(2,'D');
        }
    }
}


//flash技能
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //flash
    if(game->flash_item){
        if(event->button()==Qt::LeftButton){
            qDebug()<<event->pos().x()<<event->pos().y();
            if(event->pos().x()<Left-IconSize||event->pos().x()>Left+(MapCol+1)*IconSize)
                return;
            else if (event->pos().y()<Top-IconSize||event->pos().y()>Top+(MapRow+1)*IconSize) {
                return;
            }
            else{
                int x = int((event->pos().y()-Top)/IconSize+0.5);
                int y = int((event->pos().x()-Left)/IconSize+0.5);
                qDebug()<<x<<y;
                if(event->pos().y()>Top-IconSize&&event->pos().y()<Top){
                    game->fig1.Posx = -1;
                    game->fig1.Posy = int((event->pos().x()-Left)/IconSize+0.5);
                    qDebug()<<"posy"<<game->fig1.Posy;
                    figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
                    game->flash_item = false;
                 }
                else if(event->pos().y()<Top+(MapRow+1)*IconSize&&event->pos().y()>Top+(MapRow)*IconSize){
                    game->fig1.Posx = MapRow;
                    game->fig1.Posy = int((event->pos().x()-Left)/IconSize+0.5);
                    figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
                    game->flash_item = false;
                }
                else if(event->pos().x()>Left-IconSize&&event->pos().x()<Left){
                    game->fig1.Posx = int((event->pos().y()-Top)/IconSize+0.5);
                    game->fig1.Posy = -1;
                    figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
                    game->flash_item = false;
                }
                else if(event->pos().x()<Left+(MapCol+1)*IconSize&&event->pos().x()>Left+(MapCol)*IconSize){
                    game->fig1.Posx = int((event->pos().y()-Top)/IconSize+0.5);
                    game->fig1.Posy = MapCol;
                    figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
                    game->flash_item = false;
                }
                else if(game->Get_gamemap()[x*MapCol+y]==0){
                    game->fig1.Posx = int((event->pos().y()-Top)/IconSize+0.5);
                    game->fig1.Posy = int((event->pos().x()-Left)/IconSize+0.5);
                    figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
                    game->flash_item = false;
                }
            }
        }
    }
}
//人物操作封装
void MainWindow::fig_op(int f,char op)
{
    if(f==1){
        if(op=='W'){
            if(game->fig2.dizzy_item){
                if(!dizTime1){
                    dizTime1 = true;
                    dizTimer1->start(DizTime);
                }
                game->fig_S(game->fig1);
            }
            else
                game->fig_W(game->fig1);
        }
        else if(op=='A'){
            if(game->fig2.dizzy_item){
                if(!dizTime1){
                    dizTime1 = true;
                    dizTimer1->start(DizTime);
                }
                game->fig_D(game->fig1);
            }
            else
                game->fig_A(game->fig1);
        }
        else if (op=='S') {
            if(game->fig2.dizzy_item){
                if(!dizTime1){
                    dizTime1 = true;
                    dizTimer1->start(DizTime);
                }
                game->fig_W(game->fig1);
            }
            else
                game->fig_S(game->fig1);
        }
        else if(op=='D'){
            if(game->fig2.dizzy_item){
                if(!dizTime1){
                    dizTime1 = true;
                    dizTimer1->start(DizTime);
                }
                game->fig_A(game->fig1);
            }
            else
                game->fig_D(game->fig1);
        }
        }
    else {
        if(op=='W'){
            if(game->fig1.dizzy_item){
                if(!dizTime2){
                    dizTime2 = true;
                    dizTimer2->start(DizTime);
                }
                game->fig_S(game->fig2);
            }
            else
                game->fig_W(game->fig2);
        }
        else if(op=='A'){
            if(game->fig1.dizzy_item){
                if(!dizTime2){
                    dizTime2 = true;
                    dizTimer2->start(DizTime);
                }
                game->fig_D(game->fig2);
            }
            else
                game->fig_A(game->fig2);
        }
        else if (op=='S') {
            if(game->fig1.dizzy_item){
                if(!dizTime2){
                    dizTime2 = true;
                    dizTimer2->start(DizTime);
                }
                game->fig_W(game->fig2);
            }
            else
                game->fig_S(game->fig2);
        }
        else if(op=='D'){
            if(game->fig1.dizzy_item){
                if(!dizTime2){
                    dizTime2 = true;
                    dizTimer2->start(DizTime);
                }
                game->fig_A(game->fig2);
            }
            else
                game->fig_D(game->fig2);
        }
    }
    if(f==1)
        figure1->setGeometry(Left + game->fig1.Posy * IconSize, Top + game->fig1.Posx * IconSize, IconSize, IconSize);
    else
        figure2->setGeometry(Left + game->fig2.Posy * IconSize, Top + game->fig2.Posx * IconSize, IconSize, IconSize);
    if(game->Shuffle_item){
        load();
        game->Shuffle_item = false;
        game->fig1.activated_block[0]=game->fig1.activated_block[1]=-1;
        if(mode==2)
            game->fig2.activated_block[0]=game->fig2.activated_block[1]=-1;
        if(!game->StillCanLink()){
            int ret = QMessageBox::information(this, "QLink", "no linkable block,shuffle?",QMessageBox::Yes,QMessageBox::No);
            if(ret==QMessageBox::Yes){
                game->shuffle();
                load();
            }
        }
    }
    if(mode==1){
        if(!game->fig1.blocks.empty()){
            qDebug()<<"second"<<game->fig1.blocks.size();
            update();
            QTimer::singleShot(300, this, SLOT(hide()));
            QString str = "Points:"+QString::number(game->fig1.Score);
            ui->Point_1->setText(str);
            if(!game->StillCanLink()){
                int ret = QMessageBox::information(this, "QLink", "no linkable block,shuffle?",QMessageBox::Yes,QMessageBox::No);
                if(ret==QMessageBox::Yes){
                    game->shuffle();
                    load();
                }
            }
            if(game->isWin()){
                ItemTimer->stop();
                GameTimer->stop();
                int ret = QMessageBox::information(this, "QLink", "win！",QMessageBox::Retry,QMessageBox::Close);
                if(ret==QMessageBox::Retry){
                    delete game;
                    game = new QLink(mode);
                    ui->Timer->setDigitCount(3);
                    ui->Timer->setMode(QLCDNumber::Dec);
                    ui->Timer->setSegmentStyle(QLCDNumber::Filled);
                    ui->Timer->display(QString::number(PlayTime));
                    GameTimer->start(1000);
                    ItemTimer->start(ItemTime);
                    load();
                }
                else{
                    ItemTimer->stop();
                    GameTimer->stop();
                    emit ExitWin();
                    close();
                }
            }
        }
        else{
            for(int i=0;i<MapCol*MapRow;i++)
                if(game->Get_gamemap()[i]==0)
                    label[i]->hide();
        }
    }
    else{
        if(!game->fig1.blocks.empty()||!game->fig2.blocks.empty()){
            update();
            QTimer::singleShot(300, this, SLOT(hide()));
            QString str1 = "Player 1 Points:"+QString::number(game->fig1.Score);
            qDebug()<<game->fig1.Score;
            ui->Point_1->setText(str1);
            QString str2 = "Player 2 Points:"+QString::number(game->fig2.Score);
            ui->Point_2->setText(str2);
            qDebug()<<game->fig2.Score;
            if(!game->StillCanLink()){
                int ret = QMessageBox::information(this, "QLink", "no linkable block,shuffle?",QMessageBox::Yes,QMessageBox::No);
                if(ret==QMessageBox::Yes){
                    game->shuffle();
                    load();
                }
            }
            if(game->isWin()){
                ItemTimer->stop();
                GameTimer->stop();
                int ret = QMessageBox::information(this, "QLink", "win!",QMessageBox::Retry,QMessageBox::Close);
                if(ret==QMessageBox::Retry){
                    delete game;
                    game = new QLink(mode);
                    load();
                }
                else{
                    ItemTimer->stop();
                    GameTimer->stop();
                    emit ExitWin();
                    close();
                }
            }
        }
        else if(game->fig1.blocks.empty()&&game->fig2.blocks.empty()){
            for(int i=0;i<MapCol*MapRow;i++)
                if(game->Get_gamemap()[i]==0)
                    label[i]->hide();
        }
    }
}


//消除方块
void MainWindow::hide()
{
    for(int i=0;i<MapCol*MapRow;i++)
        if(game->Get_gamemap()[i]==0)
            label[i]->hide();
    qDebug()<<"hide";
    update();

}

//用来更新倒计时，倒计时完成，则显示游戏结束
void MainWindow::TimerEvent()
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
        int ret = QMessageBox::information(this, "QLink", "game over",QMessageBox::Retry,QMessageBox::Close);
        if(ret==QMessageBox::Retry){
            delete game;
            game = new QLink(mode);
            ui->Timer->setDigitCount(3);
            ui->Timer->setMode(QLCDNumber::Dec);
            ui->Timer->setSegmentStyle(QLCDNumber::Filled);
            ui->Timer->display(QString::number(PlayTime));
            GameTimer->start(1000);
            ItemTimer->start(ItemTime);
            load();
        }
        else{
            ItemTimer->stop();
            GameTimer->stop();
            emit ExitWin();
            close();
        }
    }

}

//用来更新道具
void MainWindow::ItemEvent()
{
    int Item;
    if(mode==1)
        Item = (rand()%3 +2)*50;
    else {
        Item = (rand()%3+3)*50;
    }
    QString Item_name=QString("://res\\image/%1.jfif").arg(Item);
    bool has_space=false;
    for(int i = 0;i<MapCol*MapRow;i++)
        if(game->Get_gamemap()[i]==0)
            has_space  =true;
    while (has_space) {
        int pos = rand()%(MapCol*MapRow);
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

//角色1眩晕函数
void MainWindow::DizEvent1()
{
    game->fig2.dizzy_item =false;
    dizTime1 = false;
    dizTimer1->stop();
}

//角色2眩晕函数
void MainWindow::DizEvent2()
{
    game->fig1.dizzy_item =false;
    dizTime1 = false;
    dizTimer2->stop();
}

//paintevent函数，在update或人物移动时调用
void MainWindow::paintEvent(QPaintEvent *)
{
    //角色1模块激活
    if(game->fig1.activated_block[0]!=-1&&game->fig1.activated){
        int block_x = game->fig1.activated_block[0];
        int block_y = game->fig1.activated_block[1];
        QString Pix_name=QString("://res\\image/%1.jfif").arg(game->Get_gamemap()[block_x*MapCol+block_y]);
        QPixmap pix1(Pix_name);
        QPixmap temp(pix1.size());
        temp.fill(Qt::transparent);
        QPainter p1(&temp);
        p1.setCompositionMode(QPainter::CompositionMode_Source);
        p1.drawPixmap(0, 0, pix1);
        p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        p1.fillRect(temp.rect(), QColor(0, 0, 0, 140));
        p1.end();
        pix1 = temp;
        label[block_x*MapCol+block_y] ->setPixmap(pix1);
    }
    else if(game->fig1.activated_block[0]!=-1&&(!game->fig1.activated)){
        int block_x = game->fig1.activated_block[0];
        int block_y = game->fig1.activated_block[1];
        QString Pix_name=QString("://res\\image/%1.jfif").arg(game->Get_gamemap()[block_x*MapCol+block_y]);
        QPixmap pix1(Pix_name);
        label[block_x*MapCol+block_y] ->setPixmap(pix1);
    }
    //角色2模块激活
    if(mode==2&&game->fig2.activated_block[0]!=-1&&game->fig2.activated){
        int block_x = game->fig2.activated_block[0];
        int block_y = game->fig2.activated_block[1];
        QString Pix_name=QString("://res\\image/%1.jfif").arg(game->Get_gamemap()[block_x*MapCol+block_y]);
        QPixmap pix1(Pix_name);
        QPixmap temp(pix1.size());
        temp.fill(Qt::transparent);
        QPainter p1(&temp);
        p1.setCompositionMode(QPainter::CompositionMode_Source);
        p1.drawPixmap(0, 0, pix1);
        p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        p1.fillRect(temp.rect(), QColor(0, 0, 0, 64));
        p1.end();
        pix1 = temp;
        label[block_x*MapCol+block_y] ->setPixmap(pix1);
    }
    else if(mode==2&&game->fig2.activated_block[0]!=-1&&(!game->fig2.activated)){
        int block_x = game->fig2.activated_block[0];
        int block_y = game->fig2.activated_block[1];
        QString Pix_name=QString("://res\\image/%1.jfif").arg(game->Get_gamemap()[block_x*MapCol+block_y]);
        QPixmap pix1(Pix_name);
        label[block_x*MapCol+block_y] ->setPixmap(pix1);
    }
    //连接线绘制
    QPainter painter(this);
    QPen pen;
    pen.setWidth(5);
    if(mode==1&&!game->fig1.blocks.empty()){
        QColor color(0,0,0);
        pen.setColor(color);
        painter.setPen(pen);
        for(int i = 0;i<int(game->fig1.blocks.size())-1;i++){
            Point p_1 = game->fig1.blocks[i];
            Point p_2 = game->fig1.blocks[i+1];
            QPoint pos1(Left+p_1.y*IconSize+IconSize/2,Top+p_1.x*IconSize+IconSize/2);
            QPoint pos2(Left+p_2.y*IconSize+IconSize/2,Top+p_2.x*IconSize+IconSize/2);
            painter.drawLine(pos1,pos2);
        }
        game->fig1.blocks.clear();
    }
    if(mode==2&&!game->fig2.blocks.empty()){
        QColor color(0,0,255);
        pen.setColor(color);
        painter.setPen(pen);
        for(int i = 0;i<int(game->fig2.blocks.size())-1;i++){
            Point p_1 = game->fig2.blocks[i];
            Point p_2 = game->fig2.blocks[i+1];
            QPoint pos1(Left+p_1.y*IconSize+IconSize/2,Top+p_1.x*IconSize+IconSize/2);
            QPoint pos2(Left+p_2.y*IconSize+IconSize/2,Top+p_2.x*IconSize+IconSize/2);
            painter.drawLine(pos1,pos2);
        }
        game->fig2.blocks.clear();
    }
}

//暂停键槽函数
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
        ItemTimer->start(ItemTime);
        ui->pause->setText(QString::fromUtf8("暂停"));
        pause = false;
    }

}

// 保存save键槽函数
void MainWindow::on_save_clicked()
{
    int remain_time = ui->Timer->value();
    QFile file("save.txt");
    QString filename;
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out<<mode<<"\n";
    for(int i = 0;i<MapCol*MapRow;i++){
        out<<game->Get_gamemap()[i]<<" ";
    }
    out<<"\n";
    out<<remain_time<<"\n";
    out<<game->fig1.Posx<<" ";
    out<<game->fig1.Posy<<"\n";
    if(mode == 2){
        out<<game->fig2.Posx<<" ";
        out<<game->fig2.Posy;
    }
     QMessageBox::information(this, "save", "save success!");
}

//exit按钮槽函数
void MainWindow::on_exit_clicked()
{
    ItemTimer->stop();
    GameTimer->stop();
    emit ExitWin();
    this->close();
}
