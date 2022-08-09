#include "item.h"

#include <QTime>

Item::Item(ITEM_TYPE t,int nShape)
{
    mPos = QPoint(0,0);

    InitItem(t,nShape);
}


Item::~Item(void)
{
}

//初始化方块
void Item::InitNew(int nSeed)
{
    if(nSeed == 0)
    {
        //默认不传参，就使用当前时间作随机因子
        qsrand(QTime::currentTime().msec());
    }
    else
    {
        //传入随机因子
        qsrand(nSeed);
    }

    ITEM_TYPE t = (ITEM_TYPE)(qrand()%ITEM_MAX);
    int s = qrand()%4;
    InitItem(t,s);
}

//不同类型的方块的选择
void Item::InitItem(ITEM_TYPE t,int nShape)
{
    mPoints.clear();

    mType = t;
    mShape = nShape;
    switch (t)
    {
    case ITEM_1:
        {
            if (nShape%2 == 0)
            {
                for (int i = 0; i < 4; i++)
                {
                    mPoints.append(mPos + QPoint( i,2));
                }
            }
            else if (nShape%2 == 1)
            {
                for (int i = 0; i < 4; i++)
                {
                    mPoints.append(mPos + QPoint( 2,i));
                }
            }
            break;
        }
    case ITEM_2:
        {
            if (nShape == 0)
            {
                mPoints.append(mPos + QPoint( 1,0));
                mPoints.append(mPos + QPoint( 0,1));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
            }
            else if (nShape == 1)
            {
                mPoints.append(mPos + QPoint( 1,0));
                mPoints.append(mPos + QPoint( 1,2));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
            }
            else if (nShape == 2)
            {
                mPoints.append(mPos + QPoint( 0,1));
                mPoints.append(mPos + QPoint( 1,2));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
            }
            else if (nShape == 3)
            {
                mPoints.append(mPos + QPoint( 1,0));
                mPoints.append(mPos + QPoint( 0,1));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 1,2));
            }

            break;
        }
    case ITEM_3:
        {
            if (nShape == 0)
            {
                mPoints.append(mPos + QPoint( 1,0));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 1,2));
                mPoints.append(mPos + QPoint( 2,2));
            }
            else if (nShape == 1)
            {
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 3,1));
                mPoints.append(mPos + QPoint( 1,2));
            }
            else if (nShape == 2)
            {
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 2,2));
                mPoints.append(mPos + QPoint( 2,3));
            }
            else if (nShape == 3)
            {
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 2,2));
                mPoints.append(mPos + QPoint( 1,2));
                mPoints.append(mPos + QPoint( 0,2));
            }
            break;
        }
    case ITEM_4:
        {
            if (nShape == 0)
            {
                mPoints.append(mPos + QPoint( 2,0));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 2,2));
                mPoints.append(mPos + QPoint( 1,2));
            }
            else if (nShape == 1)
            {
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 1,2));
                mPoints.append(mPos + QPoint( 2,2));
                mPoints.append(mPos + QPoint( 3,2));
            }
            else if (nShape == 2)
            {
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 1,2));
                mPoints.append(mPos + QPoint( 1,3));
            }
            else if (nShape == 3)
            {
                mPoints.append(mPos + QPoint( 0,1));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 2,2));
            }
            break;
        }
    case ITEM_5:
        {
            mPoints.append(mPos + QPoint( 0,0));
            mPoints.append(mPos + QPoint( 0,1));
            mPoints.append(mPos + QPoint( 1,0));
            mPoints.append(mPos + QPoint( 1,1));
            break;
        }
    case ITEM_6:
        {
            if (nShape == 0)
            {
                mPoints.append(mPos + QPoint( 1,0));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 2,2));
            }
            else if (nShape == 1)
            {
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 0,2));
                mPoints.append(mPos + QPoint( 1,2));
            }
            else if (nShape == 2)
            {
                mPoints.append(mPos + QPoint( 2,0));
                mPoints.append(mPos + QPoint( 2,1));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 1,2));
            }
            else if (nShape == 3)
            {
                mPoints.append(mPos + QPoint( 0,1));
                mPoints.append(mPos + QPoint( 1,1));
                mPoints.append(mPos + QPoint( 1,2));
                mPoints.append(mPos + QPoint( 2,2));
            }
            break;
        }
    default:
        break;
    }
}
//选择 根据旋转次数决定
void Item::ChangeShape(int nAdd)
{
    mShape = (mShape + nAdd)%4;
    InitItem(mType,mShape);
}

void Item::Draw(QPainter& painter,int nStartX,int nStartY,int nW,int nH)
{
    for (int i = 0; i< mPoints.size(); i++)
    {
        QPoint pt = mPoints[i];
        painter.drawRect(QRect(nStartX + pt.x() * nW,nStartY + pt.y() * nH,nW,nH));
    }
}

void Item::AddPoints(QVector<QPoint>& points)
{
    for (int i = 0; i<points.size(); i++)
    {
        if (!mPoints.contains(points[i]))
        {
            mPoints.append(points[i]);
        }
    }
}

void Item::Move(int x,int y)
{
    for (int i = 0; i<mPoints.size(); i++)
    {
        int x1 = mPoints[i].x() + x;
        int y1 = mPoints[i].y() + y;
        mPoints[i].setX(x1);
        mPoints[i].setY(y1);
    }
    mPos += QPoint(x,y);
}

void Item::MoveTo(int x,int y)
{
    for (int i = 0; i<mPoints.size(); i++)
    {
        int x1 = mPoints[i].x() - mPos.x() + x;
        int y1 = mPoints[i].y() - mPos.y() + y;
        mPoints[i].setX(x1);
        mPoints[i].setY(y1);
    }
    mPos = QPoint(x,y);
}
//消除
void Item::DeleteRow( int y )
{
    QVector<QPoint> newPoints;
    for (int i=0; i<mPoints.size(); i++)
    {
        if (mPoints[i].y() != y)
        {
            newPoints.append(mPoints[i]);
        }
    }
    mPoints = newPoints;
}
//消除以后
void Item::MoveDown(int nRow,int y )
{
    for (int i = 0; i<mPoints.size(); i++)
    {
        if(mPoints[i].y() < nRow)
        {
            mPoints[i].setY(mPoints[i].y()+ y);
        }
    }
}

