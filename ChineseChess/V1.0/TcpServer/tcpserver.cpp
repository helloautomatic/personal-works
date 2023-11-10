#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QString>

TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);

    loadConfig();

    myTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    QFile file(":/server.config");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray byteData = file.readAll();
        QString strData = byteData.toStdString().c_str();
        file.close();

        strData.replace("\r\n"," ");

        QStringList strListData = strData.split(" ");

        m_strIP = strListData.at(0);
        m_usPort = strListData.at(1).toUShort();

        qDebug()<<QString("监听信息输出----> IP地址:%1  端口:%2").arg(m_strIP).arg(m_usPort);

    }else{
        QMessageBox::critical(this,"打开配置文件","打开配置文件失败!");
    }
}






