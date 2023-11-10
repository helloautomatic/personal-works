#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QTcpSocket>
extern int playlingFlag;

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void loadConfig();
    static TcpClient & getInstance();
    QTcpSocket & getTcpSocket();

public slots:
    void showConnectState();
    void recvMsg();
    QString getLoginName();

protected:
    void initConnected();

private slots:
    void on_button_register_clicked();
    void on_button_login_clicked();
    void on_button_lotout_clicked();

private:
    Ui::TcpClient *ui;
    QString m_strIP;
    quint16 m_usPort;

    QTcpSocket m_tcpSocket;

    QString loginName;  // 登录的用户名，就是自己的名字

};
#endif // TCPCLIENT_H
