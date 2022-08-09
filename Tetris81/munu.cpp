#include "munu.h"
#include "ui_munu.h"
#include <QDebug>
#include <QSqlQuery>

Munu::Munu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Munu)
{
    ui->setupUi(this);
    this->playm = new PlayMenu;
    connect(this,&Munu::legin_play,[=](){
        this->hide();
        this->playm->show();
    });
   connect(this,SIGNAL(legin_user(QString)),this->playm,SLOT(recv(QString)));
    connect(this->playm,&PlayMenu::back1,[=](){
        this->playm->hide();
        this->show();
    });
}

Munu::~Munu()
{
    delete ui;
}

void Munu::on_message_clicked()
{
    //查询库中与名字对应的信息
    //1. 查询三个表中的数据
    qDebug()<<user1;
    QString str1 = QString("select * from grades where name = '%1';").arg(user1);
    qDebug()<<str1;
    QSqlQuery query1;
    qDebug()<<query1.value(0).toString();
    query1.exec(str1);
    //3.打印到 label 标签
    qDebug()<<"k";
    if(query1.next())
    {
        QString s;
        s+="姓名：";
        s+=query1.value(0).toString();
        s+="\n";
        s+="限时： ";
        s+=query1.value(1).toString();
        s+="\n";
        s+="不限时: ";
        s+=query1.value(2).toString();
        qDebug() << query1.value(0).toString();
        qDebug() << query1.value(1).toString();
        qDebug() << query1.value(2).toString();
        qDebug()<<query1.value(2).toInt();
        ui->label->setText(s);
    }
    qDebug()<<"q";

}

//拿到名字
void Munu::receiveData(QString data)
{
    qDebug()<<data;
    user1 = data;

}

void Munu::on_mode_clicked()
{
    emit legin_user(user1);
    emit legin_play();
}

void Munu::on_bt1_clicked()
{
    close();
}

