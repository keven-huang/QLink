#ifndef FIGURE_H
#define FIGURE_H
#include<vector>

struct point{
    int x;
    int y;
    point(int x1,int y1):x(x1),y(y1){
    }
};
class figure
{
public:
    figure();
    figure(int x,int y);
    ~figure();
    std::vector<point> blocks;
    //人物坐标
    int Posx;
    int Posy;
    //人物获得分数
    int Point;
    //人物是否激活以及激活方块坐标
    bool activated;
    int activated_block[2];

};

#endif // FIGURE_H
