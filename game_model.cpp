#include "game_model.h"
#include<ctime>
#include<cstdlib>
#include <algorithm>
game_model::game_model():fig1(0,0),fig2(0,0)
{
    startGame(1);
}

game_model::game_model(int mode)
{
    startGame(mode);
}

game_model::~game_model()
{
    delete[] gamemap ;
    delete[] activated_block;
    delete [] hintarr;
}

void game_model::startGame()
{
    points.clear();
    gamemap = new int [MAX_COL*MAX_ROW];
    for(int i = 0;i<MAX_COL*MAX_ROW;i++)
        gamemap[i] = 0;
    hintarr = new int [4];
    activated_block = new int[4];
    for(int i = 0;i<4;i++)
        hintarr[i] = -1;
    for(int i = 0;i<4;i++)
        activated_block[i] = -1;
    int iconId = 0;
    for(int i = 0;iconId<MAX_ICON;i+=2){
       gamemap[i] = iconId%MAX_ICON + 1;
       gamemap[i+1] = iconId %MAX_ICON + 1;
       iconId++;
    }
    srand((unsigned)time(0));
    std::random_shuffle(gamemap,gamemap+MAX_COL*MAX_ROW);
    is_no_link = false;
    flash_item = false;
    Time_item = false;

}
void game_model::startGame(int mode)
{
    startGame();
    if(mode == 1){
        //单人模式在数组最后一个放人物模型
        //if(gamemap[MAX_COL*MAX_ROW - 1]==0) gamemap[MAX_COL*MAX_ROW - 1] = -1;
        if(gamemap[MAX_COL*MAX_ROW-1]!=0){
            int i;
            for(i = 0;i<MAX_COL*MAX_ROW;i++)
                if(gamemap[i] == 0)
                    break;
            std::swap(gamemap[i],gamemap[MAX_COL*MAX_ROW - 1]);
            //gamemap[MAX_COL*MAX_ROW - 1] = -1;
        }
        fig1.Posx = MAX_ROW - 1;
        fig1.Posy = MAX_COL - 1;
        return;
    }
    else if(mode == 2){
        //双人模式分别在左上角和右下角放置模型
        if(gamemap[0] != 0){
            int i;
            for(i = 0;i<MAX_COL*MAX_ROW;i++)
                if(gamemap[i] == 0)
                    break;
            std::swap(gamemap[i],gamemap[0]);
        }
        if(gamemap[MAX_COL*MAX_ROW-1]!=0){
            int i;
            for(i = 0;i<MAX_COL*MAX_ROW;i++)
                if(gamemap[i] == 0)
                    break;
            std::swap(gamemap[i],gamemap[MAX_COL*MAX_ROW - 1]);
            //gamemap[MAX_COL*MAX_ROW - 1] = -1;
        }
        fig1.Posx = 0;
        fig1.Posy = 0;
        fig2.Posx = MAX_ROW - 1;
        fig2.Posy = MAX_COL - 1;
        return;
    }

}

int *game_model::Get_gamemap()
{
    return gamemap;
}

bool game_model::Two_linked(int srcX, int srcY, int dstX, int dstY)
{
    if(canLinkDirectly(srcX,srcY,dstX,dstY)){
        gamemap[srcX*MAX_COL+srcY]=0;
        gamemap[dstX*MAX_COL+dstY]=0;
        return true;
    }
    points.clear();
    if(canLinkWithOneCorner(srcX,srcY,dstX,dstY)){
        gamemap[srcX*MAX_COL+srcY]=0;
        gamemap[dstX*MAX_COL+dstY]=0;
        return true;
    }
    points.clear();
    if(canLinkWithTwoCorner(srcX,srcY,dstX,dstY)){
        gamemap[srcX*MAX_COL+srcY]=0;
        gamemap[dstX*MAX_COL+dstY]=0;
        return true;
    }
    points.clear();
    return false;
}

bool game_model::isWin()
{
    for(int i = 0;i<MAX_COL*MAX_ROW;i++){
        if(gamemap[i]>=1&&gamemap[i]<=MAX_ICON)
            return false;
    }
    return true;
}

void game_model::fig1_W()
{
    if(fig1.Posx>0&&gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy]==0){
        fig1.Posx = fig1.Posx - 1;
        //std::swap(gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy],gamemap[(fig1.Posx)*MAX_COL+fig1.Posy]);
        return;
    }
    //W有格子
    else if(fig1.Posx>0&&gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy]>=1&&gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy]<=MAX_ICON){
       fig1.Posx = fig1.Posx;
       fig1.Posy = fig1.Posy;
       if(activated_block[0] == -1){
           activated_block[0] = fig1.Posx - 1;
           activated_block[1] = fig1.Posy;
           return;
       }
       //已有激活
       else{
           if((activated_block[0]!=fig1.Posx-1||activated_block[1]!=fig1.Posy)
                   &&gamemap[activated_block[0]*MAX_COL+activated_block[1]] == gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy]){
               Two_linked(activated_block[0],activated_block[1],fig1.Posx - 1,fig1.Posy);
           }
           activated_block[0] = -1;
           activated_block[1] = -1;
           return;
       }
    }
    // 道具情况
    else if(fig1.Posx>0&&gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy]==50){
        return;
    }
    else if(fig1.Posx>0&&gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy]==100){
        flash_item = true;
        //fig1.Posx = fig1.Posx - 1;
        gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy] = 0;
        return;
    }
    else if(fig1.Posx>0&&gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy]==150){
        Time_item = true;
        //fig1.Posx = fig1.Posx - 1;
        gamemap[(fig1.Posx-1)*MAX_COL+fig1.Posy] = 0;
        return;
    }
}

void game_model::fig1_A()
{
    if(fig1.Posy>0&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1]==0){
        fig1.Posx = fig1.Posx;
        fig1.Posy = fig1.Posy - 1;
        //std::swap(gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1],gamemap[(fig1.Posx)*MAX_COL+fig1.Posy]);
    }
    //A有格子
    else if(fig1.Posy>0&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1]>=1&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1]<=MAX_ICON){
       fig1.Posx = fig1.Posx;
       fig1.Posy = fig1.Posy;
       if(activated_block[0] == -1){
           activated_block[0] = fig1.Posx;
           activated_block[1] = fig1.Posy - 1;
       }
       else{
           if((activated_block[0]!=fig1.Posx||activated_block[1]!=fig1.Posy-1)
                   &&gamemap[activated_block[0]*MAX_COL+activated_block[1]] == gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1]){
               Two_linked(activated_block[0],activated_block[1],fig1.Posx,fig1.Posy-1);
           }
           activated_block[0] = -1;
           activated_block[1] = -1;
       }
    }
    // 道具情况
    else if(fig1.Posy>0&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1]==50){
        return;
    }
    else if(fig1.Posy>0&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1]==100){
        flash_item = true;
        //fig1.Posx = fig1.Posx;
        //fig1.Posy = fig1.Posy - 1;
        gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1] = 0;
        return;
    }
    else if(fig1.Posy>0&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1]==150){
        Time_item = true;
        //fig1.Posx = fig1.Posx;
        //fig1.Posy = fig1.Posy - 1;
        gamemap[(fig1.Posx)*MAX_COL+fig1.Posy-1] = 0;
        return;
    }
}

void game_model::fig1_S()
{
    if(fig1.Posx<MAX_ROW-1&&gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy]==0){
        fig1.Posx = fig1.Posx + 1;
        //std::swap(gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy],gamemap[(fig1.Posx)*MAX_COL+fig1.Posy]);
    }
    //S有格子
    else if(fig1.Posx<MAX_ROW-1&&gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy]>=1&&gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy]<=MAX_ICON){
       fig1.Posx = fig1.Posx;
       fig1.Posy = fig1.Posy;
       if(activated_block[0] == -1){
           activated_block[0] = fig1.Posx + 1;
           activated_block[1] = fig1.Posy;
       }
       else{
           if((activated_block[0]!=fig1.Posx+1||activated_block[1]!=fig1.Posy)
                   &&gamemap[activated_block[0]*MAX_COL+activated_block[1]] == gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy]){
               Two_linked(activated_block[0],activated_block[1],fig1.Posx + 1,fig1.Posy);
           }
           activated_block[0] = -1;
           activated_block[1] = -1;
       }
    }
    // 道具情况
    else if(fig1.Posx<MAX_ROW-1&&gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy]==50){

        return;
    }
    else if(fig1.Posx<MAX_ROW-1&&gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy]==100){
        flash_item = true;
        //fig1.Posx = fig1.Posx + 1;
        gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy] = 0;
        return;
    }
    else if(fig1.Posx<MAX_ROW-1&&gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy]==150){
        Time_item = true;
        //fig1.Posx = fig1.Posx + 1;
        gamemap[(fig1.Posx+1)*MAX_COL+fig1.Posy] = 0;
        return;
    }
}

void game_model::fig1_D()
{
    if(fig1.Posy<MAX_COL -1&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]==0){
        fig1.Posy = fig1.Posy + 1;
        //std::swap(gamemap[(fig1.Posx)*MAX_COL+fig1.Posy],gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]);
    }
    //D有格子
    else if(fig1.Posy<MAX_COL -1&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]>=1&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]<=MAX_ICON){
       fig1.Posx = fig1.Posx;
       fig1.Posy = fig1.Posy ;
       if(activated_block[0] == -1){
           activated_block[0] = fig1.Posx;
           activated_block[1] = fig1.Posy + 1;
       }
       else{ 
           if((activated_block[0]!=fig1.Posx||activated_block[1]!=fig1.Posy+1)
                   &&gamemap[activated_block[0]*MAX_COL+activated_block[1]] == gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]){
               Two_linked(activated_block[0],activated_block[1],fig1.Posx,fig1.Posy + 1);
           }
           activated_block[0] = -1;
           activated_block[1] = -1;

       }
    }
    // 道具情况
    else if(fig1.Posy<MAX_COL -1&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]==50){

        return;
    }
    else if(fig1.Posy<MAX_COL -1&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]==100){
        flash_item = true;
        //fig1.Posy = fig1.Posy + 1;
        gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1] = 0;
        return;
    }
    else if(fig1.Posy<MAX_COL -1&&gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1]==150){
        Time_item = true;
        //fig1.Posy = fig1.Posy + 1;
        gamemap[(fig1.Posx)*MAX_COL+fig1.Posy+1] = 0;
        return;
    }

}

bool game_model::canLinkDirectly(int srcX, int srcY, int dstX, int dstY)
{
    //一横线
    if(srcX == dstX){
        if(srcY>dstY)
            std::swap(srcY,dstY);
        for(int i=srcX*MAX_COL+srcY+1;i<srcX*MAX_COL+dstY;i++)
            if(gamemap[i]>=1&&gamemap[i]<=MAX_ICON)
                return false;
        points.clear();
        point p1(srcX,srcY),p2(dstX,dstY);
        points.push_back(p1);
        points.push_back(p2);
        return true;
    }

    if(srcY==dstY){
        if(srcX>dstX)
            std::swap(srcX,dstX);
        for(int i = (srcX+1)*MAX_COL+srcY;i<dstX*MAX_COL+srcY;i+=MAX_COL)
            if(gamemap[i]>=1&&gamemap[i]<=MAX_ICON)
                return false;
        points.clear();
        point p1(srcX,srcY),p2(dstX,dstY);
        points.push_back(p1);
        points.push_back(p2);
        return true;
    }
    return false;
}

bool game_model::canLinkWithOneCorner(int srcX, int srcY, int dstX, int dstY)
{
    if(srcY>dstY){
        std::swap(srcX,dstX);
        std::swap(srcY,dstY);
    }
    if(srcX>dstX){
        if(gamemap[dstX*MAX_COL+srcY]==0){
            if(canLinkDirectly(srcX,srcY,dstX,srcY)&&canLinkDirectly(dstX,srcY,dstX,dstY)){
                points.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
                points.push_back(p1);
                points.push_back(p3);
                points.push_back(p2);
            return true;
            }
        }
        if(gamemap[srcX*MAX_COL+dstY]==0){
            if(canLinkDirectly(srcX,srcY,srcX,dstY)&&canLinkDirectly(srcX,dstY,dstX,dstY)){
                points.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
                points.push_back(p1);
                points.push_back(p3);
                points.push_back(p2);
                return true;
            }
        }


    }
    else {
        if(gamemap[srcX*MAX_COL+dstY]==0){
            if(canLinkDirectly(srcX,srcY,srcX,dstY)&&canLinkDirectly(srcX,dstY,dstX,dstY)){
                points.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
                points.push_back(p1);
                points.push_back(p3);
                points.push_back(p2);
                return true;
            }
        }
        if(gamemap[dstX*MAX_COL+srcY]==0){
            if(canLinkDirectly(srcX,srcY,dstX,srcY)&&canLinkDirectly(dstX,srcY,dstX,dstY)){
                points.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
                points.push_back(p1);
                points.push_back(p3);
                points.push_back(p2);
                return true;
            }
        }
    }
    return false;
}

bool game_model::canLinkWithTwoCorner(int srcX, int srcY, int dstX, int dstY)
{
    if(srcY>dstY){
        std::swap(srcY,dstY);
        std::swap(srcX,dstX);
    }
    for(int  y = 0;y<MAX_COL;y++){
        if(y!=srcY&&y!=dstY){
            if(gamemap[srcX*MAX_COL+y]==0&&canLinkDirectly(srcX,srcY,srcX,y)&&canLinkWithOneCorner(srcX,y,dstX,dstY)){
                points.clear();
                point p1(srcX,srcY),p2(srcX,y),p3(dstX,y),p4(dstX,dstY);
                points.push_back(p1);
                points.push_back(p2);
                points.push_back(p3);
                points.push_back(p4);
                return true;
            }
        }
    }
    for(int x = 0;x<MAX_ROW;x++){
        if(x!=srcX&&x!=dstX){
            if(gamemap[x*MAX_COL+srcY]==0&&canLinkDirectly(srcX,srcY,x,srcY)&&canLinkWithOneCorner(x,srcY,dstX,dstY)){
                points.clear();
                point p1(srcX,srcY),p2(x,srcY),p3(x,dstY),p4(dstX,dstY);
                points.push_back(p1);
                points.push_back(p2);
                points.push_back(p3);
                points.push_back(p4);
                return true;

            }
        }
    }
    if((srcX == 0||(gamemap[0*MAX_COL+srcY]==0&&canLinkDirectly(srcX,srcY,0,srcY)))
            &&(dstX==0||(gamemap[0*MAX_COL+dstY]==0&&canLinkDirectly(0,dstY,dstX,dstY)))){
        point p1(srcX,srcY),p2(-1,srcY),p3(-1,dstY),p4(dstX,dstY);
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
        return true;
    }
    if((srcY == 0||(gamemap[srcX*MAX_COL+0]==0&&canLinkDirectly(srcX,srcY,srcX,0)))
            &&(dstY==0||(gamemap[dstX*MAX_COL+0]==0&&canLinkDirectly(dstX,0,dstX,dstY)))){
        point p1(srcX,srcY),p2(srcX,0),p3(dstX,0),p4(dstX,dstY);
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
        return true;
    }
    if((srcX == MAX_ROW -1||(gamemap[(MAX_ROW-1)*MAX_COL+srcY]==0&&canLinkDirectly(srcX,srcY,MAX_ROW-1,srcY)))
            &&(dstX==MAX_ROW -1||(gamemap[(MAX_ROW-1)*MAX_COL+dstY]==0&&canLinkDirectly(MAX_ROW-1,dstY,dstX,dstY)))){
        point p1(srcX,srcY),p2(MAX_ROW,srcY),p3(MAX_ROW,dstY),p4(dstX,dstY);
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
        return true;
    }
    if((srcY == MAX_COL-1||(gamemap[srcX*MAX_COL+MAX_COL-1]==0&&canLinkDirectly(srcX,srcY,srcX,MAX_COL-1)))
            &&(dstY==MAX_COL-1||(gamemap[dstX*MAX_COL+MAX_COL-1]==0&&canLinkDirectly(dstX,MAX_COL-1,dstX,dstY)))){
        point p1(srcX,srcY),p2(srcX,MAX_COL),p3(dstX,MAX_COL),p4(dstX,dstY);
        points.push_back(p1);
        points.push_back(p2);
        points.push_back(p3);
        points.push_back(p4);
        return true;
    }
    return false;
}


