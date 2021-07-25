#ifndef MAPPER_H
#define MAPPER_H

#include <QWidget>
#include <QPainter>
#include "include/rmap.h"

//根据雷达扫描的结果进行建图
class Mapper : public QWidget
{
    Q_OBJECT
public:
    static Mapper *getMapper(QWidget *parent = nullptr);

    void updateMapper(int cx, int cy, RMap *rMap);

protected:
    void paintEvent(QPaintEvent *event);

private:
    static Mapper *ptr;

    RMap *map = nullptr;

    //记录小车坐标
    int x = 0;
    int y = 0;

    explicit Mapper(QWidget *parent = nullptr);

    void draw(QPainter &painter);
};


#endif // MAPPER_H
