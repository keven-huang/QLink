#include "linktest.h"

LinkTest::LinkTest()
{
    Test_game = new QLink;
    for(int i = 1;i<MapCol*MapRow-1;i++)
        Test_game->Get_gamemap()[i]=1;
    Test_game->Get_gamemap()[0]=0;
    Test_game->Get_gamemap()[MapCol-1]=0;
    Test_game->Get_gamemap()[(MapRow-1)*MapCol]=0;
    Test_game->Get_gamemap()[MapCol*MapRow-1]=0;
}

void LinkTest::initTestCase()
{

}
/*One_Link_Test共测试四种情况
 * 分别是箱子左下时 先竖线后横线连接 先横线竖线连接
 *      箱子左上时 先竖线后横线连接 先横线竖线连接
 * 正确错误都进行测试
 */
void LinkTest::One_Link_Test()
{
    QVERIFY(Test_game->One_Line(0,1,0,2,Test_game->fig1)==true);
    QVERIFY(Test_game->One_Line(0,1,0,3,Test_game->fig1)==false);
    //竖线
    QVERIFY(Test_game->One_Line(1,0,2,0,Test_game->fig1)==true);
    QVERIFY(Test_game->One_Line(1,0,3,0,Test_game->fig1)==false);
}
/*Two_Link_Test共测试四种情况
 * 分别是箱子左下时 先竖线后横线连接 先横线竖线连接
 *      箱子左上时 先竖线后横线连接 先横线竖线连接
 * 正确错误都进行测试
 */
void LinkTest::Two_Link_Test()
{
    QVERIFY(Test_game->Two_Line(0,1,1,0,Test_game->fig1)==true);
    QVERIFY(Test_game->Two_Line(0,2,1,0,Test_game->fig1)==false);
    QVERIFY(Test_game->Two_Line(MapRow-1,MapCol-2,MapRow-2,MapCol-1,Test_game->fig1)==true);
    QVERIFY(Test_game->Two_Line(MapRow-1,MapCol-2,MapRow-3,MapCol-1,Test_game->fig1)==false);
    QVERIFY(Test_game->Two_Line(MapRow-1,MapCol-2,MapRow-2,MapCol-1,Test_game->fig1)==true);
    QVERIFY(Test_game->Two_Line(MapRow-1,MapCol-2,MapRow-3,MapCol-1,Test_game->fig1)==false);
    QVERIFY(Test_game->Two_Line(MapRow-1,MapCol-2,MapRow-2,MapCol-1,Test_game->fig1)==true);
    QVERIFY(Test_game->Two_Line(MapRow-1,MapCol-2,MapRow-3,MapCol-1,Test_game->fig1)==false);

}
/*Three_Link_Test共测试四种情况
 * 分别是箱子左下时 先竖线后横线连接 先横线竖线连接
 *      箱子左上时 先竖线后横线连接 先横线竖线连接
 * 正确错误都进行测试
 *最后调换srcX srcY和dstX dstY
 */
void LinkTest::Three_Link_Test_border()
{
     QVERIFY(Test_game->Three_Line(0,1,0,3,Test_game->fig1)==true);
     QVERIFY(Test_game->Three_Line(1,0,0,3,Test_game->fig1)==true);

     QVERIFY(Test_game->Three_Line(1,0,3,0,Test_game->fig1)==true);
     QVERIFY(Test_game->Three_Line(0,1,0,3,Test_game->fig1)==true);

     QVERIFY(Test_game->Three_Line(MapRow-1,1,MapRow-1,3,Test_game->fig1)==true);
     QVERIFY(Test_game->Three_Line(MapRow-2,0,MapRow-1,3,Test_game->fig1)==true);

     QVERIFY(Test_game->Three_Line(1,MapCol-1,3,MapCol-1,Test_game->fig1)==true);
     QVERIFY(Test_game->Three_Line(0,MapCol-2,3,MapCol-1,Test_game->fig1)==true);
}

void LinkTest::Three_Link_Test_inside()
{
    Test_game->Get_gamemap()[0] = 0;
    QVERIFY(Test_game->Three_Line(0,1,0,3,Test_game->fig1)==true);
    QVERIFY(Test_game->Three_Line(1,0,0,3,Test_game->fig1)==true);

    QVERIFY(Test_game->Three_Line(1,0,3,0,Test_game->fig1)==true);
    QVERIFY(Test_game->Three_Line(0,1,0,3,Test_game->fig1)==true);

    QVERIFY(Test_game->Three_Line(MapRow-1,1,MapRow-1,3,Test_game->fig1)==true);
    QVERIFY(Test_game->Three_Line(MapRow-2,0,MapRow-1,3,Test_game->fig1)==true);

    QVERIFY(Test_game->Three_Line(1,MapCol-1,3,MapCol-1,Test_game->fig1)==true);
    QVERIFY(Test_game->Three_Line(0,MapCol-2,3,MapCol-1,Test_game->fig1)==true);
}

void LinkTest::All_Link_Test()
{
    for(int i = 0;i<=10;){
        int x,y;
        x = rand()%(MapCol*MapRow);
        y = rand()%(MapCol*MapRow);
        if(x!=y&&Test_game->Get_gamemap()[x]==Test_game->Get_gamemap()[y]){
            QCOMPARE(Test_game->fig_TwoCanLink(x/MapCol,x%MapCol,y/MapCol,y%MapCol,Test_game->fig1),
                     Test_game->fig_TwoCanLink(y/MapCol,y%MapCol,x/MapCol,x%MapCol,Test_game->fig1));
            i++;
        }
    }
}

void LinkTest::cleanupTestCase()
{
    delete Test_game;
}

QTEST_MAIN(LinkTest);
