#include "mytcpserver.h"
#include <QDebug>

myTcpServer::myTcpServer(){}

/* 单例模式,无论调用多少次都是返回的同一个对象 */
myTcpServer & myTcpServer::getInstance()
{
    static myTcpServer instance;
    return instance;
}

/*
 * 一旦有客户端连接,就会调用该函数
 * 该函数重写自其父类的 incomingConnection
 * 该函数是用来处理所有的客户端连接的信息
 * 每当有新的客户端连接的时候，都会触发该槽函数
 * 其中参数 qintptr socketDescriptor 是由触发该槽函数的信号传递过来的 它是新连接客户端的socket描述符
*/
void myTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"新的客户端连接";
    MyTcpSocket * pTcpSocket = new MyTcpSocket;  // 创建一个自己封装的TcpSocket

    pTcpSocket->setSocketDescriptor(socketDescriptor);  // 将槽函数接收到的TcpSocket传递给自己封装的
    m_tcpSocketList.append(pTcpSocket);

    connect(pTcpSocket,SIGNAL(offLine(MyTcpSocket*)),this,SLOT(deleteSocket(MyTcpSocket*)));
    // 当发送信号的对象，或者拥有槽对象再被delete的时候，需要断开 connect嘛？
    // 如果不断开会有什么影响呢？
}

void myTcpServer::initConnect()
{

}

void myTcpServer::resend(const char *pername, PDU *pdu)
{
    if(NULL == pername || NULL == pdu)
    {
        return;
    }

    QString QstrPername = QString(pername);
    for(int i = 0;i<m_tcpSocketList.size();i++)  // 通过pername找到相应的socket
    {
        if(QstrPername == m_tcpSocketList.at(i)->getName())
        {
            m_tcpSocketList.at(i)->write((char*)pdu,pdu->uiPDULen);  // 发送数据
            break;
        }
    }
}

QList<MyTcpSocket *> myTcpServer::getTcpSocketList()
{
    return m_tcpSocketList;
}


/* 槽函数
 * 该函数用于处理当有某个客户端下线的时候，它应该采取的动作
 * 同样的，它的参数 MyTcpSocket *socketAddr 来自于触发信号
*/
void myTcpServer::deleteSocket(MyTcpSocket *socketAddr)
{
    qDebug()<<"用户下线后处理，服务器处理用户列表";
    QList<MyTcpSocket*>::iterator iter = m_tcpSocketList.begin();
    for(;iter != m_tcpSocketList.end();iter ++ ){
        if(socketAddr == *iter){

            //delete *iter;  // 这句代码不用加了，反正也没几个用户

            (*iter)->deleteLater();
            *iter = NULL;
            m_tcpSocketList.erase(iter);
            break;
        }
    }

    qDebug()<<"函数deleteSocket执行结束";
}
