#include "qlink.h"
#include<ctime>
#include<cstdlib>
#include <algorithm>
#include<QDebug>
#include<QFile>
QLink::QLink():fig1(0,0),fig2(0,0)
{
    initQLink(1);
}

QLink::QLink(int m):fig1(0,0),fig2(0,0)
{
    mode = m;
    initQLink(mode);
}

QLink::~QLink()
{
    delete[] gamemap ;
}

//initQLink()
//初始化游戏地图以及道具
void QLink::initQLink()
{
    gamemap = new int [MapCol*MapRow];
    for(int i = 0;i<MapCol*MapRow;i++)
        gamemap[i] = 0;
    int iconId = 0;
    const int BlockNum = alpha*MapRow*MapRow;
    const int k = BlockNum/(IconNum*2);
    for(int i = 0;iconId<IconNum;){
        for(int j = 0 ;j<k;j++){
            gamemap[i] = iconId%IconNum + 1;
            gamemap[i+1] = iconId%IconNum + 1;
            i+=2;
        }
        iconId++;
    }
    iconId = 0;
    int remain = BlockNum%(IconNum*2);
    for(int i = k*IconNum*2;i<MapCol*MapRow-1&&iconId<remain/2;){
        gamemap[i] = iconId%IconNum + 1;
        gamemap[i+1] = iconId%IconNum + 1;
        iconId+=1;
        i+=2;
    }
    srand((unsigned)time(0));
    std::random_shuffle(gamemap,gamemap+MapCol*MapRow);
    visited = new int [(MapCol+2)*(MapRow+2)];
    for(int i = 0;i<(MapCol+2)*(MapRow+2);i++)
        visited[i] = 0;
    hint_block = new int[4];
    for(int i = 0;i<4;i++)
        hint_block[i] = -1;
    flash_item = Time_item = Shuffle_item = hint_item =false;
}

//mode 模式 : 1 单人模式 2 双人模式
//调用initQLink并初始化人物位置
void QLink::initQLink(int m)
{
    mode = m;
    initQLink();
    if(mode == 1){
        //单人模式在右下角放置模型
        fig1.Posx = MapRow;
        fig1.Posy = MapCol;
        return;
    }
    else if(mode == 2){
        //双人模式分别在左上角和右下角放置模型
        fig1.Posx = -1;
        fig1.Posy = -1;
        fig2.Posx = MapRow;
        fig2.Posy = MapCol;
        return;
    }

}

int *QLink::GetMap()
{
    return gamemap;
}

//fig_TwoCanLink函数要求src的x,y坐标以及dst的x,y坐标和人物
//调用private函数来判断两箱子是否能够相连
//若能相连，会把Point放入该人物的blocks中来方便画线
//返回能否相连
bool QLink::fig_TwoCanLink(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    fig.blocks.clear();
    if(One_Line(srcX,srcY,dstX,dstY,fig)){
        return true;
    }
    fig.blocks.clear();
    if(Two_Line(srcX,srcY,dstX,dstY,fig)){
        return true;
    }
    fig.blocks.clear();
    if(Three_Line(srcX,srcY,dstX,dstY,fig)){
        return true;
    }
    fig.blocks.clear();
    return false;
}
//判断地图中箱子是否完全消去
//返回胜利或不胜利
bool QLink::isWin()
{
    for(int i = 0;i<MapCol*MapRow;i++){
        if(gamemap[i] >= 1&&gamemap[i] <= IconNum)
            return false;
    }
    return true;
}
//判断当前地图中是否还有解
bool QLink::StillCanLink()
{
    solution.clear();
    for(int i = 0;i<(MapCol+2)*(MapRow+2);i++)
        if(visited[i] != 0)
            visited[i] = 0;
    std::queue<Point> q;
    Point p1(fig1.Posx,fig1.Posy);
    visited[(fig1.Posx+1)*(MapCol+2)+fig1.Posy+1] = 1;
    q.push(p1);
    while (!q.empty()) {
        Point p = q.front();
        q.pop();
        //上
        if(p.x !=-1){
            if(!visited[(p.x)*(MapCol+2)+p.y+1]){
                if(p.y == -1||p.y == MapCol||p.x == 0)
                    q.push(Point(p.x-1,p.y));
                else {
                    if(gamemap[(p.x-1)*MapCol+p.y] >= 1&&gamemap[(p.x-1)*MapCol+p.y] <= IconSize)
                        solution.push_back(Point(p.x-1,p.y));
                    else
                        q.push(Point(p.x-1,p.y));
                }
                visited[(p.x)*(MapCol+2)+p.y+1] = 1;
            }
        }
        //下
        if(p.x != MapRow){
            if(!visited[(p.x+2)*(MapCol+2)+p.y+1]){
                if(p.y == -1||p.y == MapCol||p.x == MapRow-1)
                    q.push(Point(p.x+1,p.y));
                else {
                    if(gamemap[(p.x+1)*MapCol+p.y] >= 1&&gamemap[(p.x+1)*MapCol+p.y] <= IconSize)
                        solution.push_back(Point(p.x+1,p.y));
                    else
                        q.push(Point(p.x+1,p.y));
                }
                visited[(p.x+2)*(MapCol+2)+p.y+1] = 1;
            }
        }
        //左
        if(p.y != -1){
            if(!visited[(p.x+1)*(MapCol+2)+p.y]){
                if(p.x == -1||p.x == MapRow||p.y == 0)
                    q.push(Point(p.x,p.y-1));
                else {
                    if(gamemap[(p.x)*MapCol+p.y-1] >= 1&&gamemap[(p.x)*MapCol+p.y-1] <= IconSize)
                        solution.push_back(Point(p.x,p.y-1));
                    else
                        q.push(Point(p.x,p.y-1));
                }
                visited[(p.x+1)*(MapCol+2)+p.y] = 1;
            }
        }
        //右
        if(p.y != MapCol){
            if(!visited[(p.x+1)*(MapCol+2)+p.y+2]){
                if(p.x == -1||p.x == MapRow||p.y == MapCol-1)
                    q.push(Point(p.x,p.y+1));
                else {
                    if(gamemap[(p.x)*MapCol+p.y+1] >= 1&&gamemap[(p.x)*MapCol+p.y+1] <= IconSize)
                        solution.push_back(Point(p.x,p.y+1));
                    else
                        q.push(Point(p.x,p.y+1));
                }
                visited[(p.x+1)*(MapCol+2)+p.y+2] = 1;
            }
        }
    }
    //遍历solution中箱子查看是否有解
    for(int i = 0;i<int(solution.size());i++){
        for(int j = i+1;j<int(solution.size());j++)
            if((solution[i].x != solution[j].x||solution[i].y != solution[j].y)
                    &&gamemap[solution[i].x*MapCol+solution[i].y] == gamemap[solution[j].x*MapCol+solution[j].y]
            &&fig_TwoCanLink(solution[i].x,solution[i].y,solution[j].x,solution[j].y,fig_tmp)){
                fig_tmp.blocks.clear();
                hint_block[0] = solution[i].x;
                hint_block[1] = solution[i].y;
                hint_block[2] = solution[j].x;
                hint_block[3] = solution[j].y;
                qDebug()<<solution[i].x<<solution[i].y<<gamemap[solution[i].x*MapCol+solution[i].y];
                qDebug()<<solution[j].x<<solution[j].y<<gamemap[solution[j].x*MapCol+solution[j].y];
                return true;
            }
    }
    return false;
}
//使用方式 shuffle()
//随机打乱地图中所有箱子以及道具的顺序
void QLink::shuffle()
{
    std::random_shuffle(gamemap,gamemap+MapCol*MapRow);
    if(fig1.Posx>=0&&fig1.Posx<=MapRow-1&&fig1.Posy>=0&&fig1.Posy<=MapCol-1
            &&gamemap[fig1.Posx*MapCol+fig1.Posy] != 0){
        int i;
        for(i = 0;i<MapCol*MapRow;i++)
            if(gamemap[i] == 0)
                break;
        std::swap(gamemap[i],gamemap[fig1.Posx*MapCol+fig1.Posy]);
    }
    if(mode==2&&fig2.Posx>=0&&fig2.Posx<=MapRow-1&&fig2.Posy>=0&&fig2.Posy<=MapCol-1
            &&gamemap[fig2.Posx*MapCol+fig2.Posy] != 0){
        int i;
        for(i = 0;i<MapCol*MapRow;i++)
            if(gamemap[i] == 0&&i != fig1.Posx*MapCol+fig1.Posy)
                break;
        std::swap(gamemap[i],gamemap[fig2.Posx*MapCol+fig2.Posy]);
    }
}

void QLink::fig_W(figure &fig)
{
    if(fig.Posx ==-1){
        fig.Posx = fig.Posx;
        return;
    }
    else if(fig.Posx == 0||fig.Posy == -1||fig.Posy == MapCol){
        fig.Posx = fig.Posx - 1;
        return;
    }
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]==0){
        fig.Posx = fig.Posx - 1;
        return;
    }
    //W有格子
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]>=1&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]<=IconNum){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy;
       if(fig.activated_block[0] == -1||fig.activated == false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx - 1;
           fig.activated_block[1] = fig.Posy;
           return;
       }
       //已有激活
       else{
           if((fig.activated_block[0] != fig.Posx-1||fig.activated_block[1] != fig.Posy)
                   &&gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] == gamemap[(fig.Posx-1)*MapCol+fig.Posy]){
               if(fig_TwoCanLink(fig.activated_block[0],fig.activated_block[1],fig.Posx - 1,fig.Posy,fig)){
                   if(gamemap[(fig.Posx-1)*MapCol+fig.Posy] >= 1&&gamemap[(fig.Posx-1)*MapCol+fig.Posy] <= 9)
                       fig.Score += 30;
                   else if(gamemap[(fig.Posx-1)*MapCol+fig.Posy] >= 10&&gamemap[(fig.Posx-1)*MapCol+fig.Posy] <= 14){
                       fig.Score += 60;
                   }
                   else
                       fig.Score += 100;
                   gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
                   gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] = 0;
                   fig.activated_block[0] = -1;
                   fig.activated_block[1] = -1;
                   fig.activated = false;
               }
               else{
                   fig.activated = false;
               }
           }
           else
               fig.activated = false;
           return;
       }
    }
    // 道具情况
    //shuffle
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy] == 200){
        Shuffle_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        shuffle();
        return;
    }
    //flash
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy] == 50){
        flash_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        return;
    }
    //Hint
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy] ==100){
        hint_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        StillCanLink();
        return;
    }
    //Time
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy] == 150){
        Time_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        return;
    }
    //dizzy
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy] == 250){
        fig.dizzy_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        return;
    }
}

void QLink::fig_A(figure &fig)
{
    if(fig.Posy == -1){
        fig.Posy = fig.Posy;
        return;
    }
    else if(fig.Posy == 0||fig.Posx == -1||fig.Posx == MapRow){
        fig.Posy = fig.Posy - 1;
        return;
    }
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] == 0){
        fig.Posx = fig.Posx;
        fig.Posy = fig.Posy - 1;
    }
    //A有格子
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] >= 1&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] <= IconNum){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy;
       if(fig.activated_block[0] == -1||fig.activated == false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx;
           fig.activated_block[1] = fig.Posy - 1;
       }
       else{
           if((fig.activated_block[0] != fig.Posx||fig.activated_block[1] != fig.Posy-1)
                   &&gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] == gamemap[(fig.Posx)*MapCol+fig.Posy-1]){
               if(fig_TwoCanLink(fig.activated_block[0],fig.activated_block[1],fig.Posx,fig.Posy-1,fig)){
                   if(gamemap[(fig.Posx)*MapCol+fig.Posy-1] >= 1&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] <= 9)
                       fig.Score += 30;
                   else if(gamemap[(fig.Posx)*MapCol+fig.Posy-1] >= 10&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] <= 14){
                       fig.Score += 60;
                   }
                   else
                       fig.Score += 100;
                   gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
                   gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] = 0;
                   fig.activated = false;
                   fig.activated_block[0] = -1;
                   fig.activated_block[1] = -1;
               }
               else
                   fig.activated = false;
           }
           else
               fig.activated = false;
       }
    }
    // 道具情况
    //shuffle
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] == 200){
        Shuffle_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        shuffle();
        return;
    }
    //flash
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] == 50){
        flash_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        return;
    }
    //hint
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] == 100){
        hint_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        StillCanLink();
        return;
    }
    //Time
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] == 150){
        Time_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        return;
    }
    //dizzy
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1] == 250){
        fig.dizzy_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        return;
    }
}

void QLink::fig_S(figure &fig)
{
    if(fig.Posx == MapRow){
        fig.Posx = fig.Posx;
        return;
    }
    else if(fig.Posx == MapRow-1||fig.Posy == -1||fig.Posy == MapCol){
        fig.Posx = fig.Posx + 1;
        return;
    }
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] == 0){
        fig.Posx = fig.Posx + 1;
        return;
    }
    //S有格子
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] >= 1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] <= IconNum){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy;
       if(fig.activated_block[0] == -1||fig.activated == false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx + 1;
           fig.activated_block[1] = fig.Posy;
       }
       else{
           if((fig.activated_block[0] != fig.Posx+1||fig.activated_block[1] != fig.Posy)
                   &&gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] == gamemap[(fig.Posx+1)*MapCol+fig.Posy]){
               if(fig_TwoCanLink(fig.activated_block[0],fig.activated_block[1],fig.Posx + 1,fig.Posy,fig)){
                   // 连接成功加分 不同种类加分不同 宝可梦越强 加分越多
                   if(gamemap[(fig.Posx+1)*MapCol+fig.Posy] >=1 &&gamemap[(fig.Posx+1)*MapCol+fig.Posy] <= 9)
                       fig.Score+=30;
                   else if(gamemap[(fig.Posx+1)*MapCol+fig.Posy] >= 10&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] <= 14){
                       fig.Score += 60;
                   }
                   else
                       fig.Score += 100;
                   gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
                   gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] = 0;
                   fig.activated = false;
                   fig.activated_block[0] = -1;
                   fig.activated_block[1] = -1;
               }
               else
                   fig.activated = false;
           }
           else
               fig.activated = false;
       }
    }
    // 道具情况
    //shuffle
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] == 200){
        Shuffle_item = true;
        gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
        shuffle();
        return;
    }
    //flash
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] == 50){
        flash_item = true;
        gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
        return;
    }
    //hint
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] == 100){
        hint_item = true;
        gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
        StillCanLink();
        return;
    }
    //Time
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] == 150){
        Time_item = true;
        gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
        return;
    }
    //dizzy
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy] == 250){
        fig.dizzy_item = true;
        gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
        return;
    }
}

void QLink::fig_D(figure &fig)
{
    if(fig.Posy==MapCol){
        fig.Posy = fig.Posy;
        return;
    }
    if(fig.Posy==MapCol-1||fig.Posx == -1||fig.Posx == MapRow){
         fig.Posy = fig.Posy + 1;
         return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] == 0){
        fig.Posy = fig.Posy + 1;
        return;
    }
    //D有格子
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1]>=1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1]<=IconNum){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy ;
       if(fig.activated_block[0] == -1||fig.activated == false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx;
           fig.activated_block[1] = fig.Posy + 1;
       }
       else{ 
           if((fig.activated_block[0]!=fig.Posx||fig.activated_block[1]!=fig.Posy+1)
                   &&gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] == gamemap[(fig.Posx)*MapCol+fig.Posy+1]){
               if(fig_TwoCanLink(fig.activated_block[0],fig.activated_block[1],fig.Posx,fig.Posy + 1,fig)){
                   if(gamemap[(fig.Posx)*MapCol+fig.Posy+1] >= 1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] <= 9)
                       fig.Score+=30;
                   else if(gamemap[(fig.Posx)*MapCol+fig.Posy+1] >= 10&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] <= 14){
                       fig.Score += 60;
                   }
                   else
                       fig.Score += 100;
                   gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
                   gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] = 0;
                   fig.activated = false;
                   fig.activated_block[0] = -1;
                   fig.activated_block[1] = -1;
               }
               else
                   fig.activated = false;
           }
           else
               fig.activated = false;
       }
    }
    // 道具情况
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] == 200){
        Shuffle_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        shuffle();
        return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] == 50){
        flash_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] == 100){
        hint_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        StillCanLink();
        return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] == 150){
        Time_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1] == 250){
        fig.dizzy_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        return;
    }
}

bool QLink::One_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    //横线
    if(srcX == dstX){
        if(srcY>dstY)
            std::swap(srcY,dstY);
        for(int i = srcX*MapCol+srcY+1;i<srcX*MapCol+dstY;i++)
            if(gamemap[i] >= 1&&gamemap[i] <= IconNum)
                return false;
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        return true;
    }
    //竖线
    if(srcY == dstY){
        if(srcX>dstX)
            std::swap(srcX,dstX);
        for(int i = (srcX+1)*MapCol+srcY;i < dstX*MapCol+srcY;i += MapCol)
            if(gamemap[i] >= 1&&gamemap[i] <= IconNum)
                return false;
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        return true;
    }
    return false;
}

bool QLink::Two_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    if(srcY>dstY){
        std::swap(srcX,dstX);
        std::swap(srcY,dstY);
    }
//    if(srcX>dstX){
        if(!(gamemap[dstX*MapCol+srcY] >= 1&&gamemap[dstX*MapCol+srcY] <= IconNum)){
            if(One_Line(srcX,srcY,dstX,srcY,fig)&&One_Line(dstX,srcY,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
            return true;
            }
        }
        if(!(gamemap[srcX*MapCol+dstY] >= 1&&gamemap[srcX*MapCol+dstY] <= IconNum)){
            if(One_Line(srcX,srcY,srcX,dstY,fig)&&One_Line(srcX,dstY,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
                return true;
            }
        }
//    }
//    else {
//        if(gamemap[srcX*MapCol+dstY] == 0){
//            if(One_Line(srcX,srcY,srcX,dstY,fig)&&One_Line(srcX,dstY,dstX,dstY,fig)){
//                fig.blocks.clear();
//                Point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
//                fig.blocks.push_back(p1);
//                fig.blocks.push_back(p3);
//                fig.blocks.push_back(p2);
//                return true;
//            }
//        }
//        if(gamemap[dstX*MapCol+srcY]==0){
//            if(One_Line(srcX,srcY,dstX,srcY,fig)&&One_Line(dstX,srcY,dstX,dstY,fig)){
//                fig.blocks.clear();
//                Point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
//                fig.blocks.push_back(p1);
//                fig.blocks.push_back(p3);
//                fig.blocks.push_back(p2);
//                return true;
//            }
//        }
//    }
    return false;
}

bool QLink::Three_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    if(srcY>dstY){
        std::swap(srcY,dstY);
        std::swap(srcX,dstX);
    }
    for(int  y = 0;y<MapCol;y++){
        if(y != srcY&&y != dstY){
            if(!(gamemap[srcX*MapCol+y] >= 1&&gamemap[srcX*MapCol+y] <= IconNum)&&One_Line(srcX,srcY,srcX,y,fig)&&Two_Line(srcX,y,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(srcX,y),p3(dstX,y),p4(dstX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p2);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p4);
                return true;
            }
        }
    }
    for(int x = 0;x<MapRow;x++){
        if(x != srcX&&x!=dstX){
            if(!(gamemap[x*MapCol+srcY] >= 1&&gamemap[x*MapCol+srcY] <= IconNum)&&One_Line(srcX,srcY,x,srcY,fig)&&Two_Line(x,srcY,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(x,srcY),p3(x,dstY),p4(dstX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p2);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p4);
                return true;

            }
        }
    }
    if((srcX == 0||(!(gamemap[0*MapCol+srcY] >= 1&&gamemap[0*MapCol+srcY] <= IconNum)&&One_Line(srcX,srcY,0,srcY,fig)))
            &&(dstX==0||(!(gamemap[0*MapCol+dstY] >= 1&&gamemap[0*MapCol+dstY] <= IconNum)&&One_Line(0,dstY,dstX,dstY,fig)))){
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(-1,srcY),p3(-1,dstY),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcY == 0||(!(gamemap[srcX*MapCol+0] >= 1&&gamemap[0*MapCol+srcY] <= IconNum)&&One_Line(srcX,srcY,srcX,0,fig)))
            &&(dstY==0||(!(gamemap[dstX*MapCol+0] >= 1&&gamemap[dstX*MapCol+0] <= IconNum)&&One_Line(dstX,0,dstX,dstY,fig)))){
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(srcX,-1),p3(dstX,-1),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcX == MapRow -1||(!(gamemap[(MapRow-1)*MapCol+srcY] >= 1&&gamemap[(MapRow-1)*MapCol+srcY] <= IconNum)&&One_Line(srcX,srcY,MapRow-1,srcY,fig)))
            &&(dstX==MapRow -1||(!(gamemap[(MapRow-1)*MapCol+dstY] >= 1&&gamemap[(MapRow-1)*MapCol+dstY] <= IconNum)&&One_Line(MapRow-1,dstY,dstX,dstY,fig)))){
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(MapRow,srcY),p3(MapRow,dstY),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcY == MapCol-1||(!(gamemap[srcX*MapCol+MapCol-1] >= 1&&gamemap[srcX*MapCol+MapCol-1] <= IconNum)&&One_Line(srcX,srcY,srcX,MapCol-1,fig)))
            &&(dstY==MapCol-1||(!(gamemap[dstX*MapCol+MapCol-1] >= 1&&gamemap[dstX*MapCol+MapCol-1] <= IconNum)&&One_Line(dstX,MapCol-1,dstX,dstY,fig)))){
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(srcX,MapCol),p3(dstX,MapCol),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    return false;
}


