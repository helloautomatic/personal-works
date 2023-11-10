#include "opewidget.h"

OpeWidget::OpeWidget(QWidget *parent) : QWidget(parent)
{
    m_pListWidget = new QListWidget(this);
    m_pListWidget->addItem("中国象棋小游戏");
    m_pListWidget->addItem("教程");

    m_pFriend = new Friend;
    m_pCousre = new Course;
    m_pChessWin = nullptr;  // CheWin的new时机需要根据程序交互来确定

    m_stackWdg = new QStackedWidget;
    m_stackWdg->addWidget(m_pFriend);
    m_stackWdg->addWidget(m_pCousre);

    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pListWidget);
    pMain->addWidget(m_stackWdg);

    setLayout(pMain);

    /*
        mQListWidget 和 QStackedWidget关联起来的方法是通过以下信号和槽函数的连接实现的
        connect(m_pListWidget,SIGNAL(currentRowChanged(int)),m_stackWdg,SLOT(setCurrentIndex(int)));
    */

    initConnectAll();

}

void OpeWidget::initConnectAll()
{
    connect(m_pListWidget,SIGNAL(currentRowChanged(int)),m_stackWdg,SLOT(setCurrentIndex(int)));
}

OpeWidget &OpeWidget::getInstance()
{
    static OpeWidget instance;
    return instance;
}

Friend *OpeWidget::getFriend()
{
    return m_pFriend;
}

void OpeWidget::m_setWinName(QString winName)
{
    this->setWindowTitle(winName);
}

ChessWin *OpeWidget::getChessWin()
{
    return this->m_pChessWin;
}

void OpeWidget::setChessWinPtrVal(ChessWin *newName)
{
    m_pChessWin = newName;
}
