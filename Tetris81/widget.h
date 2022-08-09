#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include "munu.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
signals:
    void login_successed(QString);
    void login_successed1();
private slots:
    void on_regist_clicked();

    void on_login_clicked();

    void on_e0_clicked();

private:
    Ui::Widget *ui;
    QSqlDatabase mydb;
    Munu *me =NULL;
};

#endif // WIDGET_H
