#ifndef MYTCTSERVER_H
#define MYTCTSERVER_H
#include <QTcpServer>
#include <QList>
#include "mytcpsocket.h"

class myTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    myTcpServer();
    static myTcpServer & getInstance();
    void incomingConnection(qintptr socketDescriptor) override; 
    void initConnect();
    void resend(const char*pername,PDU*pdu);
    QList<MyTcpSocket*> getTcpSocketList();

private:
    QList<MyTcpSocket*> m_tcpSocketList;

public slots:
    void deleteSocket(MyTcpSocket * socketAddr);

};

#endif // MYTCTSERVER_H
