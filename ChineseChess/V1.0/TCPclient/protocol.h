#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdlib.h>
// #include <unistd.h>
#include <io.h>
#include <process.h>
#include <string.h>

#define REGIST_OK "regist ok"
#define REGIST_FAILED "regist failed : name exited"
#define LOGIN_OK "login ok"
#define LOGIN_FAILED "login failed : name or password not existed"
#define SEARCH_USR_NO "NoUser"
#define SEARCH_USR_ONLINE "Online"
#define SEARCH_USR_OFFLINE "Offline"
#define UNKNOW_ERROR "Unknow error"
#define EXIST_FRIEND "friend exist"
#define ADD_FRIEND_OFFLINE "usr offline"
#define ADD_FRIEND_NOT_EXIST "usr not exist"

typedef unsigned int uint;
struct PDU{
    uint uiPDULen;  // 总的协议单元的大小
    uint uiMsgType;  // 消息类型
    char caData[64];
    uint uiMsgLen;
    int caMsg[];
};

PDU * mkPDU(uint uiMsgLen);

PDU * mkPDU(uint uiMsgLen);
enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN = 0,
    ENUM_MSG_TYPE_REGIST_REQUEST,  // 注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND,  // 注册回复

    ENUM_MSG_TYPE_LOGIN_REQUEST,  // 登陆请求
    ENUM_MSG_TYPE_LOGIN_RESPOND,  // 登陆回复

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,  // 在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,  // 在线用户回复

    ENUM_MSG_TYPE_SEARCH_USER_REQUEST,  // 搜索请求
    ENUM_MSG_TYPE_SEARCH_USER_RESPOND,  // 搜索回复

    ENUM_MSG_TYPE_ADD_FRIEND_REQUEST,  // 加好友请求
    ENUM_MSG_TYPE_ADD_FRIEND_RESPOND,  // 加好友回复

    ENUM_MSG_TYPE_ADD_FRIEND_AGREE,  // 同意添加好友
    ENUM_MSG_TYPE_ADD_FRIEND_REFUSED,  // 拒绝添加好友

    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,  // 刷新好友列表请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,  // 刷新好友列表回复


    ENUM_MSG_TYPE_OPPONENT_MOVE_CHESS_REQUEST,

    ENUM_MSG_TYPE_OPPONENT_SURRENDER_REQUEST,  //对手投降请求
    ENUM_MSG_TYPE_OPPONENT_SURRENDER_RESPOND,  // 对手投降回复


    ENUM_MSG_TYPE_AGREE_OPPONENT_SURRENDER_RESPOND,  // 同意对手投降
    ENUM_MSG_TYPE_REFUSE_OPPONENT_SURRENDER_RESPOND,  // 拒绝对手投降

    ENUM_MSG_TYPE_OFFLINE_REQUEST,  // 用户下线操作

    //    ENUM_MSG_TYPE__REQUEST,
    //    ENUM_MSG_TYPE__RESPOND,
    //    ENUM_MSG_TYPE__REQUEST,
    //    ENUM_MSG_TYPE__RESPOND,
    //    ENUM_MSG_TYPE__REQUEST,
    //    ENUM_MSG_TYPE__RESPOND,





    ENUM_MSG_TYPE_MAX = 0x00ffffff
};

#endif // PROTOCOL_H
