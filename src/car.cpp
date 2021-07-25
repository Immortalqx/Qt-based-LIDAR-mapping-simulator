#include <cmath>

#include "include/car.h"

const double PI = 3.1415926535;

Rectangle createRect(int x, int y, int height, int width, double angle)
{
    Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.height = height;
    rect.width = width;
    rect.angle = angle;
    return rect;
}

Car::Car(int x, int y, double angle, int height, int width)
{
    this->rect_c = createRect(x, y, height, width, angle);
    this->lidar = new Lidar();
}

//车子的移动逻辑，可以移动则返回true并且更新车的位置信息
bool Car::move_car(int key, RMap &rMap)
{
    int step = 10;//每次移动10个像素
    int rotate = 10;//每次转动10°
    Rectangle new_rect_c = rect_c;

    switch (key)
    {
        case 1://前进
            new_rect_c.x += floor(step * cos(new_rect_c.angle / 180.0 * PI));
            new_rect_c.y += floor(step * sin(new_rect_c.angle / 180.0 * PI));
            break;
        case 2://后退
            new_rect_c.x -= floor(step * cos(new_rect_c.angle / 180.0 * PI));
            new_rect_c.y -= floor(step * sin(new_rect_c.angle / 180.0 * PI));
            break;
        case 3://原地左转（逆时针旋转）
            new_rect_c.angle += rotate;
            break;
        case 4://原地右转（顺时针旋转）
            new_rect_c.angle -= rotate;
            break;
        default:
            break;
    }
    if (new_rect_c.angle < -180) new_rect_c.angle += 360;
    if (new_rect_c.angle > 180) new_rect_c.angle -= 360;

    if (check_collision(new_rect_c, rMap))
        return false;

    rect_c = new_rect_c;
    lidar->update_carInfo(this->X(), this->Y(), (this->Angle() + 90) * PI / 180);
    lidar->update_lidarMap(this->X() / 50, this->Y() / 50, rMap);
    lidar->buildMap();

    return true;
}

Lidar *Car::getLidar()
{
    return this->lidar;
}

//检测小车是否会与地图中的障碍物或者边界发生碰撞，发生碰撞则返回true
bool Car::check_collision(const Rectangle &new_rect_c, RMap &rMap)
{
/*=================检测小车是否会与边界发生碰撞=================*/

    //计算矩形中心点到矩形顶点的距离（M为x轴上投影距离，N为y轴上投影距离）
    //-2增加容错率，防止不能够贴着边界走（暂定，之后会修改）
    double M = fabs(new_rect_c.height * sin(new_rect_c.angle / 180.0 * PI) / 2) +
               fabs(new_rect_c.width * cos(new_rect_c.angle / 180.0 * PI) / 2) - 2;

    double N = fabs(new_rect_c.height * cos(new_rect_c.angle / 180.0 * PI) / 2) +
               fabs(new_rect_c.width * sin(new_rect_c.angle / 180.0 * PI) / 2) - 2;

    if (new_rect_c.x - M < 0 ||//检测最左边的点
        new_rect_c.y - N < 0 ||//检测最下面的点
        new_rect_c.x + M > rMap.getMapWidth() ||//检测最右边的点
        new_rect_c.y + N > rMap.getMapHeight())//检测最上面的点
        return true;

/*=================检测小车是否会与周围的障碍物发生碰撞=================*/
    //计算小车中心点在地图矩阵中的位置
    int col = new_rect_c.x / rMap.getBlockWidth();
    int row = new_rect_c.y / rMap.getBlockHeight();

    //碰撞检测(从小车中心点开始检测上下左右共九个方块)
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            //防止数组越界
            if (row + i < 0 || col + j < 0 ||
                row + i > rMap.getRow() || col + j > rMap.getCol()
                    )
                continue;
            //障碍物的矩形描述
            Rectangle rect = createRect(
                    (col + j) * rMap.getBlockWidth(),
                    (row + i) * rMap.getBlockHeight(),
                    rMap.getBlockHeight(),
                    rMap.getBlockWidth(),
                    0
            );
            if (rMap.query(col + j, row + i) && separatingAxis(new_rect_c, rect))
                return true;
        }
    }
    return false;
}

//应用分离轴定理检测两个矩形是否发生了碰撞
bool Car::separatingAxis(const Rectangle &rect_c, const Rectangle &rect_b)
{
/*=================小车在障碍物对称轴上投影=================*/
    //障碍物的左上角建立坐标系
    Rectangle rect_cbA = createRect(
            rect_c.x - rect_b.x,
            rect_c.y - rect_b.y,
            rect_c.height,
            rect_c.width,
            rect_c.angle
    );
    Rectangle rect_cbB = createRect(
            0,
            0,
            rect_b.height,
            rect_b.width,
            0
    );

/*=================障碍物在小车对称轴上投影=================*/
/*
    //小车一对称轴的直线方程：A1*x+B1*y+C1=0
    //设为小车自身坐标系的X轴
    double A1 = tan(rect_c.angle / 180.0 * PI + PI / 2);
    double B1 = -1;
    double C1 = rect_c.y - A1 * rect_c.x;

    //小车另一对称轴的直线方程：A2*x+B2*y+C2=0
    //设为小车自身坐标系的Y轴
    double A2 = tan(rect_c.angle / 180.0 * PI);
    double B2 = -1;
    double C2 = rect_c.y - A2 * rect_c.x;

    //将障碍物矩形的坐标修改成障碍物中心点坐标
    rect_b.x += rect_b.width / 2;
    rect_b.y -= rect_b.height / 2;

    //计算障碍物矩形中心点在小车自身坐标系下的坐标，旋转角度
    double d = (rect_b.x - rect_c.x) * (rect_b.x - rect_c.x) +
               (rect_b.y - rect_c.y) * (rect_b.y - rect_c.y);//到原点的距离的平方
    double r_y = fabs(A1 * rect_b.x + B1 * rect_b.y + C1) / sqrt(A1 * A1 + B1 * B1);//到x轴的距离
    double r_x = sqrt(d - r_y * r_y);//到y轴的距离

    if (A2 * rect_b.x + B2 * rect_b.y + C2 < 0)//在Y轴左边的点，X轴坐标为负
        r_x = -r_x;
    if (A1 * rect_b.x + B1 * rect_b.y + C1 < 0)//在X轴下面的点，Y轴坐标为负
        r_y = -r_y;

    Rectangle rect_bcA = createRect(
            int(r_x),
            int(r_y),
            rect_b.height,
            rect_b.width,
            180.0 - rect_c.angle
    );
    Rectangle rect_bcB = createRect(
            0,
            0,
            rect_c.height,
            rect_c.width,
            0
    );
*/
/*=================分离轴定理判断是否发生碰撞=================*/
    //只要有某个方向上的投影不相交，则不发生碰撞
    if (rect_intersect(rect_cbA, rect_cbB) /*&& rect_intersect(rect_bcA, rect_bcB)*/)
        return true;

    return false;
}

//判断矩形A是否在矩形B对称轴的投影下B相交，相交则返回true
//第一个矩形的坐标是矩形中心点的坐标，并且是相对于第二个矩形坐标系(以左上角顶点为原点)的坐标
bool Car::rect_intersect(const Rectangle &rect_A, const Rectangle &rect_B)
{
    //计算矩形中心点到矩形顶点的距离（M为x轴上投影距离，N为y轴上投影距离）
    double M = fabs(rect_A.height * sin(rect_A.angle / 180.0 * PI) / 2) +
               fabs(rect_A.width * cos(rect_A.angle / 180.0 * PI) / 2) - 5;

    double N = fabs(rect_A.height * cos(rect_A.angle / 180.0 * PI) / 2) +
               fabs(rect_A.width * sin(rect_A.angle / 180.0 * PI) / 2) - 5;

    //在对称轴（x轴）上投影
    if (rect_A.x - M > rect_B.width || rect_A.x + M < 0)
        return false;
    //在对称轴（y轴）上投影
    if (rect_A.y - N > rect_B.height || rect_A.y + N < 0)
        return false;

    return true;
}

int Car::X() const
{
    return this->rect_c.x;
}

int Car::Y() const
{
    return this->rect_c.y;
}

double Car::Angle() const
{
    return this->rect_c.angle;
}

int Car::Height() const
{
    return this->rect_c.height;
}

int Car::Width() const
{
    return this->rect_c.width;
}
