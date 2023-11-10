#ifndef CHESSWIN_H
#define CHESSWIN_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "moverules.h"
#include <QDebug>
#include <QMouseEvent>
#include <customgraphicsview.h>
#include <QPushButton>
#include <QHBoxLayout>

#define GRID_SIZE 80  // 棋盘格子的长度
#define CIRCLE_SIZE 30  // 棋子圆的半径
#define CHESS_COLOR_RED "red"  // 红色棋子
#define CHESS_COLOR_BLACK "black"  // 黑色棋子


extern QVector<Dot*> cliDotVec;  // 鼠标点击坐标暂存vector,其长度不能超过 2
extern int isMoveLocal;  // 本地行棋还是对手行棋
extern int playerColor;

extern int isMovableFlag;  // 能否行棋标志位

enum ServerEnum{

    /*命令类型,这里的命令从字符 'A' 的ASCII值开始*/
    CMD_MOVE = 'A',  // 行棋坐标
    CMD_BEGIN,  // 请求开始游戏
    CMD_HUIQI,  // 悔棋
    CMD_TOUXIANG,  // 投降
    CMD_EXIT,  // 退出
    CMD_PLAYER_BEGIN,

    /*玩家状态*/
    WAITING,
    PLAYING,

    /*对方行棋还是自己行棋的开关*/
    MOVE_LOCAL,
    MOVE_OPPONENT,

    /*行棋禁止标志*/
    MOVABLE_OK,
    MOVABLE_NO,
    MOVABLE_UNKNOWN,

    /*玩家颜色*/
    PLAYER_COLOR_RED,  // 红色
    PLAYER_COLOR_BLACK,  // 黑色
    COLOR_NONE,  // 未知

    /*玩家是否开始*/
    PLAYER_BEGIN,
    PLAYER_NOT_BEGIN
};


/* 用于刷新棋盘的类,它继承自Dot
 * 并在Dot的基础上添加了两个成员 QGraphicsTextItem 和 QGraphicsEllipseItem
 * 主要用于记录每个被棋子占据的Dot的汉字和圆*/
class HanziChess: public Dot{
public:
    QGraphicsTextItem *text;
    QGraphicsEllipseItem *circle;
    HanziChess(int x,int y,int pla,int name,QGraphicsTextItem *text,QGraphicsEllipseItem *circle);
    ~HanziChess();
};


/*和dot_vector类似,将hanziChess_vector也声明为全局变量*/
extern  QVector<HanziChess*> hanziChess_vector;

namespace Ui {
class ChessWin;
}

class ChessWin : public QWidget
{
    Q_OBJECT

public:
    explicit ChessWin(QString cheColor, QWidget *parent = nullptr);
    virtual ~ChessWin();  // 这里的析构函数最好设定为虚析构函数，防止出错

    void initAllConnection();
    void drawOneChess(qreal centerX, qreal centerY, qreal radius,
                      const QString& chineseText,HanziChess *hanziche,
                      QString cheColor);
    void initDrawChessboard();
    void freshDotHanziVector();
    void freshChessboard();
    static QString getChessName(int n);
    bool changeDot_vector(int x,int y);
    void changeDot_vevtor_opponent(int x1,int y1,int x2,int y2);
    void initGame();
    void setOpponetName(QString newName);
    QString getOpponentName();

    // 单例模式
    static ChessWin & getInstance();

public:
    QHBoxLayout *innerLayout = nullptr;
    QPushButton *regretPb = nullptr;
    QPushButton *surrenderPb = nullptr;
    QPushButton *exitPb = nullptr;
    QPushButton *reStartGamePb = nullptr;

public slots:
    void handlePbRegret();
    void handlePbSurrender();
    void handlePbExit();
    void handleClick(const QPointF &pos);
    void handleRestartGame();

private:
    Ui::ChessWin *ui;
    CustomGraphicsView *view;
    QGraphicsScene *scene;
    ChessBoard *virtualChessboard;
    ChessBoard* chessboard;
    QVector<HanziChess*> hanziChess_vector;
    QVBoxLayout *outerLayout;

    QString chessColor;

    QString opppnentName;
};

#endif
