#include "chesswin.h"
#include "ui_chesswin.h"
#include <QGraphicsTextItem>
#include "moverules.h"
#include <QDebug>
#include <QThread>
#include <QMouseEvent>
#include <QVBoxLayout>
#include "customgraphicsview.h"
#include <QPushButton>
#include <QCloseEvent>
#include <string>
#include <QMessageBox>
#include "protocol.h"
#include "tcpclient.h"
#include "opewidget.h"

QVector<Dot*> cliDotVec;
int isMovableFlag = MOVABLE_UNKNOWN;  // 未知状态
int isMoveLocal = MOVE_LOCAL;  // 初始化为本地行棋
int playerColor = COLOR_NONE;  // 初始化为未知颜色
int isOpponentBegein = PLAYER_NOT_BEGIN;  // 初始化为玩家未开始

ChessWin::ChessWin(QString cheColor, QWidget *parent)
    : QWidget(parent), ui(new Ui::ChessWin),chessColor(cheColor)
{
    ui->setupUi(this);
    view = new CustomGraphicsView();  // 创建自定义的GraphicsView
    outerLayout = new QVBoxLayout(this);  // 创建一个布局
    outerLayout->addWidget(view);  // 将布局添加到this
    scene = new QGraphicsScene(this); // 创建GraphicsScene
    view->setScene(scene);  // 设定GraphicsView的GraphicsScene


    chessboard = new ChessBoard();  // 创建虚拟棋盘,会初始化全局变量 dot_vector和rules_vector
    opppnentName = "";  // 初始化为空串


    view->setRenderHint(QPainter::Antialiasing, true);  // 抗锯齿渲染

    innerLayout = new QHBoxLayout();
    regretPb = new QPushButton("悔棋");
    surrenderPb = new QPushButton("投降");
    exitPb = new QPushButton("退出");
    reStartGamePb = new QPushButton("重新开始");


    /*将按钮添加到布局*/
    innerLayout->addWidget(regretPb);
    innerLayout->addWidget(surrenderPb);
    innerLayout->addWidget(exitPb);
    innerLayout->addWidget(reStartGamePb);

    outerLayout->addLayout(innerLayout);

    initDrawChessboard(); // 绘制棋盘,该初始化只进行一次

    initAllConnection();  // 连接所有的信号和槽函数

    initGame();  // 初始化游戏
}


ChessWin::~ChessWin()
{
    delete ui; ui = nullptr;
    delete view; view = nullptr;

    delete scene; scene = nullptr;
    delete chessboard; chessboard = nullptr;

    delete regretPb; regretPb = nullptr;
    delete surrenderPb; surrenderPb = nullptr;
    delete exitPb; exitPb = nullptr;
    delete reStartGamePb; reStartGamePb = nullptr;


    delete outerLayout; outerLayout = nullptr;
}

// 槽函数,悔棋
void ChessWin::handlePbRegret(){
    QMessageBox::information(this,"提示","该功能尚未开发");
    qDebug()<<"点击了悔棋按钮";
}


// 槽函数,投降
void ChessWin::handlePbSurrender(){
    PDU * sendPdu = mkPDU(0);
    sendPdu->uiMsgType = ENUM_MSG_TYPE_OPPONENT_SURRENDER_REQUEST;  // 投降
    char name1[32] = {'\0'};
    char name2[32] = {'\0'};
    strcpy(name1,TcpClient::getInstance().getLoginName().toStdString().c_str());
    strcat(name2,this->getOpponentName().toStdString().c_str());
    qDebug()<<name1<<"向"<<name2<<"投降";

    memcpy(sendPdu->caData,TcpClient::getInstance().getLoginName().toStdString().c_str(),32);  // 自己的名字
    memcpy(sendPdu->caData + 32,this->getOpponentName().toStdString().c_str(),32);  // 对手的名字

    TcpClient::getInstance().getTcpSocket().write((char*)sendPdu,sendPdu->uiPDULen);
    free(sendPdu);
    sendPdu = NULL;

    qDebug()<<"点击了投降按钮";
}


// 槽函数,退出
void ChessWin::handlePbExit(){
    this->close();
    qDebug()<<"点击了退出按钮";
}

// 改变dot_vector中的内容
// x和y分别是鼠标点击产生的最原始的坐标
bool ChessWin::changeDot_vector(int x,int y){

    bool isMovable = 0;

    /*TODO 棋盘的y坐标对称*/
    y = 9*GRID_SIZE - y;

    for(auto dot:dot_vector){
        if((pow(x - dot->x*GRID_SIZE,2) + pow(y - dot->y*GRID_SIZE,2)) <= pow(CIRCLE_SIZE,2)){
            qDebug()<<"坐标"<<"("<<dot->x<<","<<dot->y<<")";
            qDebug()<<"名字:"<<getChessName(dot->qiziName);
            cliDotVec.insert(cliDotVec.begin(),dot);  // 在头部插入一个元素
            if(cliDotVec.size()>2){
                cliDotVec.pop_back();
            }

            if(cliDotVec.size()==2){  // 第0个元素为后插入的元素
                isMovable = chessboard->isMovable(cliDotVec[1],cliDotVec[0]);
            }

            if(isMovable == 1){
                qDebug()<<"符合行棋规范";

                /*走棋*/
                if((cliDotVec[1]->player != PLAYER_NO) && (cliDotVec[0]->player == PLAYER_NO)){
                    qDebug()<<"走棋";

                    cliDotVec[0]->player = cliDotVec[1]->player;
                    cliDotVec[0]->qiziName = cliDotVec[1]->qiziName;

                    cliDotVec[1]->player = PLAYER_NO;
                    cliDotVec[1]->qiziName = NO_NAME;
                }

                /*吃子*/
                if((cliDotVec[1]->player != PLAYER_NO) && (cliDotVec[0]->player != PLAYER_NO)){
                    qDebug()<<"吃子";

                    cliDotVec[0]->player = cliDotVec[1]->player;
                    cliDotVec[0]->qiziName = cliDotVec[1]->qiziName;

                    cliDotVec[1]->player = PLAYER_NO;
                    cliDotVec[1]->qiziName = NO_NAME;
                }

                freshChessboard();  // 刷新棋盘

                /*注意，append()函数接受整数参数时会将其视为一个字节，并将其添加到字节数组中。*/
                PDU *sendPdu = mkPDU(sizeof(int) * 8);  // 两个dot的数据，所有有8个int,mkPDU中传入的是字节的个数，不是int类型的个数

                /*添加第一个点击*/
                sendPdu->caMsg[0] = cliDotVec[1]->x;  // x
                sendPdu->caMsg[1] = cliDotVec[1]->y;  // y
                sendPdu->caMsg[2] = cliDotVec[1]->player;  // player
                sendPdu->caMsg[3] = cliDotVec[1]->qiziName;  // qiziName

                /*添加第二个点击*/
                sendPdu->caMsg[4] = cliDotVec[0]->x;  // x
                sendPdu->caMsg[5] = cliDotVec[0]->y;  // y
                sendPdu->caMsg[6] = cliDotVec[0]->player;  // player
                sendPdu->caMsg[7] = cliDotVec[0]->qiziName;  // qiziName
                //client->sendDataToServer(byteArray);

                cliDotVec.clear();  // 完成一次行棋后，需要清空cliDotVec

                // 行棋标志位为0
                isMovableFlag = 0;


                // 发送数据给服务器

                QString name1 = TcpClient::getInstance().getLoginName();  // 获取当地用户的名字,该名字通常不会变化

                // 获取对手的名字
                ChessWin *cheWin = OpeWidget::getInstance().getChessWin();
                if(nullptr == cheWin){
                    QMessageBox::critical(this,"严重错误","程序在运行过程出现了重大错误!源文件:chesswin.cpp;布尔表达式:nullptr == cheWin");
                }
                QString name2 = cheWin->getOpponentName(); //获取对手的名字
                qDebug()<<"name1:"<<name1<<"name2"<<name2;


                sendPdu->uiMsgType = ENUM_MSG_TYPE_OPPONENT_MOVE_CHESS_REQUEST;  // 对手行棋
                memcpy(sendPdu->caData,name1.toStdString().c_str(),32);  // 拷贝第一个名字到caData的前32个字节中
                memcpy(sendPdu->caData + 32,name2.toStdString().c_str(),32);  // 拷贝第二个名字到caData的后32个字节中

                // 发送消息
                qDebug()<<"需要发送过去的行棋信息:";
                for(uint i = 0;i<sendPdu->uiMsgLen;i++){
                    qDebug()<<sendPdu->caMsg[i];
                }
                qDebug()<<"sizeof(sendPdu)"<<sizeof(sendPdu);
                qDebug()<<"sendPdu->uiPDULen"<<sendPdu->uiPDULen;

                TcpClient::getInstance().getTcpSocket().write((char*)sendPdu,sendPdu->uiPDULen);  // 发送消息
                free(sendPdu);
                sendPdu = NULL;

                // 行棋完成，行棋标志位设置为0
                isMovableFlag = 0;

            }
        }
    }
    return 0;  // 该返回值无实际意义
}


/*
该函数用于处理对手行棋，本地棋盘的刷新问题
x1和y1是最先被点击的棋子，x2和y2是后来被点击的棋子
调用时机为:当收到服务端发过来的信息(信号)后,会自动触发client的槽函数,该槽函数会调用
changeDot_vevtor_opponent
*/
void ChessWin::changeDot_vevtor_opponent(int x1,int y1,int x2,int y2){

    /*TODO 这里仅仅使用了棋盘的y轴对称*/
    y1=9-y1;
    y2=9-y2;

    /*根据对手发过来的dot1和dot2信息,找到dot1和dot2在dot_vector中对应的指针,根据指针对dot_vector中的内容进行修改*/
    Dot*dot1 = nullptr;
    Dot*dot2 = nullptr;
    for(auto dot:dot_vector){
        if(x1==dot->x && y1 == dot->y) dot1 = dot;
        if(x2==dot->x && y2 == dot->y) dot2 = dot;
    }

    /*修改dot_vector中的内容*/
    dot2->player = dot1->player;
    dot2->qiziName = dot1->qiziName;
    dot1->player = PLAYER_NO;
    dot1->qiziName = NO_NAME;
}

void ChessWin::initGame()
{
    freshDotHanziVector();
    freshChessboard();
    this->setWindowTitle(TcpClient::getInstance().getLoginName());
}

void ChessWin::setOpponetName(QString newName)
{
    this->opppnentName = newName;
}

QString ChessWin::getOpponentName()
{
    return this->opppnentName;
}


ChessWin &ChessWin::getInstance()
{
    static ChessWin instance("");
    return instance;
}


/*
槽函数
用于处理本地行棋事件的处理
该函数的调用时机是,当本地玩家点击了棋盘行棋时,该函数将被调用
*/
void ChessWin::handleClick(const QPointF &pos)
{
    if(isMovableFlag == 0){  // 如果行棋标志位为0，则直接返回
        QMessageBox::information(this,"提示","该对方行棋");
        return;  // 返回值无实际意义
    }

    int x = int(pos.x());
    int y = int(pos.y());
    changeDot_vector(x,y);
}

void ChessWin::handleRestartGame()
{
    QMessageBox::information(this,"提示","该功能尚未开发");
    qDebug()<<"点击了重新开始按钮";
}


/*
绘制一个棋子
*/
void ChessWin::drawOneChess(qreal centerX, qreal centerY, qreal radius,
                            const QString& chineseText,HanziChess *hanziche,
                            QString cheColor){

    //参数检查
    if(!(CHESS_COLOR_RED == cheColor || CHESS_COLOR_BLACK == cheColor)){
        qDebug()<<"在函数 drawOneChess 中参数检查不合格";
        return;
    }

    /*指定圆的位置和半径*/
    qreal x = centerX - radius;
    qreal y = centerY - radius;

    /*创建圆形*/
    QGraphicsEllipseItem *circle = scene->addEllipse(x, y, radius * 2, radius * 2);
    circle->setBrush(Qt::lightGray);
    QPen pen(Qt::black, 2);  // 设置线条颜色和宽度
    circle->setPen(pen);

    QGraphicsTextItem *textItem = scene->addText(chineseText);  // 创建汉字

    QFont font("Arial", int(radius - 10));  // 指定字体
    textItem->setFont(font);

    /*根据传入的参数 cheColor来确定棋子的颜色*/
    if(CHESS_COLOR_RED == cheColor)
    {
        if(PLAYER0 == hanziche->player)
        {
            textItem->setDefaultTextColor(Qt::red);
        }
        else if(PLAYER1 == hanziche->player)
        {
            textItem->setDefaultTextColor(Qt::black);
        }
    }
    else if(CHESS_COLOR_BLACK == cheColor)
    {
        if(PLAYER0 == hanziche->player)
        {
            textItem->setDefaultTextColor(Qt::black);
        }
        else if(PLAYER1 == hanziche->player)
        {
            textItem->setDefaultTextColor(Qt::red);
        }
    }

    /*计算汉字的位置使其居中*/
    qreal textWidth = textItem->boundingRect().width();
    qreal textHeight = textItem->boundingRect().height();
    qreal textX = centerX - textWidth / 2.0;
    qreal textY = centerY - textHeight / 2.0;
    textItem->setPos(textX, textY);

    hanziche->text = textItem;
    hanziche->circle = circle;
}

/*
绘制棋盘
*/
void ChessWin::initDrawChessboard(){
    const int rows = 9;
    const int cols = 8;
    const int squareSize = GRID_SIZE;

    QPen pen(Qt::black, 2); // 设置线条颜色和宽度

    /*绘制垂直线条*/
    for (int x = 0; x <= cols; ++x) {
        qreal startX = x * squareSize;
        qreal startY = 0;
        qreal endX = startX;
        qreal endY = rows * squareSize;

        QGraphicsLineItem *lineItem = scene->addLine(startX, startY, endX, endY);
        lineItem->setPen(pen);
    }

    /*绘制水平线条*/
    for (int y = 0; y <= rows; ++y) {
        qreal startX = 0;
        qreal startY = y * squareSize;
        qreal endX = cols * squareSize;
        qreal endY = startY;

        QGraphicsLineItem *lineItem = scene->addLine(startX, startY, endX, endY);
        lineItem->setPen(pen);
    }

    {
        qreal startX = 3 * squareSize;
        qreal startY = 0;

        qreal endX = 5 * squareSize;
        qreal endY = 2 * squareSize;

        QGraphicsLineItem *lineItem = scene->addLine(startX, startY, endX, endY);
        lineItem->setPen(pen);
    }

    {
        qreal startX = 3 * squareSize;
        qreal startY = 2 * squareSize;

        qreal endX = 5 * squareSize;
        qreal endY = 0;

        QGraphicsLineItem *lineItem = scene->addLine(startX, startY, endX, endY);
        lineItem->setPen(pen);
    }


    {
        qreal startX = 3 * squareSize;
        qreal startY = 9 * squareSize;

        qreal endX = 5 * squareSize;
        qreal endY = 7 * squareSize;

        QGraphicsLineItem *lineItem = scene->addLine(startX, startY, endX, endY);
        lineItem->setPen(pen);
    }

    {
        qreal startX = 3 * squareSize;
        qreal startY = 7 * squareSize;

        qreal endX = 5 * squareSize;
        qreal endY = 9 * squareSize;

        QGraphicsLineItem *lineItem = scene->addLine(startX, startY, endX, endY);
        lineItem->setPen(pen);
    }



    {
        qreal startX = 0;
        qreal startY = 4 * squareSize;

        qreal w = 8 * squareSize;
        qreal h = 1 * squareSize;
        QGraphicsRectItem * rectItem = scene->addRect(startX,startY,w,h);
        rectItem->setPen(pen);
        rectItem->setBrush(Qt::lightGray);
    }

    scene->setBackgroundBrush(Qt::lightGray);

    // 绘制其他东西
}

void ChessWin::freshDotHanziVector(){
    for(auto dot:dot_vector){
        hanziChess_vector.push_back(new HanziChess(
                                        dot->x,dot->y,dot->player,dot->qiziName,
                                        nullptr,nullptr));
    }

    /*TODO 需要注意的是，这里也使用了x和y的同时对称*/
    for(auto hanziche:hanziChess_vector){
        if(hanziche->player != PLAYER_NO)
            drawOneChess(hanziche->x * GRID_SIZE,
                         (9 - hanziche->y) * GRID_SIZE,
                         CIRCLE_SIZE,
                         getChessName(hanziche->qiziName),
                         hanziche,this->chessColor);
    }
}

/*根据dot_vector中的内容对棋盘进行刷新*/
void ChessWin::freshChessboard(){
    auto dot = dot_vector.begin();
    auto hanzi = hanziChess_vector.begin();
    for(;dot != dot_vector.end();dot ++,hanzi++){
        if((*hanzi)->text != nullptr || (*hanzi)->circle != nullptr){
            scene->removeItem((*hanzi)->text);
            scene->removeItem((*hanzi)->circle);
        }
    }
    hanziChess_vector.clear();  // 清除hanziChess_vector中的内容
    freshDotHanziVector();  // 重新初始化,这里是根据dot_vector中的内容进行初始化,每进行一次行棋,都要进行一次freshChessboard
}


QString ChessWin::getChessName(int n){
    QString name = "无";
    switch (n) {
    case BING:
        name = "兵";
        break;

    case PAO:
        name = "炮";
        break;

    case CHE:
        name = "车";
        break;

    case MA:
        name = "马";
        break;

    case XIANG:
        name = "象";
        break;

    case SHI:
        name = "士";
        break;

    case JIANG:
        name = "将";
        break;

    default:
        break;
    }
    return name;
}


void ChessWin::initAllConnection()
{
    connect(view, &CustomGraphicsView::clicked, this, &ChessWin::handleClick);
    connect(regretPb, &QPushButton::clicked, this, &ChessWin::handlePbRegret);
    connect(surrenderPb, &QPushButton::clicked, this, &ChessWin::handlePbSurrender);
    connect(exitPb, &QPushButton::clicked, this, &ChessWin::handlePbExit);
    connect(reStartGamePb,&QPushButton::clicked,this,&ChessWin::handleRestartGame);

    // 连接窗口的关闭事件到自定义的槽函数
}

HanziChess::HanziChess(int x,int y,int pla,int name,QGraphicsTextItem *text = nullptr,QGraphicsEllipseItem *circle = nullptr)
    :Dot(x,y,pla,name),text(text),circle(circle){}

HanziChess::~HanziChess(){}

