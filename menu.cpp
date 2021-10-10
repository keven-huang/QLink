#include "menu.h"
#include "ui_menu.h"

Menu::Menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Menu)
{
    ui->setupUi(this);
    this->setObjectName("menu");
    this->setStyleSheet("#menu{border-image:url(:/res/background.jfif);}");
}

Menu::~Menu()
{
    delete ui;
}

void Menu::on_single_btn_clicked()
{
    mw = new MainWindow(this,1);
    mw->show();
    this->hide();
    connect(mw,SIGNAL(ExitWin()),this,SLOT(show()));
}

void Menu::on_exit_btn_clicked()
{
    this->close();
}

void Menu::on_double_btn_clicked()
{
    mw = new MainWindow(this,2);
    mw->show();
    this->hide();
    connect(mw,SIGNAL(ExitWin()),this,SLOT(show()));
}

void Menu::on_contiue_btn_clicked()
{
    mw = new MainWindow(this,3);
    mw->show();
    this->hide();
    connect(mw,SIGNAL(ExitWin()),this,SLOT(show()));
}
