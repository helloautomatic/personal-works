#include "opedb.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QtDebug>

OpeDb::OpeDb(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

OpeDb &OpeDb::getInstance()
{
    static OpeDb instance;
    return instance;
}

void OpeDb::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("D:\\Applications\\Qt\\QtProjects\\ChessV2\\myDatabase\\test1.db");
    if(m_db.open()){
        QSqlQuery query;
        query.exec("select * from usrInfo");
        while(query.next()){
            qDebug()<<query.value(0).toLongLong()
                    <<query.value(1).toString()
                    <<query.value(2).toString();
            }
    }else{
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
    }
}

OpeDb::~OpeDb()
{
    m_db.close();  // 关闭数据库
}

bool OpeDb::handRegist(const char *name, const char *pwd)
{
    // 参数检查
    if(NULL == name || NULL == pwd){
        qDebug()<<"参数检查不合格";
        return false;
    }

    QString data = QString("insert into usrInfo(name,pwd) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery query;

    bool queryExe = query.exec(data);
    if(!queryExe){
        qDebug()<<"注册 查询语句 执行失败";
    }
    return queryExe;  // SQL语句执行失败会返回一个bool
}

bool OpeDb::handLongin(const char *name, const char *pwd)
{
    // 参数检查
    if(NULL == name || NULL == pwd){
        qDebug()<<"参数检查不合格";
        return false;
    }

    QString data =
            QString("select * from usrInfo where name = \'%1\' and pwd = \'%2\' and online = 0").arg(name).arg(pwd);
    QSqlQuery query;
    bool queryExe = query.exec(data);
    if(!queryExe){
        qDebug()<<"登陆 查询语句 执行失败";
    }

    queryExe = query.next();
    if(queryExe){
        data = QString("update usrInfo set online = 1 where name = \'%1\' and pwd = \'%2\' ").arg(name).arg(pwd);
        QSqlQuery query;
        query.exec(data);
        return true;
    }else{
        return false;
    }
    return queryExe;
}


QString OpeDb::handOffline(const char *name)
{
    // 参数检查，name为下线者的名字
    if(NULL == name){
        qDebug()<<"参数检查不合格";
    }

    // 根据下线者的名字找到其对手的名字
    QString data = QString("select name from usrInfo where id = "
                           "(select id from friend where friendId = (select id from usrInfo where name = \'%1\')) or "
                           "id = (select friendId from friend where id = (select id from usrInfo where name = \'%2\'))")
                            .arg(name).arg(name);
    QSqlQuery query;
    bool queryExe = query.exec(data);
    if(!queryExe){
        qDebug()<<"下线后查找对手名字的操作 数据表usrInfo中 查询语句 执行失败";
    }

    query.next();
    QString tempReturnName = query.value(0).toString();


    // 下线后，修改表 usrInfo 中的内容
    data = QString("update usrInfo set online = 0 where name = \'%1\'").arg(name);
    queryExe = query.exec(data);
    if(!queryExe){
        qDebug()<<"下线操作 数据表usrInfo中 查询语句 执行失败";
    }

    // 根据 name 在 friend 表中找到与之下棋的那个人的名字

    // 下线后，修改表 friend 中的内容
    data = QString("delete from friend where id = (select id from usrInfo where name = \'%1\') or friendId = (select id from usrInfo where name = \'%2\')")
            .arg(name).arg(name);
    queryExe = query.exec(data);

    if(!queryExe){
        qDebug()<<"下线操作 数据表friend中 查询语句 执行失败";
    }

    return tempReturnName;
}

QStringList OpeDb::handAllOnline()
{
    QString data = QString("select name from usrInfo where online = 1");
    QSqlQuery query;
    bool queryExe = query.exec(data);
    if(!queryExe){
        qDebug()<<"查找在先用户 查询语句 执行失败";
    }

    QStringList result;
    result.clear();

    while(query.next()){
        result.append(query.value(0).toString());
    }

    return result;
}


/*
-1 表示不存在这个用户
1  表示存在这个用户
0  表示存在这个用户，但是这个用户不在线
*/
int OpeDb::handSearchUsr(const char *name)
{
    if(NULL == name){
        return -1;  // 不存在
    }

    qDebug()<<"数据库收到的查询名字是:"<<name;

    QString data = QString("select online from usrInfo where name = \'%1\'").arg(name);
    QSqlQuery query;
    query.exec(data);

    if(query.next()){
        int ret = query.value(0).toInt();
        if(ret == 1){
            return 1;
        }else if(ret == 0){
            return 0;
        }
    }else{
        return -1;
    }
}


// 返回值目前有 0，1，2，3
// 这里的加好友的请求就是选择好友进行对战的请求
int OpeDb::handAddFriend(const char *pername, const char *name)
{
    if(NULL == pername || name == NULL){
        return -1;
    }

    QString data =
    QString("select * from friend where (id = (select id from usrInfo where name = \'%1\') and friendId = (select id from usrInfo where name = \'%2\')) or (friendId = (select id from usrInfo where name = \'%3\') and friendId = (select id from usrInfo where name = \'%4\'));")
            .arg(pername).arg(name).arg(name).arg(pername);
    qDebug()<<"执行数据库语句"<<data;
    QSqlQuery query;
    query.exec(data);
    if(query.next()){
        return 0;  // 双方已经是好友
    }
    else // 不是好友，判断是否在线
    {
        data = QString("select online from usrInfo where name = \'%1\'").arg(pername);
        QSqlQuery query;
        query.exec(data);

        if(query.next()){
            int ret = query.value(0).toInt();
            if(ret == 1){
                return 1;   // 在线
            }else if(ret == 0){
                return 2;  // 不在线
            }
        }else{
            return 3;  // 不存在这个人
        }
    }
}

QStringList OpeDb::handFlushFriend(const char *name)
{
    QStringList strFriendList;
    strFriendList.clear();
    if(NULL == name){
        return strFriendList;
    }

    // 先找到请求者的ID，然后到好友列表中，找到它的好友的ID
    QString data = QString("select name from usrInfo where online=1 and id = (select id from friend where friendId = (select id from usrInfo where name = \'%1\'))").arg(name);

    QSqlQuery query;
    query.exec(data);

    while (query.next()) {
        strFriendList.append(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }

    data = QString("select name from usrInfo where online=1 and id = (select friendId from friend where id = (select id from usrInfo where name = \'%1\'))").arg(name);
    query.exec(data);
    while(query.next()){
        strFriendList.append(query.value(0).toString());
        qDebug()<<query.value(0).toString();
    }

    return strFriendList;
}

void OpeDb::handAddPlaying(const char *name1, const char *name2)
{
    // 参数检查
    if(NULL == name1 || NULL == name2){
        qDebug()<<"参数检查不合格";
        return;
    }

    // 往friend数据表中插入正在对弈的双方的id
    QString data = QString("insert into friend(id,friendId) values((select id from usrInfo where name = \'%1\'),(select id from usrInfo where name = \'%2\'))")
            .arg(name1).arg(name2);
    QSqlQuery query;

    bool queryExe = query.exec(data);
    if(!queryExe){
        qDebug()<<"往friend表中插入数据 执行失败";
    }
}

void OpeDb::handSendMoveChess(const char *name1, const char *name2)
{

}
