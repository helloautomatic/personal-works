#ifndef OPEWIDGET_H
#define OPEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include "friend.h"
#include "course.h"
#include <QStackedWidget>  // 堆栈窗口
#include "chesswin.h"


class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    void initConnectAll();
    static OpeWidget & getInstance();
    Friend *getFriend();
    void m_setWinName(QString winName);  // 设定窗口的名字
    ChessWin * getChessWin();
    void setChessWinPtrVal(ChessWin * newName);

private:
    QListWidget * m_pListWidget;
    Friend * m_pFriend;
    Course * m_pCousre;
    ChessWin * m_pChessWin;

    QStackedWidget *m_stackWdg;

signals:

};

#endif // OPEWIDGET_H
