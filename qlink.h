#ifndef QLINK_H
#define QLINK_H
#include"figure.h"
#include<vector>
#include<queue>

/* 常量解释
 * Map_Row 生成地图行数
 * Map_Col 生成地图列数
 * Icon_Size图片大小
 * Icon_num 图片种类数量（最多16种）
 * alpha 地图填充因子（Ex. alpha=1则箱子占满地图）
 */

const int MapRow = 4;
const int MapCol = 4;
const int IconSize = 60;
const int IconNum = 6;
const double alpha = 0.8;


/* QLink模块
 *QLink模块实现了地图数组以及人物模型的移动
 * fig_W,fig_A,fig_S,fig_D分别是上下左右时人物模型的操作
 * fig_TwoCanLink接收参数和人物判断是否能够连接，若能连接，将点放入blocks中
 */
class QLink
{
public:
    QLink();
    QLink(int mode);
    ~QLink();
    void initQLink();
    void initQLink(int mode);
    //游戏内部功能
    std::vector<Point> solution;
    int mode;
    int* GetMap();
    bool fig_TwoCanLink(int srcX,int srcY,int dstX,int dstY,figure &fig);
    bool isWin();
    bool StillCanLink();
    //道具
    void shuffle();
    int* hint_block;
    bool hint_item;
    bool flash_item;
    bool Time_item;
    bool Shuffle_item;
    //操控角色1，2
    figure fig1;
    figure fig2;
    figure fig_tmp;
    void fig_W(figure &fig);
    void fig_A(figure &fig);
    void fig_S(figure &fig);
    void fig_D(figure &fig);
private:
    int *gamemap;
    int *visited;
    bool One_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    bool Two_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    bool Three_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    friend class LinkTest;
};

#endif // QLINK_H
