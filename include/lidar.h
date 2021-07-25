#ifndef LIDAR_H
#define LIDAR_H

#include "include/rmap.h"

//单例模式实现一个用于传输并且处理通用数据（主要是坐标、角度、地图）的类
class Lidar
{
public:
    Lidar();

    //更新雷达扫描到的地图（模拟雷达扫描）
    void update_lidarMap(int cx, int cy, RMap &map);

    //TODO 更新建立的地图（模拟建图）
    void buildMap();

    //重新建图（清空）
    void reBuildMap();

    //更新小车的角度
    void update_carInfo(int x, int y, double angle);

    RMap *getLidarMap();

    RMap *getLidarMapTurn();

    RMap *getMap();

    int getCar_x();

    int getCar_y();

    double getCarAngle();

private:
    //清空雷达扫描的数据（防止扫描结果错误）
    void clearMap();

    //重新建图（清空已经建好的图）
    void reMap();

    RMap *originMap = nullptr; //保存雷达附近的障碍物地图

    RMap *lidarMap = nullptr; //保存雷达扫描得到的结果

    RMap *bMap = nullptr; //保存建立的地图

    //保存车的信息
    int car_x = 0;
    int car_y = 0;
    double car_angle = 0;

    int last_x = 0;
    int last_y = 0;
};


#endif // LIDAR_H
