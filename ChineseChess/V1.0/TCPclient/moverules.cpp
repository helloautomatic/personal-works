/*
该源文件用于约束行棋规则,比如炮翻山,象走田字,马走日字,兵走过楚河汉界后才能左右移动且不能返回等等

*/

#include "moverules.h"
#include <QDebug>
#include <sstream>
#include <iomanip>

std::vector<Dot*> dot_vector;  // 全局变量的定义
std::vector<MoveRulesAbstract*> rules_vector;  // 存放规则

/*
计算两个棋子之间的距离,这里的距离是以 1 为单位
也即一个棋盘格子的长度为1
用于判断行棋的距离规则
*/
double Dot::calculateDistance(Dot* dot1) {
    return std::sqrt(std::pow(dot1->x - this->x, 2) + std::pow(dot1->y - this->y, 2));
}

/*
计算行棋方向角度,角度范围0~360度
用于判断行棋的方向规则
*/
double Dot::calculateAngle(Dot* dot1) {

    int dx = dot1->x - this->x;
    int dy = dot1->y - this->y;

    double angle = std::atan2(dy, dx) * (180.0 / acos(-1.0));

    if (angle < 0) {
        angle += 360.0;
    }
    return angle;
}


/*
计算两个棋子之间棋子的数量
用于判断炮翻山需要满足的规则,以及老将对脸规则判断
*/
int Dot::calculateDotNum(Dot* dot1) {
    // ax+by+c=0
    float a = dot1->y - this->y;
    float b = this->x - dot1->x;
    float c = dot1->x * this->y - this->x * dot1->y;
    float distance = 0.0;

    int counter = 0;
    for (Dot* dot : dot_vector) {
        if(dot->player != PLAYER_NO){  // 必须是被棋子占据的坐标点才算
            if (int(dot->x * a + dot->y * b + c) == 0) {  // 在直线上
                distance = float(this->calculateDistance(dot1));

                /*判断一个dot是不是在两点之间点*/
                if (distance > dot->calculateDistance(this) && distance > dot->calculateDistance(dot1)) {
                    counter++;
                }
            }
        }
    }
    qDebug()<<"两个棋子之间棋子的个数为:"<<counter;
    return counter;
}

/*
前置规则 MoveRules1
*/
bool MoveRule1::isMovable(Dot* dot1, Dot* dot2) {
    bool returnVal = 1;

    if (dot1->player == PLAYER_NO) { // 第一次点击为空
        returnVal = 0;
    }

    if (dot1->player == dot2->player) {  // 两次点击为同一颜色的棋子
        returnVal = 0;
    }

    if (dot1->x == dot2->x && dot1->y == dot2->y) {  // 两次点击为同一点
        returnVal = 0;
    }

    // 这里考虑到了笛卡尔坐标系和平面直角坐标系的对称问题，
    // 如果不考虑对称问题，这里的布尔表达式应该填写 PLAYER1 == dot1->player
    if(!(PLAYER0 == dot1->player)){  // 无论是吃子，还是走棋，第一次点击都不可能是对方，只能是自己
        returnVal = 0;
    }

    if (returnVal == 0) qDebug() << "不符合前置条件";
    return returnVal;
}

MoveRule1::~MoveRule1()
{

}


/*
距离规则 MoveRule2
*/
bool MoveRule2::isMovable(Dot* dot1, Dot* dot2) {
    bool returnVal = 1;
    float distance = dot1->calculateDistance(dot2);  // 计算距离的字符串

    switch (dot1->qiziName)
    {
    case BING:
    case JIANG:
        if (dot1->calculateDistance(dot2) != 1.0) returnVal = 0;
        break;

    case MA:
        if(int(dot1->calculateDistance(dot2)*1000) != int(std::sqrt(1*1 + 2*2)*1000)) returnVal = 0;
        break;

    case XIANG:
        if (int(dot1->calculateDistance(dot2)*1000) != int(std::sqrt(2 * 2 + 2 * 2)*1000)) returnVal = 0;
        break;

    case SHI:
        if (int(dot1->calculateDistance(dot2)*1000) != int(std::sqrt(1 + 1)*1000)) returnVal = 0;
        break;

    default:
        break;
    }

    if (returnVal == 0)qDebug() << "不符合距离规范";
    qDebug()<<"行棋距离:"<<distance;
    return returnVal;
}

MoveRule2::~MoveRule2()
{

}

/*
方向规则,MoveRule3
*/
bool MoveRule3::isMovable(Dot* dot1, Dot* dot2) {
    double agl = dot1->calculateAngle(dot2);
    bool returnVal = 1;
    switch (dot1->qiziName)
    {
    case PAO:
    case CHE:
    case JIANG:
        if (!(agl == 0.0 || agl == 90.0 || agl == 180.0 || agl == 270.0 || agl == 360.0))returnVal = 0;
        break;

    case BING:
        if (dot1->y <= GUOHE) {
            if (!(agl == 90.0))returnVal = 0;
        }
        else {
            if (!(agl == 0.0 || agl == 90.0 || agl == 180.0))returnVal = 0;
        }
        break;

    default:
        break;
    }
    if (returnVal == 0)qDebug() <<"不符合方向规范";
    qDebug()<<"行棋角度:"<<agl;
    return returnVal;
}

MoveRule3::~MoveRule3()
{

}


/*
范围规则,MoveRule4
*/
bool MoveRule4::isMovable(Dot* dot1, Dot* dot2) {
    bool returnVal = 1;
    switch (dot1->qiziName)
    {
    case SHI:
        if (!((JIANG_SHI_MAX_X >= dot2->x && dot2->x >= JIANG_SHI_MIN_X) && (JIANG_SHI_MAX_Y >= dot2->y && dot2->y >= JIANG_SHI_MIN_Y)))returnVal = 0;
        break;

    case XIANG:
        if (!(dot2->y <= GUOHE))returnVal = 0;
        break;

    default:
        break;
    }
    if (returnVal == 0)qDebug() <<"不符合范围规范";
    return returnVal;

}

MoveRule4::~MoveRule4()
{

}


/*
特殊规则炮翻山,拖马腿,和面,扯象脚 MoveRule5
*/
bool MoveRule5::isMovable(Dot* dot1, Dot* dot2) {

    bool returnVal = 1;
    int counter = dot1->calculateDotNum(dot2);
    Dot dot_temp = *dot1;
    double agl = dot1->calculateAngle(dot2);

    switch (dot1->qiziName) // 根据第一个棋子判断
    {

    case PAO:
        if (counter >= 2)
        {
            returnVal = 0;
        }
        else if (counter == 1)
        {
            if (!(dot1->player != dot2->player && dot2->player != PLAYER_NO && dot1->player != PLAYER_NO))  // 吃子
            {
                returnVal = 0;
            }
            else{;}
        }
        else if(counter == 0){
            if(dot2->player != PLAYER_NO){
                returnVal = 0;
            }
        }
        break;

    case CHE:
        if (counter >= 1)returnVal = 0;
        break;

    case XIANG:
        if (!(counter == 0))returnVal = 0;
        break;

    case MA:
        if (45.0 < agl && agl < 135.0) { // 上
            dot_temp.y += 2;
        }

        if (135.0 < agl && agl < 225.0) { // 左
            dot_temp.x -= 2;
        }

        if (225.0 < agl && agl< 315.0) { // 下
            dot_temp.y -= 2;
        }

        if ((315.0 < agl && agl< 360.0) || (0.0 < agl && agl < 45.0)) {  //右
            dot_temp.x += 2;
        }

        if (!(dot1->calculateDotNum(&dot_temp) == 0))returnVal = 0;
        break;


    case JIANG:

        /*如果两个棋子之间没有棋子，且双方点击的棋子都是将，则符合行棋规则*/
        if (counter == 0 && dot1->qiziName == dot2->qiziName){
            returnVal = 1;
        }

        if (!((JIANG_SHI_MAX_X >= dot2->x && dot2->x >= JIANG_SHI_MIN_X)
              && (JIANG_SHI_MAX_Y >= dot2->y && dot2->y >= JIANG_SHI_MIN_Y)))
            returnVal = 0;
        break;

    default:
        break;
    }
    if (returnVal == 0)qDebug() <<"不符特殊规则";
    return returnVal;
}

MoveRule5::~MoveRule5()
{

}

/*
对所有的规则进行判断
如果前后点击的两个点 Dot1和Dot2 满足所有规则(rules_vector中的所有规则),
那么函数返回1,否则返回0
*/
bool ChessBoard::isMovable(Dot* dot1, Dot* dot2) {
    bool reuturnVal = 1;
    for (const auto& rule : rules_vector) {
        if (!rule->isMovable(dot1, dot2)) {
            reuturnVal = 0;
            break;
        }
    }
    return reuturnVal;
}


int ChessBoard::dotLen() {
    return dot_vector.size();
}


/*
构造函数
*/
ChessBoard::ChessBoard() {
    initChessBoard();
    initRules();
}


/*
析构函数
*/
ChessBoard::~ChessBoard() {
    for (const auto& dot : dot_vector) {
        delete dot;
    }

    for (const auto& rule : rules_vector) {
        delete rule;
    }

    dot_vector.clear();
    rules_vector.clear();
}


bool ChessBoard::initChessBoard() {

    /*该函数在程序启动后,只能被调用一次,调用次数超过1将触发程序错误*/
    if (dot_vector.size() != 0) {
        return 1;
    }

    /*初始化棋盘中，被棋子占据的位置*/
    dot_vector.push_back(new Dot(0, 3, PLAYER0, BING));
    dot_vector.push_back(new Dot(2, 3, PLAYER0, BING));
    dot_vector.push_back(new Dot(4, 3, PLAYER0, BING));
    dot_vector.push_back(new Dot(6, 3, PLAYER0, BING));
    dot_vector.push_back(new Dot(8, 3, PLAYER0, BING));
    dot_vector.push_back(new Dot(0, 6, PLAYER1, BING));
    dot_vector.push_back(new Dot(2, 6, PLAYER1, BING));
    dot_vector.push_back(new Dot(4, 6, PLAYER1, BING));
    dot_vector.push_back(new Dot(6, 6, PLAYER1, BING));
    dot_vector.push_back(new Dot(8, 6, PLAYER1, BING));
    dot_vector.push_back(new Dot(1, 2, PLAYER0, PAO));
    dot_vector.push_back(new Dot(7, 2, PLAYER0, PAO));
    dot_vector.push_back(new Dot(1, 7, PLAYER1, PAO));
    dot_vector.push_back(new Dot(7, 7, PLAYER1, PAO));
    dot_vector.push_back(new Dot(0, 0, PLAYER0, CHE));
    dot_vector.push_back(new Dot(8, 0, PLAYER0, CHE));
    dot_vector.push_back(new Dot(0, 9, PLAYER1, CHE));
    dot_vector.push_back(new Dot(8, 9, PLAYER1, CHE));
    dot_vector.push_back(new Dot(1, 0, PLAYER0, MA));
    dot_vector.push_back(new Dot(7, 0, PLAYER0, MA));
    dot_vector.push_back(new Dot(1, 9, PLAYER1, MA));
    dot_vector.push_back(new Dot(7, 9, PLAYER1, MA));
    dot_vector.push_back(new Dot(2, 0, PLAYER0, XIANG));
    dot_vector.push_back(new Dot(6, 0, PLAYER0, XIANG));
    dot_vector.push_back(new Dot(2, 9, PLAYER1, XIANG));
    dot_vector.push_back(new Dot(6, 9, PLAYER1, XIANG));
    dot_vector.push_back(new Dot(3, 0, PLAYER0, SHI));
    dot_vector.push_back(new Dot(5, 0, PLAYER0, SHI));
    dot_vector.push_back(new Dot(3, 9, PLAYER1, SHI));
    dot_vector.push_back(new Dot(5, 9, PLAYER1, SHI));
    dot_vector.push_back(new Dot(4, 0, PLAYER0, JIANG));
    dot_vector.push_back(new Dot(4, 9, PLAYER1, JIANG));

    /*初始化棋盘中没被棋子占据的位置*/
    std::vector<Dot*> dot_temp_vector;
    int count = 0;
    int count02 = 0;
    for(int x=0;x<9;x++){
        for(int y = 0;y<10;y++){
            bool found = false;  // 用于标志是否找到了目标元素
            for(auto dot:dot_vector){
                if((dot->x == x) && (dot->y == y)){
                    found = true;  // 找到匹配的元素
                    count ++;
                    break;
                }
            }
            if(!found){
                dot_temp_vector.push_back(new Dot(x,y,PLAYER_NO,NO_NAME));
                count02 ++;
            }
        }
    }

    /*拼接dot_vector和dot_temp_vector,形成整个棋盘的所有dot*/
    dot_vector.insert(dot_vector.end(), dot_temp_vector.begin(), dot_temp_vector.end());

    return 0;  // 该返回值目前没有实际意义,仅仅是为了满足C++的语法规则要求
}

/*
将所有行棋规则添加到 rules_vector中
rules_vector是全局变量
*/
bool ChessBoard::initRules() {

    /* 该函数在程序启动后只能被调用一次,多次调用会引发错误 */
    if (rules_vector.size() != 0){
        return 1;
    }

    /* 将5个行棋规则添加到 rules_vector中 */
    rules_vector.push_back(new MoveRule1());
    rules_vector.push_back(new MoveRule2());
    rules_vector.push_back(new MoveRule3());
    rules_vector.push_back(new MoveRule4());
    rules_vector.push_back(new MoveRule5());
    return 0;
}
