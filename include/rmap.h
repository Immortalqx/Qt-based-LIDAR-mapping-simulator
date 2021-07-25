#ifndef RMAP_H
#define RMAP_H

#include <string>

//一个用于保存仿真环境地图的类
class RMap
{
public:
    explicit RMap(int col = 16, int row = 16, bool random_map = false, int height = 50, int width = 50);

    //增加到矩阵中
    void addIN(int x, int y, bool block = false);

    //从矩阵中删除
    void deleteOUT(int x, int y);

    //查询矩阵的某个点是否为1
    bool query(int x, int y);

    //清除所有数据
    void clear();

    //获取地图的行数
    int getRow() const;

    //获取地图的列数
    int getCol() const;

    //获取障碍物的长度
    int getBlockHeight() const;

    //获取障碍物的宽度
    int getBlockWidth() const;

    //获取地图的长度
    int getMapHeight() const;

    //获取地图的宽度
    int getMapWidth() const;

    bool saveRmap(std::string path);

    bool loadRmap(std::string path);

protected:
    //用矩阵保存场景中的障碍物
    int **map;
    int row;//行
    int col;//列

    //障碍物的长和宽
    int block_height;
    int block_width;
};


#endif // RMAP_H
