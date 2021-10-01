#include "game_model.h"
#include<ctime>
#include<cstdlib>
#include <algorithm>
game_model::game_model():fig1(0,0),fig2(0,0)
{
    startGame(1);
}

game_model::game_model(int mode):fig1(0,0),fig2(0,0)
{
    startGame(mode);
}

game_model::~game_model()
{
    delete[] gamemap ;
}

void game_model::startGame()
{
    gamemap = new int [Map_Col*Map_Row];
    for(int i = 0;i<Map_Col*Map_Row;i++)
        gamemap[i] = 0;
    int iconId = 0;
    int k = Map_Row*Map_Col/(Icon_Num*2);
    for(int i = 0;iconId<Icon_Num;){
        for(int j = 0 ;j<k;j++){
            gamemap[i] = iconId%Icon_Num + 1;
            gamemap[i+1] = iconId%Icon_Num + 1;
            i+=2;
        }
        iconId++;
    }
    srand((unsigned)time(0));
    std::random_shuffle(gamemap,gamemap+Map_Col*Map_Row);
    is_no_link = false;
    flash_item = Time_item = Shuffle_item = dizzy_item = false;
}
void game_model::startGame(int mode)
{
    startGame();
    if(mode == 1){
        //单人模式在右下角放置模型
        fig1.Posx = Map_Row;
        fig1.Posy = Map_Col;
        return;
    }
    else if(mode == 2){
        //双人模式分别在左上角和右下角放置模型
        fig1.Posx = -1;
        fig1.Posy = -1;
        fig2.Posx = Map_Row;
        fig2.Posy = Map_Col;
        return;
    }

}

int *game_model::Get_gamemap()
{
    return gamemap;
}

bool game_model::Two_linked(int srcX, int srcY, int dstX, int dstY,figure &fig)
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

bool game_model::isWin()
{
    for(int i = 0;i<Map_Col*Map_Row;i++){
        if(gamemap[i]>=1&&gamemap[i]<=Icon_Num)
            return false;
    }
    return true;
}

void game_model::shuffle()
{
    std::random_shuffle(gamemap,gamemap+Map_Col*Map_Row);
    if(gamemap[fig1.Posx*Map_Col+fig1.Posy] != 0){
        int i;
        for(i = 0;i<Map_Col*Map_Row;i++)
            if(gamemap[i] == 0)
                break;
        std::swap(gamemap[i],gamemap[0]);
    }
}

void game_model::fig1_W(figure &fig)
{
    if(fig.Posx==-1){
        fig.Posx = fig.Posx;
        return;
    }
    else if(fig.Posx==0||fig.Posy==-1||fig.Posy==Map_Col){
        fig.Posx = fig.Posx - 1;
        return;
    }
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]==0){
        fig.Posx = fig.Posx - 1;
        return;
    }
    //W有格子
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]>=1&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]<=Icon_Num){
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
                   &&gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]] == gamemap[(fig.Posx-1)*Map_Col+fig.Posy]){
               if(Two_linked(fig.activated_block[0],fig.activated_block[1],fig.Posx - 1,fig.Posy,fig)){
                   if(gamemap[(fig.Posx-1)*Map_Col+fig.Posy]>=1&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]<=9)
                       fig.Point+=30;
                   else if(gamemap[(fig.Posx-1)*Map_Col+fig.Posy]>=10&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]<=14){
                       fig.Point+=60;
                   }
                   else
                       fig.Point+=100;
                   gamemap[(fig.Posx-1)*Map_Col+fig.Posy]=0;
                   gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]]=0;
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
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]==200){
        Shuffle_item = true;
        gamemap[(fig.Posx-1)*Map_Col+fig.Posy] = 0;
        shuffle();
        return;
    }
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]==100){
        flash_item = true;
        gamemap[(fig.Posx-1)*Map_Col+fig.Posy] = 0;
        return;
    }
    else if(fig.Posx>0&&gamemap[(fig.Posx-1)*Map_Col+fig.Posy]==150){
        Time_item = true;
        gamemap[(fig.Posx-1)*Map_Col+fig.Posy] = 0;
        return;
    }
}

void game_model::fig1_A(figure &fig)
{
    if(fig.Posy==-1){
        fig.Posy = fig.Posy;
        return;
    }
    else if(fig.Posy == 0||fig.Posx==-1||fig.Posx==Map_Row){
        fig.Posy = fig.Posy - 1;
        return;
    }
    else if(fig.Posy>0&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]==0){
        fig.Posx = fig.Posx;
        fig.Posy = fig.Posy - 1;
    }
    //A有格子
    else if(fig.Posy>0&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]>=1&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]<=Icon_Num){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy;
       if(fig.activated_block[0] == -1||fig.activated==false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx;
           fig.activated_block[1] = fig.Posy - 1;
       }
       else{
           if((fig.activated_block[0]!=fig.Posx||fig.activated_block[1]!=fig.Posy-1)
                   &&gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]] == gamemap[(fig.Posx)*Map_Col+fig.Posy-1]){
               if(Two_linked(fig.activated_block[0],fig.activated_block[1],fig.Posx,fig.Posy-1,fig)){
                   if(gamemap[(fig.Posx)*Map_Col+fig.Posy-1]>=1&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]<=9)
                       fig.Point+=30;
                   else if(gamemap[(fig.Posx)*Map_Col+fig.Posy-1]>=10&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]<=14){
                       fig.Point+=60;
                   }
                   else
                       fig.Point+=100;
                   gamemap[(fig.Posx)*Map_Col+fig.Posy-1] = 0;
                   gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]]=0;
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
    else if(fig.Posy>0&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]==200){
        Shuffle_item = true;
        gamemap[(fig.Posx)*Map_Col+fig.Posy-1] = 0;
        shuffle();
        return;
    }
    else if(fig.Posy>0&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]==100){
        flash_item = true;
        gamemap[(fig.Posx)*Map_Col+fig.Posy-1] = 0;
        return;
    }
    else if(fig.Posy>0&&gamemap[(fig.Posx)*Map_Col+fig.Posy-1]==150){
        Time_item = true;
        gamemap[(fig.Posx)*Map_Col+fig.Posy-1] = 0;
        return;
    }
}

void game_model::fig1_S(figure &fig)
{
    if(fig.Posx==Map_Row){
        fig.Posx = fig.Posx;
        return;
    }
    else if(fig.Posx==Map_Row-1||fig.Posy==-1||fig.Posy==Map_Col){
        fig.Posx = fig.Posx + 1;
        return;
    }
    else if(fig.Posx<Map_Row-1&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]==0){
        fig.Posx = fig.Posx + 1;
        return;
    }
    //S有格子
    else if(fig.Posx<Map_Row-1&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]>=1&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]<=Icon_Num){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy;
       if(fig.activated_block[0] == -1||fig.activated == false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx + 1;
           fig.activated_block[1] = fig.Posy;
       }
       else{
           if((fig.activated_block[0]!=fig.Posx+1||fig.activated_block[1]!=fig.Posy)
                   &&gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]] == gamemap[(fig.Posx+1)*Map_Col+fig.Posy]){
               if(Two_linked(fig.activated_block[0],fig.activated_block[1],fig.Posx + 1,fig.Posy,fig)){
                   // 连接成功加分 不同种类加分不同 宝可梦越强 加分越多
                   if(gamemap[(fig.Posx+1)*Map_Col+fig.Posy]>=1&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]<=9)
                       fig.Point+=30;
                   else if(gamemap[(fig.Posx+1)*Map_Col+fig.Posy]>=10&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]<=14){
                       fig.Point+=60;
                   }
                   else
                       fig.Point+=100;
                   gamemap[(fig.Posx+1)*Map_Col+fig.Posy] = 0;
                   gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]] = 0;
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
    else if(fig.Posx<Map_Row-1&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]==200){
        flash_item = true;
        gamemap[(fig.Posx+1)*Map_Col+fig.Posy] = 0;
        shuffle();
        return;
    }
    else if(fig.Posx<Map_Row-1&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]==100){
        flash_item = true;
        //fig1.Posx = fig1.Posx + 1;
        gamemap[(fig.Posx+1)*Map_Col+fig.Posy] = 0;
        return;
    }
    else if(fig.Posx<Map_Row-1&&gamemap[(fig.Posx+1)*Map_Col+fig.Posy]==150){
        Time_item = true;
        //fig1.Posx = fig1.Posx + 1;
        gamemap[(fig.Posx+1)*Map_Col+fig.Posy] = 0;
        return;
    }
}

void game_model::fig1_D(figure &fig)
{
    if(fig.Posy==Map_Col){
        fig.Posy = fig.Posy;
        return;
    }
    if(fig.Posy==Map_Col-1||fig.Posx==-1||fig.Posx==Map_Row){
         fig.Posy = fig.Posy + 1;
         return;
    }
    else if(fig.Posy<Map_Col -1&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1]==0){
        fig.Posy = fig.Posy + 1;
        return;
    }
    //D有格子
    else if(fig.Posy<Map_Col -1&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1]>=1&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1]<=Icon_Num){
       fig.Posx = fig.Posx;
       fig.Posy = fig.Posy ;
       if(fig.activated_block[0] == -1||fig.activated == false){
           fig.activated = true;
           fig.activated_block[0] = fig.Posx;
           fig.activated_block[1] = fig.Posy + 1;
       }
       else{ 
           if((fig.activated_block[0]!=fig.Posx||fig.activated_block[1]!=fig.Posy+1)
                   &&gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]] == gamemap[(fig.Posx)*Map_Col+fig.Posy+1]){
               if(Two_linked(fig.activated_block[0],fig.activated_block[1],fig.Posx,fig.Posy + 1,fig)){
                   if(gamemap[(fig.Posx)*Map_Col+fig.Posy+1]>=1&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1])
                       fig.Point+=30;
                   else if(gamemap[(fig.Posx)*Map_Col+fig.Posy+1]>=10&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1]<=14){
                       fig.Point+=60;
                   }
                   else
                       fig.Point+=100;
                   gamemap[(fig.Posx)*Map_Col+fig.Posy+1] = 0;
                   gamemap[fig.activated_block[0]*Map_Col+fig.activated_block[1]] = 0;
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
    else if(fig.Posy<Map_Col -1&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1]==200){
        Shuffle_item = true;
        gamemap[(fig.Posx)*Map_Col+fig.Posy+1] = 0;
        shuffle();
        return;
    }
    else if(fig.Posy<Map_Col -1&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1]==100){
        flash_item = true;
        gamemap[(fig.Posx)*Map_Col+fig.Posy+1] = 0;
        return;
    }
    else if(fig.Posy<Map_Col -1&&gamemap[(fig.Posx)*Map_Col+fig.Posy+1]==150){
        Time_item = true;
        gamemap[(fig.Posx)*Map_Col+fig.Posy+1] = 0;
        return;
    }
    else{
        fig.Posy = fig.Posy+1;
    }

}

bool game_model::One_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    //一横线
    if(srcX == dstX){
        if(srcY>dstY)
            std::swap(srcY,dstY);
        for(int i=srcX*Map_Col+srcY+1;i<srcX*Map_Col+dstY;i++)
            if(gamemap[i]>=1&&gamemap[i]<=Icon_Num)
                return false;
        fig.blocks.clear();
        point p1(srcX,srcY),p2(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        return true;
    }

    if(srcY==dstY){
        if(srcX>dstX)
            std::swap(srcX,dstX);
        for(int i = (srcX+1)*Map_Col+srcY;i<dstX*Map_Col+srcY;i+=Map_Col)
            if(gamemap[i]>=1&&gamemap[i]<=Icon_Num)
                return false;
        fig.blocks.clear();
        point p1(srcX,srcY),p2(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        return true;
    }
    return false;
}

bool game_model::Two_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    if(srcY>dstY){
        std::swap(srcX,dstX);
        std::swap(srcY,dstY);
    }
    if(srcX>dstX){
        if(gamemap[dstX*Map_Col+srcY]==0){
            if(One_Line(srcX,srcY,dstX,srcY,fig)&&One_Line(dstX,srcY,dstX,dstY,fig)){
                fig.blocks.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
            return true;
            }
        }
        if(gamemap[srcX*Map_Col+dstY]==0){
            if(One_Line(srcX,srcY,srcX,dstY,fig)&&One_Line(srcX,dstY,dstX,dstY,fig)){
                fig.blocks.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
                return true;
            }
        }


    }
    else {
        if(gamemap[srcX*Map_Col+dstY]==0){
            if(One_Line(srcX,srcY,srcX,dstY,fig)&&One_Line(srcX,dstY,dstX,dstY,fig)){
                fig.blocks.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(srcX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
                return true;
            }
        }
        if(gamemap[dstX*Map_Col+srcY]==0){
            if(One_Line(srcX,srcY,dstX,srcY,fig)&&One_Line(dstX,srcY,dstX,dstY,fig)){
                fig.blocks.clear();
                point p1(srcX,srcY),p2(dstX,dstY),p3(dstX,srcY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p2);
                return true;
            }
        }
    }
    return false;
}

bool game_model::Three_Line(int srcX, int srcY, int dstX, int dstY,figure &fig)
{
    if(srcY>dstY){
        std::swap(srcY,dstY);
        std::swap(srcX,dstX);
    }
    for(int  y = 0;y<Map_Col;y++){
        if(y!=srcY&&y!=dstY){
            if(gamemap[srcX*Map_Col+y]==0&&One_Line(srcX,srcY,srcX,y,fig)&&Two_Line(srcX,y,dstX,dstY,fig)){
                fig.blocks.clear();
                point p1(srcX,srcY),p2(srcX,y),p3(dstX,y),p4(dstX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p2);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p4);
                return true;
            }
        }
    }
    for(int x = 0;x<Map_Row;x++){
        if(x!=srcX&&x!=dstX){
            if(gamemap[x*Map_Col+srcY]==0&&One_Line(srcX,srcY,x,srcY,fig)&&Two_Line(x,srcY,dstX,dstY,fig)){
                fig.blocks.clear();
                point p1(srcX,srcY),p2(x,srcY),p3(x,dstY),p4(dstX,dstY);
                fig.blocks.push_back(p1);
                fig.blocks.push_back(p2);
                fig.blocks.push_back(p3);
                fig.blocks.push_back(p4);
                return true;

            }
        }
    }
    if((srcX == 0||(gamemap[0*Map_Col+srcY]==0&&One_Line(srcX,srcY,0,srcY,fig)))
            &&(dstX==0||(gamemap[0*Map_Col+dstY]==0&&One_Line(0,dstY,dstX,dstY,fig)))){
        fig.blocks.clear();
        point p1(srcX,srcY),p2(-1,srcY),p3(-1,dstY),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcY == 0||(gamemap[srcX*Map_Col+0]==0&&One_Line(srcX,srcY,srcX,0,fig)))
            &&(dstY==0||(gamemap[dstX*Map_Col+0]==0&&One_Line(dstX,0,dstX,dstY,fig)))){
        fig.blocks.clear();
        point p1(srcX,srcY),p2(srcX,-1),p3(dstX,-1),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcX == Map_Row -1||(gamemap[(Map_Row-1)*Map_Col+srcY]==0&&One_Line(srcX,srcY,Map_Row-1,srcY,fig)))
            &&(dstX==Map_Row -1||(gamemap[(Map_Row-1)*Map_Col+dstY]==0&&One_Line(Map_Row-1,dstY,dstX,dstY,fig)))){
        fig.blocks.clear();
        point p1(srcX,srcY),p2(Map_Row,srcY),p3(Map_Row,dstY),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    if((srcY == Map_Col-1||(gamemap[srcX*Map_Col+Map_Col-1]==0&&One_Line(srcX,srcY,srcX,Map_Col-1,fig)))
            &&(dstY==Map_Col-1||(gamemap[dstX*Map_Col+Map_Col-1]==0&&One_Line(dstX,Map_Col-1,dstX,dstY,fig)))){
        fig.blocks.clear();
        point p1(srcX,srcY),p2(srcX,Map_Col),p3(dstX,Map_Col),p4(dstX,dstY);
        fig.blocks.push_back(p1);
        fig.blocks.push_back(p2);
        fig.blocks.push_back(p3);
        fig.blocks.push_back(p4);
        return true;
    }
    return false;
}


