#include "tcpclient.h"

#include <QApplication>
#include "opewidget.h"
#include "online.h"
#include "friend.h"
#include "chesswin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    TcpClient::getInstance().show();  // 单例模式，无论什么时候都是一个。

    //ChessWin::getInstance().show();

//    OpeWidget w;
//    w.show();

//    Online w;
//    w.show();

//    Friend w;
//    w.show();
    return a.exec();
}
