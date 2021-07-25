#include "include/viewer.h"
#include "cmath"
#include <QStyleOption>

Viewer *Viewer::ptr = nullptr;

Viewer *Viewer::getViewer(QWidget *parent)
{
    if (ptr == nullptr)
    {
        if (parent == nullptr)
            throw "ERROR:\tCreate Viewer by nullptr!";

        ptr = new Viewer(parent);
    }
    return ptr;
}

Viewer::Viewer(QWidget *parent) :
        QWidget(parent)
{
}

void Viewer::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    QWidget::paintEvent(event);

    // 反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(50, 50, 50), 1));
    // 设置填充的颜色
    painter.setBrush(QColor(0, 150, 200));

    //绘制三角区块
    QPainterPath path;
    path.moveTo(200, 200);
    path.lineTo(210, 210);
    path.lineTo(200, 180);
    path.lineTo(190, 210);
    painter.drawPath(path);

    this->draw(painter);
}

void Viewer::updateViewer(RMap *map)
{
    this->lidarMap = map;
    this->update();
}

void Viewer::draw(QPainter &painter)
{
    if (lidarMap == nullptr)
        return;

    painter.setBrush(QColor(255, 50, 50));

    for (int i = 0; i <= lidarMap->getRow(); i++)
    {
        for (int j = 0; j <= lidarMap->getCol(); j++)
        {
            if (lidarMap->query(i, j) == 1)
            {
                int x = i * 4;
                int y = j * 4;
                painter.drawRect(x, y, lidarMap->getBlockWidth(), lidarMap->getBlockHeight());
            }
        }
    }
}
