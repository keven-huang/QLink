#include "figure.h"


figure::figure()
{
    blocks.clear();
    Score = 0;
    activated = false;
    for(int i = 0;i<2;i++)
        activated_block[i] = -1;
    dizzy_item = false;
}

figure::figure(int x, int y):Posx(x),Posy(y),Score(0)
{
    blocks.clear();
    activated = false;
    for(int i = 0;i<2;i++)
        activated_block[i] = -1;
    dizzy_item = false;
}
figure::~figure(){
}
