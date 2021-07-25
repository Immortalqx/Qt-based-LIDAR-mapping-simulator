#include <ctime>
#include <cstdlib>
#include <fstream>
#include "include/rmap.h"

RMap::RMap(int col, int row, bool random_map, int height, int width) :
        col(col),
        row(row),
        block_height(height),
        block_width(width)
{
    srand(time(nullptr));
    //动态生成二维数组
    map = new int *[col + 1];
    for (int i = 0; i < col + 1; i++)
        map[i] = new int[row + 1];

    //初始化并随机生成障碍物
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            if (random_map && rand() % 6 == 0 && !((i < 2 && j > col - 3) || (i > row - 3 && j < 2)))
                map[i][j] = 1;
            else
                map[i][j] = 0;
}

void RMap::addIN(int x, int y, bool block)
{
    //block为真表示起点和终点处不能够添加障碍物(起点和重点大小固定为2格*2格)
    if (block && (y < 2 && x > col - 3) || (y > row - 3 && x < 2))
        return;

    //检测是否越界
    if (y < row && x < col && y >= 0 && x >= 0)
        map[y][x] = 1;
}

void RMap::deleteOUT(int x, int y)
{
    //检测是否越界
    if (y < row && x < col && y >= 0 && x >= 0)
        map[y][x] = 0;
}

void RMap::clear()
{
    for (int i = 0; i <= row; i++)
        for (int j = 0; j <= col; j++)
            map[i][j] = 0;
}

bool RMap::query(int x, int y)
{
    if (x < 0 || y < 0 || x > col || y > row)
        return false;

    else if (map[x][y] == 1)
        return true;

    else
        return false;
}

int RMap::getCol() const
{
    return this->col;
}

int RMap::getRow() const
{
    return this->row;
}

int RMap::getBlockHeight() const
{
    return this->block_height;
}

int RMap::getBlockWidth() const
{
    return this->block_width;
}

int RMap::getMapHeight() const
{
    return block_height * row;
}

int RMap::getMapWidth() const
{
    return block_width * col;
}

bool RMap::saveRmap(std::string path)
{
    std::ofstream file;
    file.open(path);

    if (!file.is_open())
        return false;

    file << row << " " << col << std::endl;
    file << block_height << " " << block_width << std::endl;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
            file << map[i][j] << " ";
        file << std::endl;
    }
    file.close();
    return true;
}

bool RMap::loadRmap(std::string path)
{
    std::ifstream file;
    file.open(path);

    if (!file.is_open())
        return false;

    this->clear();

    file >> row >> col >> block_height >> block_width;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            file >> map[i][j];

    return true;
}
