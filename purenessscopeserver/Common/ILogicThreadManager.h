#ifndef _ILOGICTHREADMANAGER_H
#define _ILOGICTHREADMANAGER_H

#include <stdio.h>
#include "ILogicQueue.h"

class ILogicThreadManager
{
public:
    virtual ~ILogicThreadManager() {}

    //创建消息队列
    virtual int CreateLogicThread(ILogicQueue* pLogicQueue) = 0;

    //关闭消息队列
    virtual int KillLogicThread(int nLogicThreadID) = 0;

    //添加消息和消息队列的映射关系
    virtual int MessageMappingLogicThread(int nLogicThreadID, int nMessageID) = 0;

    //发送线程消息
    virtual int SendLogicThreadMessage(int nMessageID, void* arg) = 0;
};

#endif
