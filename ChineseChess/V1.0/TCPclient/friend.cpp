#include "friend.h"
#include "tcpclient.h"
#include <QInputDialog>
#include <QMessageBox>

Friend::Friend(QWidget *parent) : QWidget(parent)
{
    m_pOnline = new Online;
    QVBoxLayout * pMain = new QVBoxLayout;
    pMain->addWidget(m_pOnline);


    connectAll();
    setLayout(pMain);
}

void Friend::connectAll()
{
}

void Friend::showAllOnlineUsr(PDU *pdu)
{
    if(NULL == pdu){
        return;
    }

    m_pOnline->showUsr(pdu);
}

Online *Friend::getMyOnline()
{
    return m_pOnline;
}


void Friend::showOnline()
{
    if(m_pOnline->isHidden()){
        m_pOnline->show();

        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;

        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }
    else{
        m_pOnline->hide();
    }
}

void Friend::searchUsr()
{
    m_strSearchName = QInputDialog::getText(this,"搜索对手","用户名");
    if(!m_strSearchName.isEmpty()){
        qDebug()<< m_strSearchName;
        PDU*pdu = mkPDU(0);
        memcpy(pdu->caData, m_strSearchName.toStdString().c_str(), m_strSearchName.size());
        pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_REQUEST;
        TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu = NULL;
    }else{
        QMessageBox::warning(this,"用户搜索","用户名不能为空");
    }
}

void Friend::flushFriend()
{
    QString strName = TcpClient::getInstance().getLoginName();
    PDU * pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),strName.size());

    TcpClient::getInstance().getTcpSocket().write((char*)pdu,pdu->uiPDULen);  // 发送数据
    pdu = NULL;
}
