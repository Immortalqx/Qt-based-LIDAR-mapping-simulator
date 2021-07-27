#include "include/lidar.h"
#include <cmath>

const double PI = 3.1415926535;

Lidar::Lidar()
{
    originMap = new RMap(9, 9, false, 40, 40);

    lidarMap = new RMap(100, 100, false, 4, 4);

    bMap = new RMap(200, 200, false, 2, 2);

    this->clearMap();
    this->reMap();
}

//根据车子的坐标（x，y）和障碍物的地图 map 更新雷达看到的障碍物
void Lidar::update_lidarMap(int cx, int cy, RMap &map)
{
    this->clearMap();

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            int col = cx - 4 + i;
            int row = cy - 4 + j;
            if (col < 0 || col > 15)
                originMap->addIN(i, j);
            else if (row < 0 || row > 15)
                originMap->addIN(i, j);
            else if (map.query(col, row))
                originMap->addIN(i, j);
        }
    }

    double step = 0.015;//步长，确定雷达扫描的精度
    int x_0 = 200;//中心点
    int y_0 = 200;//中心点

    for (double angle = 0.00; angle < 2 * PI; angle += step)
    {
        int x = 0;
        int y = 0;
        int m, n;

        for (int i = 0; i < 284; i += 1)
        {
            x = x_0 + i * cos(angle) + 8;
            y = y_0 + i * sin(angle) + 8;

            if (x > 400 || y > 400 || x < 0 || y < 0)
                break;

            if (originMap->query(x / 50, y / 50))
            {
                //这里的调整与小车的长和宽有关
                m = (x - car_y + cy * 50 + 20) / 4;
                n = (y - car_x + cx * 50 + 20) / 4;

                this->lidarMap->addIN(m, n);
                break;
            }
        }
    }
}

void Lidar::buildMap()
{
    //没有移动或者移动很小（比如原地旋转导致的移动）则不更新地图
    if ((car_x + 3) / 50 == (last_x + 1) / 50 && (car_y + 3) / 50 == (last_y + 1) / 50)
        return;

    //把雷达扫描的障碍物更新到地图中，
    int fix = 0;
    for (int i = 0; i < lidarMap->getCol(); i = i + 1)
    {
        for (int j = 0; j < lidarMap->getRow(); j = j + 1)
        {
            //模拟界面中的障碍物坐标
            int x = car_x + 4 * (i - 50);
            int y = car_y + 4 * (j - 50);
            //转化为保存的地图中的坐标
            x = x / 4.0 - 4.8;
            y = y / 4.0 - 5.0;
            if (x >= 0 && x < bMap->getCol() && y >= 0 && y < bMap->getRow())
            {
                if (lidarMap->query(i, j))
                    bMap->addIN(y, x);
                else if (fabs(i - lidarMap->getCol() / 2) < fix && fabs(j - lidarMap->getRow() / 2) < fix)
                    bMap->deleteOUT(y, x);
            }
        }
    }
}

void Lidar::reBuildMap()
{
    this->reMap();
}

void Lidar::update_carInfo(int x, int y, double angle)
{
    last_x = car_x;
    last_y = car_y;
    this->car_x = x;
    this->car_y = y;
    this->car_angle = angle;
}

void Lidar::clearMap()
{
    originMap->clear();
    lidarMap->clear();
}

void Lidar::reMap()
{
    bMap->clear();
}

RMap *Lidar::getLidarMap()
{
    return this->lidarMap;
}

RMap *Lidar::getLidarMapTurn()
{
    RMap *Tmap = new RMap(100, 100, false, 4, 4);
    Tmap->clear();

    int x_0 = 200;//中心点
    int y_0 = 200;//中心点

    for (int i = 0; i <= lidarMap->getRow(); i++)
    {
        for (int j = 0; j <= lidarMap->getCol(); j++)
        {
            if (lidarMap->query(i, j) == 1)
            {
                double xm = i * 4 - 22;
                double ym = j * 4 - 20;
                int x = x_0 + (xm - x_0) * cos(car_angle) + (ym - y_0) * sin(car_angle);
                int y = y_0 + (ym - y_0) * cos(car_angle) - (xm - x_0) * sin(car_angle);
                Tmap->addIN(y / 4, x / 4);
            }
        }
    }
    return Tmap;
}

RMap *Lidar::getMap()
{
    return this->bMap;
}

int Lidar::getCar_x()
{
    return this->car_x;
}

int Lidar::getCar_y()
{
    return this->car_y;
}

double Lidar::getCarAngle()
{
    return this->car_angle;
}
