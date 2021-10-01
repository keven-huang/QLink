#ifndef GAME_MODEL_H
#define GAME_MODEL_H
#include"figure.h"
#include<vector>

const int Map_Row = 6;
const int Map_Col = 6;
const int Icon_Size = 50;
const int Icon_Num = 9;

/* 游戏模块类
 * 初始化共做5件事情
 * 1.初始化activated block
 * 2.将激活状态设置为false
 * 3.将激活方块数组初始化
 * 4.将绘图用点集合初始化
 * 5.将Point初始化为0
 */

class game_model
{
public:
    game_model();
    game_model(int mode);
    ~game_model();
    void startGame();
    void startGame(int mode);
    //游戏内部功能
    int* Get_gamemap();
    bool Two_linked(int srcX,int srcY,int dstX,int dstY,figure &fig);
    bool Is_No_link();
    bool isWin();
    //item
    void shuffle();
    bool flash_item;
    bool Time_item;
    bool Shuffle_item;
    bool dizzy_item;
    //操控角色1，2
    figure fig1;
    figure fig2;
    void fig1_W(figure &fig);
    void fig1_A(figure &fig);
    void fig1_S(figure &fig);
    void fig1_D(figure &fig);
    //游戏外
    void save();
private:
    int *gamemap;    
    bool is_no_link;

    bool One_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    bool Two_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
    bool Three_Line(int srcX, int srcY, int dstX, int dstY,figure &fig);
};

#endif // GAME_MODEL_H
