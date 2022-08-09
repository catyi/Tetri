#ifndef PLAYTOTAL_H
#define PLAYTOTAL_H
#include <QObject>
#include <QMainWindow>
#include <QKeyEvent>
#include <QString>

namespace Ui {
class Playtotal;
}

class Playtotal : public QMainWindow
{
    Q_OBJECT
signals:
    void back2();
    void time1(bool);

public:
    explicit Playtotal(QWidget *parent = 0);
    ~Playtotal();


protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
     void on_pushButton_3_clicked();
    void slotUpdateScore(int nScore);
    void slotUpdateLevel(int nSpeed);

    void on_pushButton_clicked();

    void on_radioButton_clicked(bool checked);
    void on_pushButton_2_clicked();
    void recv2(QString);//接收用户名字
    void upgrade(int nScore);//更新成绩

private:
    Ui::Playtotal *ui;
    bool ti;
    QString user4;
    int mScore;
};

#endif // PLAYTOTAL_H
