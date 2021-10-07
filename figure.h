#ifndef FIGURE_H
#define FIGURE_H
#include<vector>

/*人物初始化共做5件事情
 * 1.初始化activated block
 * 2.将激活状态设置为false
 * 3.将激活方块数组初始化
 * 4.将绘图用点集合初始化
 * 5.将Point初始化为0
 */

struct Point{
    int x;
    int y;
    Point(int x1,int y1):x(x1),y(y1){
    }
};
class figure
{
public:
    figure();
    figure(int x,int y);
    ~figure();
    std::vector<Point> blocks;
    //人物坐标
    int Posx;
    int Posy;
    //人物获得分数
    int Score;
    //人物是否激活以及激活方块坐标
    bool dizzy_item;
    bool activated;
    int activated_block[2];

};

#endif // FIGURE_H
