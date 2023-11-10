#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include <QTcpSocket>
#include "protocol.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    void initConnectionsAll();
    QString getName();
    virtual ~MyTcpSocket();

public slots:
    void recvMsg();
    void clientOffline();

signals:
    // void offLine(MyTcpSocket* socketAddr);
    void offLine(MyTcpSocket* socketAddr);
private:
    QString m_strName;  // 登陆成功的时候，将用户的名字记录下来了
};

#endif // MYTCPSOCKET_H
