#ifndef MUNU_H
#define MUNU_H

#include <QWidget>
#include "playmenu.h"
#include "ui_playmenu.h"
#include <QString>
#include <QSqlDatabase>

namespace Ui {
class Munu;
}

class Munu : public QWidget
{
    Q_OBJECT

public:
    explicit Munu(QWidget *parent = 0);
    ~Munu();
signals:
    void legin_play();
    void legin_user(QString);

private slots:
    void on_message_clicked();

    void on_mode_clicked();

    void on_bt1_clicked();

    void receiveData(QString);



private:
    Ui::Munu *ui;
    PlayMenu *playm = NULL;
    QSqlDatabase mydb1;
    QString user1;
};

#endif // MUNU_H
