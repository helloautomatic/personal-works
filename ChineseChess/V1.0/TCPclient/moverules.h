#ifndef MOVERULSE_H
#define MOVERULSE_H

#include<vector>
#include<iostream>
#include<string>
#include<cmath>
#include<iostream>

#define GUOHE 4  // 从0开始.4也是未过河的临界点
#define JIANG_SHI_MIN_X 3
#define JIANG_SHI_MIN_Y 0
#define JIANG_SHI_MAX_X 5
#define JIANG_SHI_MAX_Y 2

enum DOT_STATE
{
    PLAYER0,
    PLAYER1,
    PLAYER_NO,  // 用于标志该dot没有被棋子占据
    NO_NAME,
    BING,
    PAO,
    CHE,
    MA,
    XIANG,
    SHI,
    JIANG
};

class Dot
{
public:
    Dot() {}
    Dot(int x_, int y_, int player_, int qiziName_) :x(x_), y(y_), player(player_), qiziName(qiziName_) {}

public:
    int x;
    int y;
    int player;  // PLAYER0,PLAYER1,PLAYER_NO,
    int qiziName;
public:
    double calculateDistance(Dot* dot1);
    double calculateAngle(Dot* dot1);
    int calculateDotNum(Dot* dot1);
};


class MoveRulesAbstract {
public:
    MoveRulesAbstract() {}
    virtual ~MoveRulesAbstract() {}
    virtual bool isMovable(Dot* dot1, Dot* dot2) = 0;
};


class MoveRule1 :public MoveRulesAbstract
{
public:
    virtual bool isMovable(Dot* dot1, Dot* dot2);
    virtual ~MoveRule1();
};


class MoveRule2 :public MoveRulesAbstract
{
public:
    virtual bool isMovable(Dot* dot1, Dot* dot2);
    virtual ~MoveRule2();
};


class MoveRule3 :public MoveRulesAbstract
{
public:
    virtual bool isMovable(Dot* dot1, Dot* dot2);
    virtual ~MoveRule3();
};


class MoveRule4 :public MoveRulesAbstract
{
public:
    virtual bool isMovable(Dot* dot1, Dot* dot2);
    virtual ~MoveRule4();
};


class MoveRule5 :public MoveRulesAbstract
{
public:
    virtual bool isMovable(Dot* dot1, Dot* dot2);
    virtual ~MoveRule5();
};

extern std::vector<Dot*> dot_vector;  // 全局变量的声明

class ChessBoard {
public:
    ChessBoard();
    ~ChessBoard();
    int dotLen();
    bool isMovable(Dot* dot1, Dot* dot2);
    Dot* dot1 = nullptr;
    Dot* dot2 = nullptr;

private:
    bool initChessBoard();
    bool initRules();
};

#endif
