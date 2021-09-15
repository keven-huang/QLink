#ifndef GAME_MODEL_H
#define GAME_MODEL_H
#include"figure.h"
#include<vector>
const int MAX_ROW = 15;
const int MAX_COL = 20;
const int ICON_SIZE = 35.0;
const int MAX_ICON = 11;

struct point{
    int x;
    int y;
    point(int x1,int y1):x(x1),y(y1){

    }
};
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
    bool Two_linked(int srcX,int srcY,int dstX,int dstY);
    bool Is_No_link();
    bool isWin();
    int *activated_block;
    std::vector<point> points;
    //item
    void shuffle();
    void Hint();
    bool flash_item;
    bool Time_item;
    //操控角色1，2
    figure fig1;
    figure fig2;
    void fig1_W();
    void fig1_A();
    void fig1_S();
    void fig1_D();
    //游戏外
    void save();
private:
    int *gamemap;    
    int *hintarr;
    bool is_no_link;
    //bool isCanLink(int srcX, int srcY, int dstX, int dstY);

    bool canLinkDirectly(int srcX, int srcY, int dstX, int dstY);
    bool canLinkWithOneCorner(int srcX, int srcY, int dstX, int dstY);
    bool canLinkWithTwoCorner(int srcX, int srcY, int dstX, int dstY);
};

#endif // GAME_MODEL_H
