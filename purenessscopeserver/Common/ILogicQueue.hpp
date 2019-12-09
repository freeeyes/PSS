#ifndef ILOGICQUEUE_H
#define ILOGICQUEUE_H

#include "define.h"
#include <string>

//add 与工作线程无关的业务消息队列的逻辑业务类,必须继承后实现 liuruiqi
enum ThreadReturn
{
    THREAD_Task_Exit = 0,    //当前线程退出
    THREAD_Task_Finish,      //当前线程任务完成
};

enum ThreadError
{
    THREAD_Error_Timeout = 0,    //线程死锁
};

class ILogicQueue
{
public:
    //初始化必填 参数:逻辑ID, 描述, 检测时间(秒)
    ILogicQueue(uint32 u4LogicThreadID, uint32 u4Timeout, std::string& strDesc) : m_u4LogicThreadID(u4LogicThreadID),
        m_u4Timeout(u4Timeout),
        m_strDesc(strDesc)
    {
    };

    virtual ~ILogicQueue() {};
public:
    //获取线程ID
    uint32 GetLogicThreadID()
    {
        return m_u4LogicThreadID;
    };

    //超时
    void SetTimeOut(uint32 u4Timeout)
    {
        m_u4Timeout = u4Timeout;
    };

    uint32 GetTimeOut()
    {
        return m_u4Timeout;
    };

    //描述
    std::string GetDescriptor()
    {
        return m_strDesc;
    };

public:
    //初始化内容
    virtual bool Init() = 0;
    //执行逻辑
    virtual ThreadReturn Run(int nMessage, void* arg) = 0;
    //错误处理
    virtual uint32 Error(int nErrorID) = 0;
    //退出善后
    virtual void Exit() = 0;
private:
    ILogicQueue& operator=(const ILogicQueue&) = delete;
private:
    uint32      m_u4LogicThreadID;
    uint32      m_u4Timeout;
    std::string m_strDesc;
};


#endif // ILOGIC_H