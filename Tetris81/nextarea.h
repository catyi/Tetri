#ifndef NEXTAREA_H
#define NEXTAREA_H

#include <QWidget>
#include "item.h"

namespace Ui {
class NextArea;
}

class NextArea : public QWidget
{
    Q_OBJECT

public:
    explicit NextArea(QWidget *parent = 0);
    ~NextArea();
protected:
    void paintEvent(QPaintEvent *);

public slots:
    void slotUpdateNextItem(ITEM_TYPE t,int nShape);


private:
    Item mItem;
private:
    Ui::NextArea *ui;
};

#endif // NEXTAREA_H
