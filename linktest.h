#ifndef LINKTEST_H
#define LINKTEST_H
#include<QtTest/QtTest>
#include "qlink.h"
class LinkTest : public QObject
{
    Q_OBJECT
public:
    LinkTest();
    QLink* Test_game;
private slots:
    void initTestCase();
    void One_Link_Test();
    void Two_Link_Test();
    void Three_Link_Test_border();
    void Three_Link_Test_inside();
    void All_Link_Test();
    void cleanupTestCase();
};

#endif // LINKTEST_H
