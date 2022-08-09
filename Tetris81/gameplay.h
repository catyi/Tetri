#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include <QObject>
#include "Item.h"
#include <QWidget>
#include <QTime>
#include<QString>
namespace Ui {
class GamePlay;
}

class GamePlay : public QWidget
{
    Q_OBJECT
private slots:


public:
    explicit GamePlay(QWidget *parent = 0);
    ~GamePlay();
    void DrawBKRects();         //用作背景的方块
    void DrawFixedRects();      //下落后已固定不动的方块
    void DrawCurItem();         //当前下落中的方块

    void NewGame();
    void KeyPressed(int key); //按键


    bool HitSide();             //判断当前下落方块是否超左右边界
    bool HitBottom();           //判断当前下落方块是否达到底部
    bool HitTop();              //判断当前下落方块是否达到顶部
    void AddToFixedRects();     //把当前方块加入到 固定方块
    void DeleteFullRows();      //删除完整的行

    int GetLevelTime(int level);   //获取不同等级关卡对应的定时器时间，关卡越高，时间越快(短)。比如1关=1s,2关=900ms,3关=800ms
    void pause(bool status); //暂停处理
    void stopgame();
signals:
    void sigUpdateNextItem(ITEM_TYPE t,int nShape);
    void sigUpdateScore(int nScore);
    void sigUpdateLevel(int nSpeed);
    void gameover(int nScore);//发送到主窗口 更新成绩

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent*);

private:
    Item mFixItems;     //已落下、固定住了的方块们
    Item mCurItem;      //当前移动中的方块
    Item mNextItem;     //下一个方块

    int mTimerID;       //定时器ID
    int mScore;         //得分
    int mLevel;         //关卡
    QTime playtime;     //游戏时间


private:
    Ui::GamePlay *ui;
};

#endif // GAMEPLAY_H
