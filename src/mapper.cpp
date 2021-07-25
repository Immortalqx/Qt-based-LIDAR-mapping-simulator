#include "include/mapper.h"
#include "cmath"
#include <QStyleOption>

Mapper *Mapper::ptr = nullptr;

Mapper *Mapper::getMapper(QWidget *parent)
{
    if (ptr == nullptr)
    {
        if (parent == nullptr)
            throw "ERROR:\tCreate Mapper by nullptr!";

        ptr = new Mapper(parent);
    }
    return ptr;
}

void Mapper::updateMapper(int cx, int cy, RMap *rMap)
{
    this->x = cx / 2;
    this->y = cy / 2;
    this->map = rMap;
    this->update();
}

Mapper::Mapper(QWidget *parent) :
        QWidget(parent)
{
}

void Mapper::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);

    this->draw(painter);
}

void Mapper::draw(QPainter &painter)
{
    //反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    //设置画笔颜色、宽度
    painter.setPen(QPen(QColor(50, 50, 50), 1));
    //设置填充的颜色
    painter.setBrush(QColor(255, 0, 200));

    painter.drawRect(x, y, 5, 5);

    //绘制三角区块(这里将跟踪小车的位置以及角度！)
    //QPainterPath path;
    //path.moveTo(200, 200);
    //path.lineTo(210, 210);
    //path.lineTo(200, 180);
    //path.lineTo(190, 210);
    //painter.drawPath(path);

    if (map == nullptr)
        return;

    painter.setBrush(QColor(0, 255, 255));
    for (int i = 0; i <= 200; i++)
    {
        for (int j = 0; j <= 200; j++)
        {
            if (map->query(i, j) == 1)
                painter.drawRect(i * 2, j * 2, 3, 3);
        }
    }
}
