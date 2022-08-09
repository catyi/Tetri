#ifndef TIMEGAME_H
#define TIMEGAME_H

#include "Item.h"
#include <QWidget>
#include <QTime>
#include <QTimer>
#include <QString>
#include <QSqlDatabase>

namespace Ui {
class TimeGame;
}

class TimeGame : public QWidget
{
    Q_OBJECT
signals:
    void back3();

    public:
    explicit TimeGame(QWidget *parent = 0);
    ~TimeGame();
    void keyPressEvent(QKeyEvent *e); //键盘响应
    void DrawBKRects();         //用作背景的方块
    void DrawFixedRects();      //下落后已固定不动的方块
    void DrawCurItem();         //当前下落中的方块

    void NewGame();



    bool HitSide();             //判断当前下落方块是否超左右边界
    bool HitBottom();           //判断当前下落方块是否达到底部
    bool HitTop();              //判断当前下落方块是否达到顶部
    void AddToFixedRects();     //把当前方块加入到 固定方块
    void DeleteFullRows();      //删除完整的行

    int GetLevelTime(int level);   //获取不同等级关卡对应的定时器时间，关卡越高，时间越快(短)。比如1关=1s,2关=900ms,3关=800ms
    void stopgame();

private slots:

    void on_pushButton_2_clicked();

    void timeover();

    void on_pushButton_3_clicked();
    void recv1(QString);//接收用户名字
    void upgrade();//更新成绩

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent*);

private:
    Item mFixItems;     //已落下、固定住了的方块们
    Item mCurItem;      //当前移动中的方块
    Item mNextItem;     //下一个方块

    int mTimerID;       //定时器ID
    int mScore;         //得分
    int mLevel;         //关
    QTime playtime;     //游戏时间
    QTimer *timer1; //计时器
    int sc=60*3;//秒数
     QString user3;      //用户名
private:
    Ui::TimeGame *ui;
};

#endif // TIMEGAME_H
