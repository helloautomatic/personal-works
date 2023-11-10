#ifndef OPEDB_H
#define OPEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>

class OpeDb : public QObject
{
    Q_OBJECT
public:
    explicit OpeDb(QObject *parent = nullptr);
    static OpeDb& getInstance();
    void init();
    ~OpeDb();

    bool handRegist(const char* name,const char*pwd);
    bool handLongin(const char*name,const char*pwd);
    QString handOffline(const char*name);
    QStringList handAllOnline();
    int handSearchUsr(const char*name);
    int handAddFriend(const char* pername,const char*name);
    QStringList handFlushFriend(const char *name);
    void handAddPlaying(const char *name1,const char* name2);
    void handSendMoveChess(const char *name1,const char*name2);

signals:

private:
    QSqlDatabase m_db;  // 连接数据库
};

#endif // OPEDB_H
