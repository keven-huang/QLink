#include "figure.h"

/*人物初始化共做5件事情
 * 1.初始化activated block
 * 2.将激活状态设置为false
 * 3.将激活方块数组初始化
 * 4.将绘图用点集合初始化
 * 5.将Point初始化为0
 */
figure::figure()
{
    blocks.clear();
    Point = 0;
    activated = false;
    for(int i = 0;i<2;i++)
        activated_block[i] = -1;
}

figure::figure(int x, int y):Posx(x),Posy(y),Point(0)
{
    blocks.clear();
    activated = false;
    for(int i = 0;i<2;i++)
        activated_block[i] = -1;
}
figure::~figure(){
}
