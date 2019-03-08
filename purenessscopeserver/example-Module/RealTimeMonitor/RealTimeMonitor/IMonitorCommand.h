#ifndef IMONITORCOMMAND_H
#define IMONITORCOMMAND_H

#include "define.h"
#include "IObject.h"

//虚类，用户服务器间通讯回调使用
//add by freeeyes
#define MONITER_SERVER_ID          1

#define COMMAND_MONITOR_LOGIN      0x2001    //发送注册命令
#define COMMAND_MONITOR_DATA       0x2002    //发送心跳当前数据吞吐命令
#define COMMAND_MONITOR_LOGIN_ACK  0x8001    //注册命令回应
#define COMMAND_MONITOR_DATA_ACK   0x8002    //当前数据吞吐命令回应

enum EM_MONITOR_STATE
{
    MONITOR_STATE_DISCONNECT = 0,   //未连接到远程监控服务器
    MONITOR_STATE_CONNECT,          //已连接到远程监控服务器
    MONITOR_STATE_LOGINED,          //远程监控服务器已验证
};

class IMonitorCommand
{
public:
    virtual ~IMonitorCommand() {};

    virtual void SetState(EM_MONITOR_STATE em_monitor_state) = 0;

};

#endif
