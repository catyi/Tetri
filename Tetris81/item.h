#ifndef ITEM_H
#define ITEM_H
#pragma once
#include "qvector.h"
#include "qpoint.h"
#include "qmap.h"
#include "qpainter.h"

enum ITEM_TYPE{
    ITEM_1 = 0,		//长条
    ITEM_2,			//山字形
    ITEM_3,			//手枪形1
    ITEM_4,			//手枪形2
    ITEM_5,			//田
    ITEM_6,			//Z字形1
    ITEM_MAX,
};

class Item
{
public:
    Item(){}
    Item(ITEM_TYPE t,int nShape = 0);
    ~Item(void);

    void InitNew(int nSeed = 0);
    void InitItem(ITEM_TYPE t,int nShape = 0);
    void ChangeShape(int nAdd = 1);

    void AddPoints(QVector<QPoint>& points);
    void Move(int x,int y);             //横向移动x格,竖向移动y格
    void MoveTo(int x,int y);           //移动到位置(x,y)格
    void MoveDown(int nRow,int y);		//第nRow行以上的部分下移y个单位，用在消除之后

    void Draw(QPainter& painter,int nStartX,int nStartY,int nW,int nH);
    void DeleteRow(int y);
public:
    QVector<QPoint> mPoints;    //方块元素内含有的点数，每个点代表一个格子坐标，每个方块Item含有4个坐标点，就是4个格子
    QPoint mPos;
    ITEM_TYPE mType;    //方块有6种类型
    int mShape;         //但是每个类型又可以变形（改变方向）,每个大类下面又可能有1~4个方向的不同形状
};

#endif // ITEM_H
