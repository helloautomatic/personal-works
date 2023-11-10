#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "protocol.h"
#include "opewidget.h"
#include "chesswin.h"
#include <string.h>


int playlingFlag = 0;  // 该标志位用于标志是否正在下棋


TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);

    loadConfig();  // 加载配置文件
    initConnected();  // 连接所有信号和槽函数

    /*连接服务器*/
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);

    this->setWindowTitle("用户登录  未连接服务器");

}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray byteData = file.readAll();
        QString strData = byteData.toStdString().c_str();
        file.close();

        strData.replace("\r\n"," ");

        QStringList strListData = strData.split(" ");

        m_strIP = strListData.at(0);
        m_usPort = strListData.at(1).toUShort();

    }else{
        QMessageBox::critical(this,"open config","open config failed!");
    }
}

TcpClient &TcpClient::getInstance()
{
    static TcpClient instance;
    return instance;
}

QTcpSocket &TcpClient::getTcpSocket()
{
    return m_tcpSocket;
}

/* 槽函数
 * 客户端连接服务器成功的时候会调用该函数 */
void TcpClient::showConnectState()
{
    this->setWindowTitle("用户登录  已连接服务器");
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}

void TcpClient::recvMsg()
{
    // 接收数据处理函数
    qDebug()<<"获取当前对象可读取的字节流"<< m_tcpSocket.bytesAvailable();  // 获取当前对象可读取的字节流
    uint uiPDULen = 0;
    m_tcpSocket.read((char*)&uiPDULen,sizeof(uint));  // 将第一个字节读入到uiPDULen中
    uint uiMsgLen = uiPDULen - sizeof(PDU);  // PDU的大小并不包括其中的数据指针
    PDU *pdu = mkPDU(uiMsgLen);
    m_tcpSocket.read((char*)pdu + sizeof(uint),uiPDULen);

    // 根据数据类型处理请求
    switch(pdu->uiMsgType){
        case ENUM_MSG_TYPE_REGIST_RESPOND:{
            if(0 == strcmp(pdu->caData,REGIST_OK)){
                QMessageBox::information(this,"注册","注册成功");
            }else if(0 == strcmp(pdu->caData,REGIST_FAILED)){
                QMessageBox::warning(this,"注册","注册失败");
            }
            break;
        }


        case ENUM_MSG_TYPE_LOGIN_RESPOND:{
            if(0 == strcmp(pdu->caData,LOGIN_OK)){
                QMessageBox::information(this,"登陆","登陆成功");
                OpeWidget::getInstance().m_setWinName(this->getLoginName());  // 显示登录后的界面名字
                OpeWidget::getInstance().show();  // 登陆成功，打开登陆成功后的界面
                emit OpeWidget::getInstance().getFriend()->getMyOnline()->flushOnlineList();

                this->hide();  // 隐藏登陆界面
            }else if(0 == strcmp(pdu->caData,LOGIN_FAILED)){
                QMessageBox::warning(this,"登陆","登陆失败,用户名或者密码不正确");
            }
            break;
        }

        case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:{
            OpeWidget::getInstance().getFriend()->getMyOnline()->showUsr(pdu);

            break;
        }

        case ENUM_MSG_TYPE_SEARCH_USER_RESPOND:{
            qDebug()<<"查询结果为:"<<pdu->caData;
            if(0 == strcmp(SEARCH_USR_NO,pdu->caData)){
                qDebug()<<SEARCH_USR_NO;
                QMessageBox::information(this,"提示",QString("%1:not exist").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
            }else if(0 == strcmp(SEARCH_USR_ONLINE,pdu->caData)){
                qDebug()<<SEARCH_USR_ONLINE;
                QMessageBox::information(this,"提示",QString("%1:online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
            }else if(0 == strcmp(SEARCH_USR_OFFLINE,pdu->caData)){
                qDebug()<<SEARCH_USR_OFFLINE;
                QMessageBox::information(this,"提示",QString("%1:offline").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
            }
            break;
        }

        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{  // 其他人想要添加你为好友的请求
            char caName[32] = {'\0'};
            strncpy(caName,pdu->caData + 32,32);
            int ret = QMessageBox::information(this,"对弈请求",QString("%1 想要和你对弈").arg(caName),
                                     QMessageBox::Yes,QMessageBox::No);

            PDU* respdu = mkPDU(0);
            memcpy(respdu->caData,pdu->caData,32);  // 第一个是自己的名字，所以要取前32个字节
            memcpy(respdu->caData + 32,pdu->caData + 32,32);  // 第二个是对方的名字，所以要 + 32个字节
            if(QMessageBox::Yes == ret)
            {
                respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;  // 同意添加好友

                // 你同意了对方的请求，所以这里 = 1
                playlingFlag = 1;



                // TODO 打开对弈窗口，开始下棋
                QMessageBox::information(this,"开始对弈通知","你已经同意了对方的对弈请求，开始下棋吧");
                OpeWidget::getInstance().setChessWinPtrVal(new ChessWin(CHESS_COLOR_BLACK));
                OpeWidget::getInstance().getChessWin()->show();
                OpeWidget::getInstance().hide();

                // 设定对手的名字
                OpeWidget::getInstance().getChessWin()->setOpponetName(caName);

                // 行棋标志位设置为 0，因为是别人请求你的
                isMovableFlag = 0;

            }
            else  // 包括把客户端程序关掉的情况，就是没有说同意和不同意，但是直接关掉了窗口
            {
                respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSED;
            }

            // 发送消息
            m_tcpSocket.write((char*)respdu,respdu->uiPDULen);  // 发送消息
            free(respdu);
            respdu = NULL;

            break;
        }

        case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:{
            QMessageBox::information(this,"啊啊啊啊啊啊啊啊",pdu->caData);

            break;
        }


        // 对方同意你的添加请求，请求者通常是红方，为先下棋的一方
        case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:{
            QMessageBox::information(this,"同意请求","对方同意了你的请求，开始下棋吧");
            OpeWidget::getInstance().setChessWinPtrVal(new ChessWin(CHESS_COLOR_RED));
            OpeWidget::getInstance().getChessWin()->show();
            OpeWidget::getInstance().hide();

            // 后32个字节为被通知的一方
            char opponentName[32] =  {'\0'};
            memcpy(opponentName,pdu->caData,32);
            OpeWidget::getInstance().getChessWin()->setOpponetName(QString(opponentName));

            // 行棋方标志位置1，因为是你请求别人的
            isMovableFlag = 1;

            // = 1，表示正在下棋
            playlingFlag = 1;


            break;
        }


        // 对方拒绝你的添加请求
        case ENUM_MSG_TYPE_ADD_FRIEND_REFUSED:{
        QMessageBox::information(this,"拒绝请求","对方拒绝和你下棋");
            break;
        }

        // 处理来自服务器的行棋命令
        case ENUM_MSG_TYPE_OPPONENT_MOVE_CHESS_REQUEST:{
            char name[32] = {'\0'};  // 对手的名字
            int dotData[8] = {'\0'};
            strncpy(name,pdu->caData,32);  // 将前32个字节取出来
            memcpy(dotData,pdu->caMsg,pdu->uiMsgLen);

            qDebug()<<"收到了来自"<<name<<"的行棋信息";
            qDebug()<<"输出坐标信息";
            for(uint i = 0;i < uint(pdu->uiMsgLen / 4);i++){
                qDebug()<<dotData[i]<<",";
            }

            // 根据对手发过来的行棋信息，刷新本地的棋盘
            OpeWidget::getInstance().getChessWin()->changeDot_vevtor_opponent(dotData[0],dotData[1],dotData[4],dotData[5]);
            OpeWidget::getInstance().getChessWin()->freshChessboard();  // 刷新棋盘

            // 本地行棋标志位设置为1
            isMovableFlag = 1;

            break;
        }

        case ENUM_MSG_TYPE_OPPONENT_SURRENDER_RESPOND:{
            char name1[32] = {'\0'};
            char name2[32] = {'\0'};
            memcpy(name1,pdu->caData,32);
            memcpy(name2,pdu->caData + 32,32);
            qDebug()<<"收到了"<<name1<<"的投降请求";

            // 弹出box，询问是否接受投降，根据询问结果进行下一步的操作
            int ret = QMessageBox::information(this,"投降请求",QString("%1 请求投降").arg(name1),
                                     QMessageBox::Yes,QMessageBox::No);

            if (QMessageBox::Yes == ret){  // 接受对方的投降,游戏结束，关闭下棋界面
                pdu->uiMsgType = ENUM_MSG_TYPE_AGREE_OPPONENT_SURRENDER_RESPOND;  // 同意对手投降
                m_tcpSocket.write((char*)pdu,pdu->uiPDULen);  // 发送数据
                qDebug()<<"你  同意  了对方的投降请求";
                // 关闭窗口，游戏结束的一些相关操作
//                OpeWidget::getInstance().getChessWin()->close();  // 这个窗口关闭会导致投降的请求方无法收到回复信息，为神马？？？？
//                OpeWidget::getInstance().setChessWinPtrVal(nullptr);
            }
            else{// 游戏继续，并通知对方，拒绝它的投降，包括点击了No和直接关闭窗口
                pdu->uiMsgType = ENUM_MSG_TYPE_REFUSE_OPPONENT_SURRENDER_RESPOND;  // 拒绝对手投降
                m_tcpSocket.write((char*)pdu,pdu->uiPDULen);  // 发送数据
                qDebug()<<"你  拒绝  了对方的投降请求";
            }

            break;
        }

        case ENUM_MSG_TYPE_AGREE_OPPONENT_SURRENDER_RESPOND:{  // 对方同意了你的投降请求
            QMessageBox::information(this,"提示","对方同意了你的投降请求");

            // 关闭窗口，处理游戏结束的一些工作
//            OpeWidget::getInstance().getChessWin()->close();  // 关闭窗口，不知道会不会导致错误
//            OpeWidget::getInstance().setChessWinPtrVal(nullptr);

            break;
        }

        case ENUM_MSG_TYPE_REFUSE_OPPONENT_SURRENDER_RESPOND:{  // 对方拒绝了你的投降请求
            QMessageBox::information(this,"提示","对方拒绝你的投降请求");
            break;
        }

        case ENUM_MSG_TYPE_OFFLINE_REQUEST:{
            QMessageBox::information(this,"提示","对方下线了!");
            OpeWidget::getInstance().getChessWin()->close();  // 关闭窗口

            playlingFlag = 0;  // 设定为0，因为对方下线了，所以处于空闲状态
            break;
        }

        default: {
            break;
        }
    };

    free(pdu);
    pdu = NULL;
}

QString TcpClient::getLoginName()
{
    return loginName;
}

// 用于连接所有的信号和槽
void TcpClient::initConnected()
{
    /*客户端连接服务器成果后,会自动触发一个信号,该信号被连接到槽函数 showConnectState
        用于显示连接成功
    */
    connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(showConnectState()));

    /*
        每当socket接受到数据时，触发消息接受函数 recvMsg()
        */
    connect(&m_tcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
}

void TcpClient::on_button_register_clicked()
{
    QString strName = ui->line_usrName->text();
    QString strPwd = ui->line_pwd->text();
    if(!strName.isEmpty() && !strPwd.isEmpty()){
        PDU* pdu = mkPDU(0);  // 因为没有使用caMsg字段，所以为0
        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData + 32,strPwd.toStdString().c_str(),32);

        // 数据发送
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }else{
        QMessageBox::critical(this,"注册","注册失败:用户名或者密码不能为空");
    }
}

void TcpClient::on_button_login_clicked()
{
    QString strName = ui->line_usrName->text();
    QString strPwd = ui->line_pwd->text();
    if(!strName.isEmpty() && !strPwd.isEmpty()){
        PDU* pdu = mkPDU(0);  // 因为没有使用caMsg字段，所以为0
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        strncpy(pdu->caData,strName.toStdString().c_str(),32);
        strncpy(pdu->caData + 32,strPwd.toStdString().c_str(),32);

        loginName = strName;

        // 数据发送
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }else{
        QMessageBox::critical(this,"登陆","登陆失败:用户名或者密码为空");
    }
}

void TcpClient::on_button_lotout_clicked()
{
    QMessageBox::information(this,"提示","该功能尚未开发");
}
