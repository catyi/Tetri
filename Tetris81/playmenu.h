#ifndef PLAYMENU_H
#define PLAYMENU_H
#include <QWidget>
#include "playtotal.h"
#include "timegame.h"
namespace Ui {
class PlayMenu;
}

class PlayMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PlayMenu(QWidget *parent = 0);
    ~PlayMenu();
signals:
    void back1();
    void go();
    void go1();
    void go_user(QString);
    void go_user1(QString);


private slots:

    void on_e1_clicked();

    void on_b2_clicked();

    void on_st1_clicked();

    void on_pushButton_clicked();

    void recv(QString);

private:
    Ui::PlayMenu *ui;
    Playtotal *sta =NULL;
    TimeGame *tplay = NULL;
    QString user2;
};

#endif // PLAYMENU_H
