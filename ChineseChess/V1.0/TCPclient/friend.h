#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include<QTextEdit>
#include<QListWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include "online.h"
#include "protocol.h"

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);
    void connectAll();
    void showAllOnlineUsr(PDU*pdu);

    QString m_strSearchName;

    Online * getMyOnline();

signals:

public slots:
    void showOnline();
    void searchUsr();
    void flushFriend();

private:
    Online *m_pOnline;
};

#endif // FRIEND_H
