#include "gameplay.h"
#include "ui_gameplay.h"
#include <QTimerEvent>
#include <QMessageBox>
#include <QKeyEvent>
#include <QTime>
#include <QPainter>
#include <QString>
#include <QDebug>
#include <QDebug>

#define RECT_COLUMES	15 //游戏区 15个小格子宽
#define RECT_ROWS		20 //游戏区 25个小格子高
#define RECT_WIDTH		20 //小格子像素宽
#define RECT_HEIGHT		20 //小格子像素高

//默认出生点 x方向
#define DEFAULT_BORN_POS_X    (RECT_COLUMES / 2 - 1)

GamePlay::GamePlay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamePlay)
{
    //初始化
    mScore = 1500;
    mLevel = 1;
    setMinimumSize(RECT_COLUMES*RECT_WIDTH, RECT_ROWS*RECT_HEIGHT);

}

GamePlay::~GamePlay()
{
    delete ui;
}

//游戏开始
void GamePlay::NewGame()
{

    mScore = 1500;
    mLevel = 1;
    setMinimumSize(RECT_COLUMES*RECT_WIDTH, RECT_ROWS*RECT_HEIGHT);
    mFixItems.mPoints.clear();
    //mCurItem 和 mNextItem 使用不同随机因子，避免初始一样
    mCurItem.InitNew(QTime::currentTime().msec());
    mCurItem.MoveTo(DEFAULT_BORN_POS_X, 1);

    mNextItem.InitNew(QTime::currentTime().second());
    emit sigUpdateNextItem(mNextItem.mType,mNextItem.mShape);

    mScore = 0;
    mLevel = 1;
    mTimerID = startTimer( GetLevelTime(mLevel));
    playtime.start();//游戏控制
}

//键盘响应
void GamePlay::KeyPressed(int key)
{
    int x = 0;
    int y = 0;
    switch(key)
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

void GamePlay::paintEvent(QPaintEvent *)
{

    DrawBKRects();      //绘制游戏区被景
    DrawFixedRects();   //绘制固定方块
    DrawCurItem();      //绘制当前方块
    update();
}
//背景的方块
void GamePlay::DrawBKRects()
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
void GamePlay::DrawFixedRects()
{
    QPainter painter(this);
    painter.setBrush(QColor("#D3D3D3"));
    painter.setPen(QPen(QColor(Qt::black),1));

    mFixItems.Draw(painter,0,0,RECT_WIDTH,RECT_HEIGHT);
}
//下落
void GamePlay::DrawCurItem()
{
    QPainter painter(this);
    painter.setBrush(QColor("#FFDEAD"));
    painter.setPen(QPen(QColor(Qt::black),1));

    mCurItem.Draw(painter,0,0,RECT_WIDTH,RECT_HEIGHT);
}
//时间
void GamePlay::timerEvent(QTimerEvent* e)
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
                QString s;
                s += "GAME OVER\n";
                s += QString("SCORE:%1\n").arg(mScore);
                s += QString("Leval:%1").arg(mLevel);
                QMessageBox:: StandardButton result=QMessageBox::information(this,tr("GAME OVER"),s,QMessageBox::Yes|QMessageBox::Reset);
                emit gameover(mScore);
                if(QMessageBox::Reset == result)
                    NewGame();
                return;
            }
            mCurItem = mNextItem; //产生新的当前方块类形，直接用Vector 值赋值Vector 变量
            mCurItem.MoveTo(DEFAULT_BORN_POS_X, 1); //移到中间位置

            mNextItem.InitNew();
            emit sigUpdateNextItem(mNextItem.mType,mNextItem.mShape);
        }
    }
}

bool GamePlay::HitSide()
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

bool GamePlay::HitBottom()
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

bool GamePlay::HitTop()
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

void GamePlay::AddToFixedRects()//把当前方块座标加入到固定方块
{
    mFixItems.AddPoints(mCurItem.mPoints);
}

void GamePlay::DeleteFullRows()//删除完整的行
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
    emit sigUpdateScore(mScore);    //更新得分

    //粗略使用每1000分一关
    if(mScore >= 1000 * mLevel)
    {
        mLevel++;

        //随关卡增加下落速度，即把定时器加快
        killTimer(mTimerID);
        mTimerID = startTimer( GetLevelTime(mLevel) );

        emit sigUpdateLevel(mLevel);    //更新关卡
    }
}
//暂停
void GamePlay::pause(bool status)
{
    qDebug()<<status;
    if(status)
    {
        killTimer(mTimerID);//停止游戏计时器
    }
    else
    {
        mTimerID=startTimer(GetLevelTime(mLevel));//开启游戏定时器
        playtime.restart();
    }
}
//停止
void GamePlay::stopgame()
{

    mCurItem.mPoints.clear(); //Vector 的clear 函数，清除座标
    mNextItem.mPoints.clear();
    mFixItems.mPoints.clear();
    killTimer(mTimerID);// 停止定时器
    emit gameover(mScore);

}
//关卡设计
int GamePlay::GetLevelTime(int level)
{
    //第1关=1000ms,第2关=900ms 越来越快 第8关=300ms
    //关卡>8后，速度不再增加，保持200ms
    if(level > 8)
    {
        return 200;
    }
    if (level > 0)
    {
        return (11 - level) * 100;
    }
}
