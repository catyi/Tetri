#include "playmenu.h"
#include "ui_playmenu.h"
#include <QDebug>
PlayMenu::PlayMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayMenu)
{
    ui->setupUi(this);
    this->sta = new Playtotal;
    this->tplay = new TimeGame;
    connect(this,&PlayMenu::go,[=](){
        this->hide();
        this->sta->show();
    });
    connect(this,SIGNAL(go_user1(QString)),this->sta,SLOT(recv2(QString)));
    connect(this,&PlayMenu::go1,[=](){
       this->hide();
       this->tplay->show();
    });
    connect(this,SIGNAL(go_user(QString)),this->tplay,SLOT(recv1(QString)));
    connect(this->tplay,&TimeGame::back3,[=](){
        this->tplay->hide();
        this->show();
    });
    connect(this->sta,&Playtotal::back2,[=](){
       this->sta->hide();
        this->show();
    });
}

PlayMenu::~PlayMenu()
{
    delete ui;
}

void PlayMenu::on_e1_clicked()
{
    close();
}

void PlayMenu::on_b2_clicked()
{
    emit back1();
}

void PlayMenu::on_st1_clicked()
{
    emit go_user1(user2);
    emit go();
}

void PlayMenu::on_pushButton_clicked()
{
    emit go_user(user2);
    emit go1();

}
void PlayMenu::recv(QString data1)
{
    qDebug()<<data1;
    user2=data1;
}
