#include "nextarea.h"
#include "ui_nextarea.h"

NextArea::NextArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NextArea)
{
    ui->setupUi(this);
}

NextArea::~NextArea()
{
    delete ui;
}
//绘制窗口2 下一个方块
void NextArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QColor("#FFDEAD"));
    painter.setPen(QPen(QColor(Qt::black),1));

    int xStart = 5;
    int yStart = 5;
    int w = 20;
    int h = 20;
    foreach (QPoint pt, mItem.mPoints)
    {
        int x = xStart + pt.x() * w;
        int y = yStart + pt.y() * h;
        painter.drawRect(x, y, w, h);
    }

    update();
}

void NextArea::slotUpdateNextItem(ITEM_TYPE t, int nShape)
{
    mItem.InitItem(t,nShape);
}
