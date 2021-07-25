#ifndef CAR_H
#define CAR_H

#include "include/rmap.h"
#include "include/lidar.h"

typedef struct Rectangle
{
    int x, y;//默认为矩形左上角顶点的坐标
    int height;//矩形长
    int width;//矩形宽
    double angle;//矩形角度，采用角度制，定义沿x轴正方向时为0，且顺时针增加。
} Rectangle;

//实现车子的逻辑代码（碰撞检测、移动转向等）
class Car
{
public:
    Car(int x = 0, int y = 0, double angle = -90, int height = 50, int width = 50);

    bool move_car(int key, RMap &rMap);//车子的移动

    Lidar *getLidar();

    int X() const;

    int Y() const;

    double Angle() const;

    int Height() const;

    int Width() const;

private:
    //使用矩形来描述小车的位置信息
    //注意：为了减少小车旋转时的计算量，该矩形变量的坐标为小车的中心点坐标！！！
    Rectangle rect_c{};

    //小车的雷达
    Lidar *lidar;

    //传参数可以用引用，这样会提高性能，例如 Rectangle &new_rect_c
    static bool check_collision(const Rectangle &new_rect_c, RMap &rMap);//小车的碰撞检测

    static bool separatingAxis(const Rectangle &rect_c, const Rectangle &rect_b);//分离轴定理

    static bool rect_intersect(const Rectangle &rect_A, const Rectangle &rect_B);//判断两个矩形是否相交
};


#endif // CAR_H
