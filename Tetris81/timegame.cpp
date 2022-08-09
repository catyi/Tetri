#include "timegame.h"
#include "ui_timegame.h"
#include <QTimerEvent>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTime>
#include <QPainter>
#include <QString>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#define RECT_COLUMES	15 //游戏区 15个小格子宽
#define RECT_ROWS		20 //游戏区 25个小格子高
#define RECT_WIDTH		20 //小格子像素宽
#define RECT_HEIGHT		20 //小格子像素高

//默认出生点 x方向
#define DEFAULT_BORN_POS_X    (RECT_COLUMES / 2 - 1)
TimeGame::TimeGame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeGame)
{
    ui->setupUi(this);
    setFixedSize(500,450);
    ui->Score1->setText(QString::number(mScore));

}

TimeGame::~TimeGame()
{
    delete ui;
}
void TimeGame::recv1(QString data1)
{
    qDebug()<<data1;
    //data1 用户名
    user3 = data1;
}
//放弃
void TimeGame::on_pushButton_3_clicked()
{
    killTimer(mTimerID);
    timer1->stop();
    QString s;
    s += "ARE YOU SURE?\n";
    s += "Score not save\n";
    QMessageBox:: StandardButton result=QMessageBox::information(this,tr("GAME GIVE"),s,QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::No == result)
    {
        mTimerID=startTimer(GetLevelTime(mLevel));//开启游戏定时器
        timer1->start();
        playtime.restart();
    }
    else{
        stopgame();
        emit back3();
    }
}
//开始
void TimeGame::on_pushButton_2_clicked()
{
    qDebug()<<user3;
    timer1 = new QTimer(this);
    connect(timer1,SIGNAL(timeout()),this,SLOT(timeover()));
    timer1->start(1000);//定时事件

    NewGame();
}
//更新成绩
void TimeGame::upgrade()
{
    //1. 查询三个表中的数据
    qDebug()<<user3;
    QString str1 = QString("select * from grades where name = '%1';").arg(user3);
    qDebug()<<str1;
    QSqlQuery query1;
    query1.exec(str1);
    int gScore = query1.value(1).toInt();
    qDebug()<<gScore;
    if(mScore >= gScore)
    {
        QString str2 = QString("update grades set grade1 = '%0'  where name = '%1';").arg(QString::number(mScore)).arg(user3);
        qDebug()<<str2;
        QSqlQuery query2;
        query2.exec(str2);
    }
}
//游戏开始
void TimeGame::NewGame()
{
    mScore =0;
    mLevel = 1;
    mTimerID = startTimer( GetLevelTime(mLevel));
    playtime.start();//游戏控制 
    mFixItems.mPoints.clear();
    //mCurItem 和 mNextItem 使用不同随机因子，避免初始一样
    mCurItem.InitNew(QTime::currentTime().msec());
    mCurItem.MoveTo(DEFAULT_BORN_POS_X, 1);

    mNextItem.InitNew(QTime::currentTime().second());
   // emit sigUpdateNextItem(mNextItem.mType,mNextItem.mShape);


}

//键盘响应
void TimeGame::keyPressEvent(QKeyEvent *e)
{
    int x = 0;
    int y = 0;
    switch(e->key())
    {
    case Qt::Key_A:
    {
        x = -1;
        break;
    }
    case Qt::Key_Z:
    {
        mCurItem.ChangeShape();
        if(HitSide() || HitBottom())
        {
            mCurItem.ChangeShape(-1);
        }
        return;
    }
    case Qt::Key_D:
    {
        x = 1;
        break;
    }
    case Qt::Key_W:
    {
        y = 1;
        break;
    }
    case Qt::Key_S:
    {
        //空格键直接移到底部
        while(1)
        {
            mCurItem.Move(0,1);
            if(HitSide() || HitBottom())
            {
                mCurItem.Move(0,-1);
                break;
            }
        }
        return;
    }
    }
    mCurItem.Move(x,y);

    if (HitSide() || HitBottom())
    {
        mCurItem.Move(-x,-y);
    }
}
//绘画
void TimeGame::paintEvent(QPaintEvent *)
{

    DrawBKRects();      //绘制游戏区被景
    DrawFixedRects();   //绘制固定方块
    DrawCurItem();      //绘制当前方块
    ui->Time1->setText(QString::number(sc));
    update();
}
//背景的方块
void TimeGame::DrawBKRects()
{
    QPainter painter(this);
    painter.setBrush(QColor("#797979"));
    painter.setPen(Qt::NoPen);

    for(int i = 0;i<RECT_COLUMES; i++)
    {
        for (int j = 0; j<RECT_ROWS; j++)
        {
            //第0列和最后一列和第0行和最后一行
            if(i==0||i==RECT_COLUMES-1||j==0||j==RECT_ROWS-1)
                //drawRect(左上角x座标，左上角y座标，方格宽，方格高）
                painter.drawRect(i*RECT_HEIGHT,j*RECT_WIDTH,RECT_WIDTH,RECT_HEIGHT);
        }
    }
}
//固定
void TimeGame::DrawFixedRects()
{
    QPainter painter(this);
    painter.setBrush(QColor("#D3D3D3"));
    painter.setPen(QPen(QColor(Qt::black),1));

    mFixItems.Draw(painter,0,0,RECT_WIDTH,RECT_HEIGHT);
}
//下落
void TimeGame::DrawCurItem()
{
    QPainter painter(this);
    painter.setBrush(QColor("#FFDEAD"));
    painter.setPen(QPen(QColor(Qt::black),1));

    mCurItem.Draw(painter,0,0,RECT_WIDTH,RECT_HEIGHT);
}
//时间到了
void TimeGame::timeover()
{
    sc -= 1;
    if(sc <= 0)
    {
        upgrade();//更新成绩
        killTimer(mTimerID);//结束                
        timer1->stop();
        QString s;
        s += "GAME SUCCESS\n";
        s += QString("SCORE:%1\n").arg(mScore);
        QMessageBox:: StandardButton result=QMessageBox::information(this,tr("GAME OVER"),s,QMessageBox::No|QMessageBox::Reset);
        if(QMessageBox::Reset == result)
                NewGame();
        else
        {

               stopgame();
               emit back3();
        }
    }
}
//时间
void TimeGame::timerEvent(QTimerEvent* e)
{
    if (e->timerId() == mTimerID)
    {
        mCurItem.Move(0,1);
        if (HitBottom())//碰到下边界或固定方块
        {
            mCurItem.Move(0,-1);//上移
            AddToFixedRects();//加入
            DeleteFullRows();//满格则删

            if (HitTop())//碰到上边界
            {
                killTimer(mTimerID);//结束
                timer1->stop();//结束
                QString s;
                s += "GAME OVER\n";
                s += QString("SCORE:%1\n").arg(mScore);
                QMessageBox:: StandardButton result=QMessageBox::information(this,tr("GAME OVER"),s,QMessageBox::Yes|QMessageBox::Reset);
                if(QMessageBox::Reset == result)
                    NewGame();
                else{
                    stopgame();
                    emit back3();
                }
            }
            mCurItem = mNextItem; //产生新的当前方块类形，直接用Vector 值赋值Vector 变量
            mCurItem.MoveTo(DEFAULT_BORN_POS_X, 1); //移到中间位置

            mNextItem.InitNew();
            //emit sigUpdateNextItem(mNextItem.mType,mNextItem.mShape);
        }
    }
}

bool TimeGame::HitSide()
{
    for (int i = 0; i<mCurItem.mPoints.size(); i++)
    {
        QPoint pt = mCurItem.mPoints[i];
        if (pt.x() <= 0 || pt.x() >= RECT_COLUMES - 1)//如果X坐标小于等于0或大于等于RECT_COLUMES - 1，则撞到边界了
        {
            return true;
        }
    }
    return false;
}

bool TimeGame::HitBottom()
{
    for (int i = 0; i<mCurItem.mPoints.size(); i++)
    {
        QPoint pt = mCurItem.mPoints[i];
        if (pt.y() >= RECT_ROWS - 1)//如果y座标大于等于RECT_ROWS-1，则表示撞上下边界了
        {
            return true;
        }
        if (mFixItems.mPoints.contains(pt))// 如果固定不动的方块的座标包含当前下落方块的座标，则表示下到底了
        {
            return true;
        }
    }
    return false;
}

bool TimeGame::HitTop()
{
    for (int i = 0; i<mFixItems.mPoints.size(); i++)
    {
        QPoint pt = mFixItems.mPoints[i];
        if (pt.y() <= 1)//如果y座标小于等于1， 则表示撞上边界了
        {
            return true;
        }
    }
    return false;
}

void TimeGame::AddToFixedRects()//把当前方块座标加入到固定方块
{
    mFixItems.AddPoints(mCurItem.mPoints);
}

void TimeGame::DeleteFullRows()//删除完整的行
{
    int nRowsDeleted = 0;
    for (int i = 1; i<RECT_ROWS-1; i++)
    {
        int nCount = 0;
        for (int j = 1; j<RECT_COLUMES-1; j++)
        {
            if (mFixItems.mPoints.contains(QPoint(j,i)))
            {
                nCount++;
            }
        }
        if (nCount >= RECT_COLUMES-2)
        {
            mFixItems.DeleteRow(i);
            mFixItems.MoveDown(i,1);	//消除行之上的内容下移一个单位
            nRowsDeleted++;
        }
    }

    //一次元素落下，最多可能消4行
    //一次消除的越多，得分越多
    if (nRowsDeleted == 1)
    {
        mScore += 100;
    }
    else if (nRowsDeleted == 2)
    {
        mScore += 250;
    }
    else if (nRowsDeleted == 3)
    {
        mScore += 400;
    }
    else if (nRowsDeleted == 4)
    {
        mScore += 550;
    }
    ui->Score1->setText(QString::number(mScore));    //更新得分

    //粗略使用每1000分一关
    if(mScore >= 1000 * mLevel)
    {
        mLevel++;

        //随关卡增加下落速度，即把定时器加快
        killTimer(mTimerID);
        mTimerID = startTimer( GetLevelTime(mLevel) );

    }
}

//停止
void TimeGame::stopgame()
{
    mCurItem.mPoints.clear(); //Vector 的clear 函数，清除座标
    mNextItem.mPoints.clear();
    mFixItems.mPoints.clear();
    timer1->stop();
    delete timer1;
    sc=60*3;

    killTimer(mTimerID);// 停止定时器

}
//时间设计
int TimeGame::GetLevelTime(int sc)
{

    if(sc < 60)
    {
        return 300;
    }
    else
    {
        int l = sc/60;
        return (7 - l) * 100;
    }
}



