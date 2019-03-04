#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_AUTOTEST_HEAD         0x5000   //带头返回数据包
#define COMMAND_AUTOTEST_NOHEAD       0x5001   //不带头数据包
#define COMMAND_AUTOTEST_HEADBUFF     0x5002   //带头缓冲数据包
#define COMMAND_AUTOTEST_NOHEADBUFF   0x5003   //不带头缓冲数据包
#define COMMAND_AUTOTEST_LOGDATA      0x5004   //测试记录日志
#define COMMAND_AUTOTEST_WORKTIMEOUT  0x5005   //测试工作线程超时重建
#define COMMAND_AUTOTEST_WORKAI       0x5006   //测试workAI

#define COMMAND_AUTOTEST_RETUEN_HEAD         0xf000
#define COMMAND_AUTOTEST_RETUEN_NOHEAD       0xf001
#define COMMAND_AUTOTEST_RETUEN_HEADBUFF     0xf002
#define COMMAND_AUTOTEST_RETUEN_NOHEADBUFF   0xf003
#define COMMAND_AUTOTEST_RETUEN_LOGDATA      0xf004
#define COMMAND_AUTOTEST_RETURN_WORKTIMEOUT  0xf005
#define COMMAND_AUTOTEST_RETURN_WORKAI       0xf006

using namespace std;

class CBaseCommand : public CClientCommand
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
    void SetServerObject(CServerObject* pServerObject);

private:
    bool Do_Head(IMessage* pMessage);             //返回包含头的数据包
    bool Do_NoHead(IMessage* pMessage);           //返回不包含头的数据包
    bool Do_HeadBuff(IMessage* pMessage);         //返回包含缓冲的数据包头的数据包
    bool Do_NoHeadBuff(IMessage* pMessage);       //返回不包含数据包头的数据包
    bool Do_LogData(IMessage* pMessage);          //记录日志信息
    bool Do_SleepWorkThread(IMessage* pMessage);  //触发工作线程重启
    bool Do_TimeoutWordThread(IMessage* pMessage);//触发工作线程超时


private:
    CServerObject* m_pServerObject;
    int            m_nCount;
};

