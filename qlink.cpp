#include "qlink.h"
#include<ctime>
#include<cstdlib>
#include <algorithm>
QLink::QLink():fig1(0,0),fig2(0,0)
{
    startGame(1);
}

QLink::QLink(int mode):fig1(0,0),fig2(0,0)
{
    startGame(mode);
}

QLink::~QLink()
{
    delete[] gamemap ;
}

void QLink::startGame()
{
    gamemap = new int [MapCol*MapRow];
    for(int i = 0;i<MapCol*MapRow;i++)
        gamemap[i] = 0;
    int iconId = 0;
    int k = MapRow*MapCol/(IconNum*2);
    for(int i = 0;iconId<IconNum;){
        for(int j = 0 ;j<k;j++){
            gamemap[i] = iconId%IconNum + 1;
            gamemap[i+1] = iconId%IconNum + 1;
            i+=2;
        }
        iconId++;
    }
    srand((unsigned)time(0));
    std::random_shuffle(gamemap,gamemap+MapCol*MapRow);
    flash_item = Time_item = Shuffle_item = dizzy_item = false;
}
void QLink::startGame(int mode)
{
    startGame();
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

int *QLink::Get_gamemap()
{
    return gamemap;
}

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

bool QLink::isWin()
{
    for(int i = 0;i<MapCol*MapRow;i++){
        if(gamemap[i]>=1&&gamemap[i]<=IconNum)
            return false;
    }
    return true;
}

bool QLink::StillCanLink()
{
    solution.clear();
    for(int i = 0;i<MapCol;i++){
        int j = 0;
        if(gamemap[i]>=1&&gamemap[i]<=IconNum){
            Point p(i,j);
            solution.push_back(p);
        }
    }
    for(int i = 0;i<int(solution.size());i++){
        for(int j = i;j<int(solution.size());j++)
            //if((solution[i].x!=solution[j].x||solution[i].y!=solution[j].y)
            //&&Two_Line(solution[i].x,solution[i].y,solution[j].x,solution[j].y)){
                return true;
            }
    return false;
}

void QLink::shuffle()
{
    std::random_shuffle(gamemap,gamemap+MapCol*MapRow);
    if(gamemap[fig1.Posx*MapCol+fig1.Posy] != 0){
        int i;
        for(i = 0;i<MapCol*MapRow;i++)
            if(gamemap[i] == 0)
                break;
        std::swap(gamemap[i],gamemap[0]);
    }
}

void QLink::fig_W(figure &fig)
{
    if(fig.Posx==-1){
        fig.Posx = fig.Posx;
        return;
    }
    else if(fig.Posx==0||fig.Posy==-1||fig.Posy==MapCol){
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
           if((fig.activated_block[0]!=fig.Posx-1||fig.activated_block[1]!=fig.Posy)
                   &&gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] == gamemap[(fig.Posx-1)*MapCol+fig.Posy]){
               if(fig_TwoCanLink(fig.activated_block[0],fig.activated_block[1],fig.Posx - 1,fig.Posy,fig)){
                   if(gamemap[(fig.Posx-1)*MapCol+fig.Posy]>=1&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]<=9)
                       fig.Score+=30;
                   else if(gamemap[(fig.Posx-1)*MapCol+fig.Posy]>=10&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]<=14){
                       fig.Score+=60;
                   }
                   else
                       fig.Score+=100;
                   gamemap[(fig.Posx-1)*MapCol+fig.Posy]=0;
                   gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]]=0;
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
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]==200){
        Shuffle_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        shuffle();
        return;
    }
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]==100){
        flash_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        return;
    }
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*MapCol+fig.Posy]==150){
        Time_item = true;
        gamemap[(fig.Posx-1)*MapCol+fig.Posy] = 0;
        return;
    }
}

void QLink::fig_A(figure &fig)
{
    if(fig.Posy==-1){
        fig.Posy = fig.Posy;
        return;
    }
    else if(fig.Posy == 0||fig.Posx==-1||fig.Posx==MapRow){
        fig.Posy = fig.Posy - 1;
        return;
    }
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]==0){
        fig.Posx = fig.Posx;
        fig.Posy = fig.Posy - 1;
    }
    //A有格子
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]>=1&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]<=IconNum){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy;
       if(fig.activated_block[0] == -1||fig.activated==false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx;
           fig.activated_block[1] = fig.Posy - 1;
       }
       else{
           if((fig.activated_block[0]!=fig.Posx||fig.activated_block[1]!=fig.Posy-1)
                   &&gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] == gamemap[(fig.Posx)*MapCol+fig.Posy-1]){
               if(fig_TwoCanLink(fig.activated_block[0],fig.activated_block[1],fig.Posx,fig.Posy-1,fig)){
                   if(gamemap[(fig.Posx)*MapCol+fig.Posy-1]>=1&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]<=9)
                       fig.Score+=30;
                   else if(gamemap[(fig.Posx)*MapCol+fig.Posy-1]>=10&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]<=14){
                       fig.Score+=60;
                   }
                   else
                       fig.Score+=100;
                   gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
                   gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]]=0;
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
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]==200){
        Shuffle_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        shuffle();
        return;
    }
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]==100){
        flash_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        return;
    }
    else if(fig.Posy>0&&gamemap[(fig.Posx)*MapCol+fig.Posy-1]==150){
        Time_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy-1] = 0;
        return;
    }
}

void QLink::fig_S(figure &fig)
{
    if(fig.Posx==MapRow){
        fig.Posx = fig.Posx;
        return;
    }
    else if(fig.Posx==MapRow-1||fig.Posy==-1||fig.Posy==MapCol){
        fig.Posx = fig.Posx + 1;
        return;
    }
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]==0){
        fig.Posx = fig.Posx + 1;
        return;
    }
    //S有格子
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]>=1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]<=IconNum){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy;
       if(fig.activated_block[0] == -1||fig.activated == false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx + 1;
           fig.activated_block[1] = fig.Posy;
       }
       else{
           if((fig.activated_block[0]!=fig.Posx+1||fig.activated_block[1]!=fig.Posy)
                   &&gamemap[fig.activated_block[0]*MapCol+fig.activated_block[1]] == gamemap[(fig.Posx+1)*MapCol+fig.Posy]){
               if(fig_TwoCanLink(fig.activated_block[0],fig.activated_block[1],fig.Posx + 1,fig.Posy,fig)){
                   // 连接成功加分 不同种类加分不同 宝可梦越强 加分越多
                   if(gamemap[(fig.Posx+1)*MapCol+fig.Posy]>=1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]<=9)
                       fig.Score+=30;
                   else if(gamemap[(fig.Posx+1)*MapCol+fig.Posy]>=10&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]<=14){
                       fig.Score+=60;
                   }
                   else
                       fig.Score+=100;
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
               fig.activated =false;
       }
    }
    // 道具情况
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]==200){
        flash_item = true;
        gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
        shuffle();
        return;
    }
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]==100){
        flash_item = true;
        //fig1.Posx = fig1.Posx + 1;
        gamemap[(fig.Posx+1)*MapCol+fig.Posy] = 0;
        return;
    }
    else if(fig.Posx<MapRow-1&&gamemap[(fig.Posx+1)*MapCol+fig.Posy]==150){
        Time_item = true;
        //fig1.Posx = fig1.Posx + 1;
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
    if(fig.Posy==MapCol-1||fig.Posx==-1||fig.Posx==MapRow){
         fig.Posy = fig.Posy + 1;
         return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1]==0){
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
                   if(gamemap[(fig.Posx)*MapCol+fig.Posy+1]>=1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1])
                       fig.Score+=30;
                   else if(gamemap[(fig.Posx)*MapCol+fig.Posy+1]>=10&&gamemap[(fig.Posx)*MapCol+fig.Posy+1]<=14){
                       fig.Score+=60;
                   }
                   else
                       fig.Score+=100;
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
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1]==200){
        Shuffle_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        shuffle();
        return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1]==100){
        flash_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        return;
    }
    else if(fig.Posy<MapCol -1&&gamemap[(fig.Posx)*MapCol+fig.Posy+1]==150){
        Time_item = true;
        gamemap[(fig.Posx)*MapCol+fig.Posy+1] = 0;
        return;
    }
    else{
        fig.Posy = fig.Posy+1;
    }

}

bool QLink::One_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    //一横线
    if(srcX == dstX){
        if(srcY>dstY)
            std::swap(srcY,dstY);
        for(int i=srcX*MapCol+srcY+1;i<srcX*MapCol+dstY;i++)
            if(gamemap[i]>=1&&gamemap[i]<=IconNum)
                return false;
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        return true;
    }

    if(srcY==dstY){
        if(srcX>dstX)
            std::swap(srcX,dstX);
        for(int i = (srcX+1)*MapCol+srcY;i<dstX*MapCol+srcY;i+=MapCol)
            if(gamemap[i]>=1&&gamemap[i]<=IconNum)
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
    if(srcX>dstX){
        if(gamemap[dstX*MapCol+srcY]==0){
            if(One_Line(srcX,srcY,dstX,srcY,fig)&&One_Line(dstX,srcY,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
            return true;
            }
        }
        if(gamemap[srcX*MapCol+dstY]==0){
            if(One_Line(srcX,srcY,srcX,dstY,fig)&&One_Line(srcX,dstY,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
                return true;
            }
        }


    }
    else {
        if(gamemap[srcX*MapCol+dstY]==0){
            if(One_Line(srcX,srcY,srcX,dstY,fig)&&One_Line(srcX,dstY,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
                return true;
            }
        }
        if(gamemap[dstX*MapCol+srcY]==0){
            if(One_Line(srcX,srcY,dstX,srcY,fig)&&One_Line(dstX,srcY,dstX,dstY,fig)){
                fig.blocks.clear();
                Point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
                return true;
            }
        }
    }
    return false;
}

bool QLink::Three_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    if(srcY>dstY){
        std::swap(srcY,dstY);
        std::swap(srcX,dstX);
    }
    for(int  y = 0;y<MapCol;y++){
        if(y!=srcY&&y!=dstY){
            if(gamemap[srcX*MapCol+y]==0&&One_Line(srcX,srcY,srcX,y,fig)&&Two_Line(srcX,y,dstX,dstY,fig)){
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
        if(x!=srcX&&x!=dstX){
            if(gamemap[x*MapCol+srcY]==0&&One_Line(srcX,srcY,x,srcY,fig)&&Two_Line(x,srcY,dstX,dstY,fig)){
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
    if((srcX == 0||(gamemap[0*MapCol+srcY]==0&&One_Line(srcX,srcY,0,srcY,fig)))
            &&(dstX==0||(gamemap[0*MapCol+dstY]==0&&One_Line(0,dstY,dstX,dstY,fig)))){
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(-1,srcY),p3(-1,dstY),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcY == 0||(gamemap[srcX*MapCol+0]==0&&One_Line(srcX,srcY,srcX,0,fig)))
            &&(dstY==0||(gamemap[dstX*MapCol+0]==0&&One_Line(dstX,0,dstX,dstY,fig)))){
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(srcX,-1),p3(dstX,-1),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcX == MapRow -1||(gamemap[(MapRow-1)*MapCol+srcY]==0&&One_Line(srcX,srcY,MapRow-1,srcY,fig)))
            &&(dstX==MapRow -1||(gamemap[(MapRow-1)*MapCol+dstY]==0&&One_Line(MapRow-1,dstY,dstX,dstY,fig)))){
        fig.blocks.clear();
        Point p1(srcX,srcY),p2(MapRow,srcY),p3(MapRow,dstY),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcY == MapCol-1||(gamemap[srcX*MapCol+MapCol-1]==0&&One_Line(srcX,srcY,srcX,MapCol-1,fig)))
            &&(dstY==MapCol-1||(gamemap[dstX*MapCol+MapCol-1]==0&&One_Line(dstX,MapCol-1,dstX,dstY,fig)))){
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


