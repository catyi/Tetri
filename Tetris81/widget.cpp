#include "widget.h"
#include "ui_widget.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    me = new Munu;

    //创建数据库
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("sjdb");
    bool ok = mydb.open();
    //创建用户名+密码表
    QString  str("create table if not exists nampass(name, pass);");
    QSqlQuery query;
    query.exec(str);

    //创建分数表单(名字 分数 ）
    QString str1("create table if not exists grades(name, grade1,grade2);");
    QSqlQuery query1;
    query1.exec(str1);

    connect(this,SIGNAL(login_successed(QString)),this->me,SLOT(receiveData(QString)));
    connect(this, &Widget::login_successed1,[=](){
        this->hide();
        this->me->show();
    });

}

Widget::~Widget()
{
    delete ui;
}

//注册界面
void Widget::on_regist_clicked()
{
    //查询是否有重名
    QString str1 = QString("select * from nampass where name = '%0';").arg(ui->name->text());
    QSqlQuery query1;
    query1.exec(str1);

    //qDebug()<<str1;
    //qDebug()<<query1.record();

    if(query1.next())
    {
        qDebug()<<"not";
        ui->lineEdit->setText("用户名已存在");
    }
    else
    {
        qDebug()<<"yes";
    //未有重名，向入据库中加入一条信息
        QString  str = QString("insert into nampass values('%0', '%1');").arg(ui->name->text()).arg(ui->password->text());
        QSqlQuery query;
        query.exec(str);
        QString  str1 = QString("insert into grades values('%0', '%1','%2');").arg(ui->name->text()).arg(QString::number(0)).arg(QString::number(0));
        QSqlQuery query1;
        query1.exec(str1);
        ui->lineEdit->setText("注册成功");
    }
}

//登录界面
void Widget::on_login_clicked()
{
    //去数据库中查询一条信息
    QString  str = QString("select * from nampass where name='%0' and pass='%1';").arg(ui->name->text()).arg(ui->password->text());
    QSqlQuery query;
    query.exec(str);

    qDebug() << str;

    if(query.next())
    {
        qDebug() << query.value(0).toString();
        qDebug() << query.value(1).toString();
        ui->lineEdit->setText("登陆成功");
      //  qDebug()<<query.record();
        emit login_successed1();
        emit login_successed(ui->name->text());
    }else{
         qDebug()<<"login failed";
         ui->lineEdit->setText("密码不正确");
    }
}

//退出
void Widget::on_e0_clicked()
{
    close();
}
