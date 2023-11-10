#include "mytcpsocket.h"
#include <QtDebug>
#include "opedb.h"
#include "protocol.h"
#include "mytcpserver.h"

MyTcpSocket::MyTcpSocket()
{
    initConnectionsAll();
}


/* 用于连接所有的槽函数 */
void MyTcpSocket::initConnectionsAll()
{
    connect(this,SIGNAL(readyRead()),this, SLOT(recvMsg()));

    // 只要是客户端断开的连接，就会触发这个信号。
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffline()));
}

QString MyTcpSocket::getName()
{
    return m_strName;
}

MyTcpSocket::~MyTcpSocket()
{

}


/* 槽函数 */
void MyTcpSocket::recvMsg()
{
    // 接收数据处理函数
    qDebug()<<"获取当前对象可读取的字节流"<< this->bytesAvailable();  // 获取当前对象可读取的字节流
    uint uiPDULen = 0;
    this->read((char*)&uiPDULen,sizeof(uint));  // 将第一个字节读入到uiPDULen中
    uint uiMsgLen = uiPDULen - sizeof(PDU);  // PDU的大小并不包括其中的数据指针
    PDU *pdu = mkPDU(uiMsgLen);  // 这里的pdu需要free掉
    this->read((char*)pdu + sizeof(uint),uiPDULen);

    // 根据数据类型处理请求
    switch(pdu->uiMsgType){
        case ENUM_MSG_TYPE_REGIST_REQUEST:{
            char caName[32] = {'\0'};
            char caPwd[32] = {'\0'};
            strncpy(caName,pdu->caData,32);
            strncpy(caPwd,pdu->caData + 32,32);
            bool ret = OpeDb::getInstance().handRegist(caName,caPwd);  // 数据库中处理注册的函数

            PDU * resPdu = mkPDU(0);  // 产生用于回复的pdu
            resPdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPOND;

            if(ret){
                strcpy(resPdu->caData,REGIST_OK);
                qDebug()<<"strcpy(resPdu->caData,REGIST_OK);";
            }else{
                strcpy(resPdu->caData,REGIST_FAILED);
                qDebug()<<"strcpy(resPdu->caData,REGIST_FAILED);";
            }

            // 数据发送
            write((char*)resPdu,resPdu->uiPDULen);
            free(resPdu);
            resPdu = NULL;

            break;
            }


        case ENUM_MSG_TYPE_LOGIN_REQUEST:{
            char caName[32] = {'\0'};
            char caPwd[32] = {'\0'};
            strncpy(caName,pdu->caData,32);
            strncpy(caPwd,pdu->caData + 32,32);
            bool ret = OpeDb::getInstance().handLongin(caName,caPwd);

            PDU * resPdu = mkPDU(0);  // 产生用于回复的pdu
            resPdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;

            if(ret){
                strcpy(resPdu->caData,LOGIN_OK);
                m_strName = caName;  // 登陆成功的时候，将名字记录下
            }else{
                strcpy(resPdu->caData,LOGIN_FAILED);
            }

            // 数据发送
            write((char*)resPdu,resPdu->uiPDULen);
            free(resPdu);
            resPdu = NULL;
            break;
        }

        case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST:{
            QStringList ret = OpeDb::getInstance().handAllOnline();

            uiMsgLen = ret.size()*32;  // uiMsgLen事先在该函数的头部定义好了
            PDU * resPDU = mkPDU(uiMsgLen);
            resPDU->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;

            for(int i = 0;i<ret.size();i++){
                memcpy((char*)(resPDU->caMsg) + i*32,
                       ret.at(i).toStdString().c_str(),
                       ret.at(i).size());
                qDebug()<<"写入到caMsg中的内容"<<(char*)(resPDU->caMsg);
            }

            // 数据发送
            write((char*)resPDU,resPDU->uiPDULen);
            free(resPDU);
            resPDU = NULL;
            break;
        }

        case ENUM_MSG_TYPE_SEARCH_USER_REQUEST:{
            int ret = OpeDb::getInstance().handSearchUsr(pdu->caData);
            qDebug()<<"数据库的查询结果是:"<<ret;
            PDU * resPdu = mkPDU(0);
            resPdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_RESPOND;

            if(-1 == ret){
                qDebug()<<SEARCH_USR_NO;
                strcpy(resPdu->caData,SEARCH_USR_NO);
            }else if(1 == ret){
                qDebug()<<SEARCH_USR_ONLINE;
                strcpy(resPdu->caData,SEARCH_USR_ONLINE);
            }else if(0 == ret){
                qDebug()<<SEARCH_USR_OFFLINE;
                strcpy(resPdu->caData,SEARCH_USR_OFFLINE);
            }

            // 数据发送
            write((char*)resPdu,resPdu->uiPDULen);
            free(resPdu);
            resPdu = NULL;

            break;
        }

        // 这个case是用来处理，客户端发来的请求对弈的
        // 这个请求对弈是指定某个人，与之下棋
        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
            char caPername[32] = {'\0'};  // 被请求添加好友的名字
            char caName[32] = {'\0'};
            strncpy(caPername,pdu->caData,32);
            strncpy(caName,pdu->caData + 32,32);
            qDebug()<<"接收到的加好友信息:"<<caPername<<","<<caName;

            // 返回值为1 表示在线
            int ret = OpeDb::getInstance().handAddFriend(caPername,caName);

            PDU* respdu = NULL;
            if(-1 == ret)
            {
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                strcpy(respdu->caData,UNKNOW_ERROR);

                // 数据发送
                this->write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
            }
            else if(0 == ret)
            {
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                strcpy(respdu->caData,EXIST_FRIEND);

                // 数据发送
                this->write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
            }
            else if(1 == ret)
            {
                // resend内部完成了数据发送
                myTcpServer::getInstance().resend(caPername,pdu);  // 在resend函数中，可以通过caPername找到这个人的socket，然后转发
            }
            else if(2 == ret)
            {
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                strcpy(respdu->caData,ADD_FRIEND_OFFLINE);

                // 数据发送
                this->write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
            }
            else if(3 == ret)
            {
                respdu = mkPDU(0);
                respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
                strcpy(respdu->caData,ADD_FRIEND_NOT_EXIST);

                // 数据发送
                this->write((char*)respdu,respdu->uiPDULen);
                free(respdu);
                respdu = NULL;
            }
            break;
        }

        case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:{  // 同意对方请求的对弈
            char caName[32] = {'\0'};
            char toName[32] = {'\0'};
            strncpy(caName,pdu->caData,32);  // 前32个字节是请求者的名字
            strncpy(toName,pdu->caData + 32,32);  // 后32个字节是要通知的名字
            qDebug()<<caName<<"同意"<<toName<<"添加好友请求";

            // 修改数据库中的表，friend，把正在对弈的两个人添加进去
            OpeDb::getInstance().handAddPlaying(caName,toName);

            // 通知对方
            PDU* respdu = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
            memcpy(respdu->caData,caName,32);
            memcpy(respdu->caData + 32,toName,32);

            // 数据发送
            myTcpServer::getInstance().resend(toName,respdu);
            free(respdu);
            respdu = NULL;

            break;
        }

        case ENUM_MSG_TYPE_ADD_FRIEND_REFUSED:{  // 拒绝对方请求的对弈
            char caName[32] = {'\0'};
            char toName[32] = {'\0'};
            strncpy(caName,pdu->caData,32);  // 前32个字节是请求者的名字
            strncpy(toName,pdu->caData + 32,32);  // 后32个字节是要通知的名字
            qDebug()<<caName<<"拒绝了"<<toName<<"添加好友请求";

            // 通知对方
            PDU* respdu = mkPDU(0);
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSED;
            memcpy(respdu->caData,caName,32);
            memcpy(respdu->caData + 32,toName,32);

            // 数据发送
            myTcpServer::getInstance().resend(toName,respdu);
            free(respdu);
            respdu = NULL;
            break;
        }

        case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST:{
            char name[32] = {'\0'};
            strncpy(name,pdu->caData,32);
            OpeDb::getInstance().handFlushFriend(name);
            break;
        }

        case ENUM_MSG_TYPE_OPPONENT_MOVE_CHESS_REQUEST:{
            char name[32] = {'\0'};
            char toName[32] = {'\0'};
            int dotData[8] = {'\0'};
            strncpy(name,pdu->caData,32);  // 将前32个字节取出来
            strncpy(toName,pdu->caData + 32,32);
            memcpy(dotData,pdu->caMsg,pdu->uiMsgLen);

            qDebug()<<"收到了来自"<<name<<"的行棋信息,"<<"需要将信息发送给"<<toName;
            qDebug()<<"输出坐标信息";
            for(uint i = 0;i < uint(pdu->uiMsgLen / 4);i++){
                qDebug()<<dotData[i]<<",";
            }

            pdu->uiMsgType = ENUM_MSG_TYPE_OPPONENT_MOVE_CHESS_REQUEST;
            // 根据toName找到toName对应的socket，然后将信息发送过去
            myTcpServer::getInstance().resend(toName,pdu);

            break;
        }

        case ENUM_MSG_TYPE_OPPONENT_SURRENDER_REQUEST:{
            char name1[32] = {'\0'};
            char name2[32] = {'\0'};
            memcpy(name1,pdu->caData,32);
            memcpy(name2,pdu->caData + 32,32);
            qDebug()<<"收到了"<<name1<<"向"<<name2<<"投降";

            pdu->uiMsgType = ENUM_MSG_TYPE_OPPONENT_SURRENDER_RESPOND;  // 回复投降的请求

            myTcpServer::getInstance().resend(name2,pdu);  // 发送消息


            // 根据name2将消息转发出去

            break;
        }

        case ENUM_MSG_TYPE_AGREE_OPPONENT_SURRENDER_RESPOND:{
            char name1[32] = {'\0'};
            char name2[32] = {'\0'};
            memcpy(name1,pdu->caData,32);
            memcpy(name2,pdu->caData + 32,32);
            pdu->uiMsgType = ENUM_MSG_TYPE_AGREE_OPPONENT_SURRENDER_RESPOND;
            myTcpServer::getInstance().resend(name1,pdu);
            break;
        }

        case ENUM_MSG_TYPE_REFUSE_OPPONENT_SURRENDER_RESPOND:{
            char name1[32] = {'\0'};
            char name2[32] = {'\0'};
            memcpy(name1,pdu->caData,32);
            memcpy(name2,pdu->caData + 32,32);
            pdu->uiMsgType = ENUM_MSG_TYPE_REFUSE_OPPONENT_SURRENDER_RESPOND;
            myTcpServer::getInstance().resend(name1,pdu);
            break;
        }

        default: {
        qDebug()<<"服务端的default被执行了";
            break;
        }
    };
    free(pdu);
}

// 该槽函数由 disconnected() 信号触发，
// disconnected() 信号由某个客户端断开连接的时候触发
void MyTcpSocket::clientOffline()
{
    qDebug()<<m_strName<<"下线了";


    char name1[32] = {'\0'};
    char name2[32] = {'\0'};

    // 在数据库中找到需要通知的那个人的名字
    QString nameTemp = OpeDb::getInstance().handOffline(m_strName.toStdString().c_str());
    strcpy(name1,nameTemp.toStdString().c_str());  // 将要被通知的人的名字
    strcpy(name2,this->getName().toStdString().c_str());  // 下线人的名字

    if(!nameTemp.isEmpty()){
        // 根据这个名字在客户端列表中找到对应的socket，然后使用该socket将信息准确的发出去。
        QList<MyTcpSocket*>::iterator iter = myTcpServer::getInstance().getTcpSocketList().begin();
        for(;iter != myTcpServer::getInstance().getTcpSocketList().end();iter ++ ){
            if(QString(name1) == (*iter)->getName()){

                PDU* sendPdu = mkPDU(0);
                sendPdu->uiMsgType = ENUM_MSG_TYPE_OFFLINE_REQUEST;  // 用户下线操作
                memcpy(sendPdu->caData,name1,32);
                memcpy(sendPdu->caData + 32,name2,32);
                (*iter)->write((char*)sendPdu,sendPdu->uiMsgType);
                break;
            }
        }
    }

    emit offLine(this);  // 发送信号,为了删除  客户端列表  中的相应内容
    // 注意!!!!!! 当emit发送了信号后，程序不会跳转到槽函数处执行（不会等待槽函数执行），
    // 而是当信号发送完毕后，程序继续往下执行
}
