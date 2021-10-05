#ifndef QLINK_H
#define QLINK_H
#include"figure.h"
#include<vector>


const int MapRow = 6;
const int MapCol = 6;
const int IconSize = 50;
const int IconNum = 2;
const int alpha = 0;
/* 常量解释
 * Map_Row 生成地图行数
 * Map_Col 生成地图列数
 * Icon_Size图片大小
 * Icon_num 图片种类数量（最多16种）
 * alpha 地图填充因子（Ex. alpha=1则箱子占满地图）
 *
 * 游戏模块类
 * 初始化共做5件事情
 * 1.初始化activated block
 * 2.将激活状态设置为false
 * 3.将激活方块数组初始化
 * 4.将绘图用点集合初始化
 * 5.将Point初始化为0
 */

class QLink
{
public:
    QLink();
    QLink(int mode);
    ~QLink();
    void startGame();
    void startGame(int mode);
    //游戏内部功能
    std::vector<Point> solution;
    std::vector<Point> CanWalk;
    int* Get_gamemap();
    bool fig_TwoCanLink(int srcX,int srcY,int dstX,int dstY,figure &fig);
    bool isWin();
    bool StillCanLink();
    bool fig_bfs();
    //item
    void shuffle();
    bool flash_item;
    bool Time_item;
    bool Shuffle_item;
    bool dizzy_item;
    //操控角色1，2
    figure fig1;
    figure fig2;
    void fig_W(figure &fig);
    void fig_A(figure &fig);
    void fig_S(figure &fig);
    void fig_D(figure &fig);
    //游戏外
    void save();
private:
    int *gamemap;
    bool One_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    bool Two_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    bool Three_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    friend class LinkTest;
};

#endif // QLINK_H
