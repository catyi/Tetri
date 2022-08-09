#include "playtotal.h"
#include "ui_playtotal.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

Playtotal::Playtotal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Playtotal)
{
    ui->setupUi(this);
    setFixedSize(700,550);
    // 信号传递
    connect(ui->widget,&GamePlay::sigUpdateScore,this,&Playtotal::slotUpdateScore);
    connect(ui->widget,&GamePlay::sigUpdateLevel,this,&Playtotal::slotUpdateLevel);
    connect(ui->widget,&GamePlay::sigUpdateNextItem,ui->widget_2,&NextArea::slotUpdateNextItem);
    connect(this,&Playtotal::time1,ui->widget,&GamePlay::pause);
    connect(ui->widget,&GamePlay::gameover,this,&Playtotal::upgrade);
}

 Playtotal::~ Playtotal()
{
    delete ui;
}
//游戏主运行逻辑在GamePlay内，不过按键消息因为是Playtotal接受，所以改
void  Playtotal::keyPressEvent(QKeyEvent *e)
{
    ui->widget->KeyPressed(e->key());
}
void Playtotal::recv2(QString data2)//接收用户名字
{
    qDebug()<<data2;
    user4 = data2;

}
//成绩更新
void Playtotal::upgrade(int nScore)
{
    //1. 查询三个表中的数据
    mScore = nScore;
    qDebug()<<user4;
    QString str1 = QString("select * from grades where name = '%1';").arg(user4);
    qDebug()<<str1;
    QSqlQuery query1;
    query1.exec(str1);
    int gScore = query1.value(2).toInt();
    qDebug()<<gScore;
    if(mScore >= gScore)
    {
        QString str2 = QString("update grades set grade2 = '%0'  where name = '%1';").arg(QString::number(mScore)).arg(user4);
        qDebug()<<str2;
        QSqlQuery query2;
        query2.exec(str2);
    }
}
//分数更新
void  Playtotal::slotUpdateScore(int nScore)
{
    ui->label_score->setText(QString::number(nScore));
}
//等级不一样，匹配的速度不一样
void Playtotal::slotUpdateLevel(int nSpeed)
{
    ui->label_level->setText(QString::number(nSpeed));
}
//返回界面
void Playtotal::on_pushButton_3_clicked()
{
    ui->widget->stopgame();
    emit back2();
}

//重开
void Playtotal::on_pushButton_clicked()
{
    ui->widget->stopgame();
    ui->widget->NewGame();
}

void Playtotal::on_radioButton_clicked(bool checked)
{
    ti=checked;
    qDebug()<<ti;
    emit time1(ti);
}

void Playtotal::on_pushButton_2_clicked()
{
    ui->widget->NewGame();
}
