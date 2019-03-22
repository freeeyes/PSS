#ifndef _ILOGICTHREAD_H
#define _ILOGICTHREAD_H

#include <stdio.h>

enum ThreadReturn
{
    THREAD_Task_Exit = 0,    //当前线程退出
    THREAD_Task_Finish,      //当前线程任务完成
};

enum ThreadError
{
    THREAD_Error_Timeout = 0,    //线程死锁
};

//线程创建函数(参数为线程ID)
typedef void(*ThreadInit)(int);

//线程消息处理函数(参数为线程ID, 消息ID, 内容指针)
typedef ThreadReturn(*ThreadCallbackLogic)(int, int, void*);

//线程错误回调函数(线程ID, 错误ID)
typedef void(*ThreadErrorLogic)(int, int);

//线程退出函数
typedef void(*ThreadExit)(int);

class ILogicThreadManager
{
public:
    virtual ~ILogicThreadManager() {}

    //创建逻辑线程
    virtual int CreateLogicThread(int nLogicThreadID,
                                  int nTimeout,
                                  ThreadInit thread_init,
                                  ThreadCallbackLogic thread_callback_logic,
                                  ThreadErrorLogic thread_callback_error,
                                  ThreadExit thread_exit)  = 0;

    //关闭逻辑线程
    virtual int KillLogicThread(int nLogicThreadID) = 0;

    //添加消息和逻辑线程的映射关系
    virtual int MessageMappingLogicThread(int nLogicThreadID, int nMessageID) = 0;

    //发送线程消息
    virtual int SendLogicThreadMessage(int nMessageID, void* arg) = 0;
};

int ILogicThreadManager::MessageMappingLogicThread(int nLogicThreadID, int nMessageID)
{
    return 0;
}

#endif
