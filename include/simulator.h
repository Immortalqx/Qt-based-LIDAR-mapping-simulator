#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QKeyEvent>

#include "include/car.h"
#include "include/rmap.h"


//仿真环境
class Simulator : public QWidget
{
    Q_OBJECT
public:
    static Simulator *getSimulator(QWidget *parent = nullptr);

    void clearMap();

    void creatNewMap();

    bool saveMap(std::string filepath);

    bool loadMap(std::string filepath);

    void reBuildMap();

    void setMouseClick();

protected:
    void paintEvent(QPaintEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

private:
    QLabel *carLabel;//显示小车的控件

    QImage carImage;

    Car *car;

    RMap *map;

    bool click;

    void drawMap(QPainter &painter);//绘制Map

    //TODO 使用GUI程序画小车
    //void drawCar(QPainter &painter);

    explicit Simulator(QWidget *parent = nullptr);

    static Simulator *ptr;
};


#endif // SIMULATOR_H
