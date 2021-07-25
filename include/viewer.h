#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QPainter>

#include "include/rmap.h"

//仿真雷达环境类
//为方便重绘操作而使用单例模式实现
class Viewer : public QWidget
{
    Q_OBJECT
public:
    static Viewer *getViewer(QWidget *parent = nullptr);

    void updateViewer(RMap *map);

protected:
    void paintEvent(QPaintEvent *event);

private:
    static Viewer *ptr;

    RMap *lidarMap = nullptr;

    explicit Viewer(QWidget *parent = nullptr);

    void draw(QPainter &painter);
};


#endif // VIEWER_H
