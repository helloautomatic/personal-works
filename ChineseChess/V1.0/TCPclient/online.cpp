#include "online.h"
#include "ui_online.h"
#include <QDebug>
#include "tcpclient.h"
#include <QInputDialog>
#include <QMessageBox>

Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
    connectAll();
}

Online::~Online()
{
    delete ui;
}

void Online::showUsr(PDU *pdu)
{
    if(NULL == pdu){
        return;
    }

    char caTmp[32];
    ui->listWidget->clear();  // 清除好友列表
    uint uiSize = pdu->uiMsgLen/32;
    for(uint i=0;i<uiSize;i++){
        memcpy(caTmp,(char*)(pdu->caMsg) + i*32,32);
        ui->listWidget->addItem(caTmp);
    }
}

void Online::connectAll()
{
    // 连接
    connect(this,SIGNAL(flushOnlineList()),this,SLOT(on_m_pShowOnlineUsr_clicked()));
}

void Online::on_addFriend_pb_clicked()
{
    QListWidgetItem *pItem = ui->listWidget->currentItem();
    if(nullptr == pItem){
        QMessageBox::information(this,"提示","请选择一个你要与之对弈的玩家");
        return;
    }

    QString strPerUsrName = pItem->text();  // 加好友的名字
    qDebug()<<"strPerUsrName的值为:"<< strPerUsrName;

    QString strLoginName = TcpClient::getInstance().getLoginName();  // 自己的名字
    qDebug()<<"加好友的名字:"<<strPerUsrName;
    qDebug()<<"自己的名字:"<<strLoginName;

    if(strPerUsrName == strLoginName){
        QMessageBox::information(this,"提示","你不能选择和自己对弈");
        return;
    }

    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData, strPerUsrName.toStdString().c_str(), strPerUsrName.size());
    memcpy(pdu->caData + 32, strLoginName.toStdString().c_str(),strLoginName.size());

    // 因为每个tcpClient和每个tcpSocket是一一对应的所以，可以这么写
    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);  // 发送消息
    free(pdu);
    pdu = NULL;
}

// 请求所有的在线玩家
void Online::on_m_pShowOnlineUsr_clicked()
{
    QString strName = TcpClient::getInstance().getLoginName();
    PDU * pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());

    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);  // 发送数据
    pdu = NULL;
    qDebug()<<"点击了刷新列表";
}

void Online::on_m_pSerachUsr_clicked()
{
    bool isOkCliecked = false;
    QInputDialog * myDilog = new QInputDialog;
    myDilog->setParent(this);
    m_strSearchName = myDilog->getText(this,"搜索玩家","用户名",QLineEdit::Normal,"",&isOkCliecked);

    if(true == isOkCliecked){  // 点击了ok按钮
        if(m_strSearchName.isEmpty()){
            QMessageBox::warning(this,"用户搜索","用户名不能为空");
        }
        else
        {
            qDebug()<< m_strSearchName;
            PDU*pdu = mkPDU(0);
            memcpy(pdu->caData, m_strSearchName.toStdString().c_str(), m_strSearchName.size());
            pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_REQUEST;
            TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
            free(pdu);
            pdu = NULL;
        }
    }
    myDilog->deleteLater();
}

void Online::on_m_pPrivateChat_clicked()
{
    QMessageBox::information(this,"提示","该功能尚未开发!");
    qDebug()<<"点击了私聊";
}
