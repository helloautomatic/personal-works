#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include "protocol.h"

namespace Ui {
class Online;
}

class Online : public QWidget
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = nullptr);
    ~Online();
    void showUsr(PDU*pdu);
    void connectAll();

public slots:
    void on_m_pShowOnlineUsr_clicked();

private slots:
    void on_addFriend_pb_clicked();

    void on_m_pSerachUsr_clicked();

    void on_m_pPrivateChat_clicked();

signals:
    void flushOnlineList();

private:
    Ui::Online *ui;
    QString m_strSearchName;
};

#endif // ONLINE_H
