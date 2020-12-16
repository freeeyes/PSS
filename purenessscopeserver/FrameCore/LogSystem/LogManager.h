#ifndef _LOGMANAGER_H
#define _LOGMANAGER_H

//管理日志块的池
//一个使用者提议，对日志采用分级管理。
//也就是日志里面包含了
//使用C11线程队列替换ACE_Task,用现代化的写法
//add by freeeyes

#include <stdio.h>
#include "ILogObject.h"
#include "ILogManager.h"
#include "BaseTask.h"
#include "ThreadQueue.h"

class CLogManager : public ILogManager
{
public:
    CLogManager(void) = default;

    int open();
    int svc(void);
    int Close();

    void Init(int nThreadCount = 1, int nQueueMax = MAX_MSG_THREADQUEUE, uint32 u4MailID = 0);
    int Start();
    int Stop();
    bool IsRun() const;

    int PutLog(shared_ptr<_LogBlockInfo> pLogBlockInfo);
    int RegisterLog(shared_ptr<IServerLogger> pServerLogger);
    int UnRegisterLog() const;

    void SetReset(bool blReset);

    void ResetLogData(uint16 u2LogLevel);

    //对维护接口
    uint32 GetLogCount() const;
    uint32 GetCurrLevel() const;

    uint16 GetLogID(uint16 u2Index) const;
    const char*  GetLogInfoByServerName(uint16 u2LogID) const;
    const char*  GetLogInfoByLogName(uint16 u2LogID) const;
    int    GetLogInfoByLogDisplay(uint16 u2LogID) const;
    uint16 GetLogInfoByLogLevel(uint16 u2LogID) const;

    //对内写日志的接口
    int WriteLog_i(uint16 u2LogType, const string& strLog);

    int WriteToMail_i(uint16 u2LogType, uint16 u2MailID, const string& strTitle, const string& strLog);

    //对外写日志的接口
    int WriteLogBinary(uint16 u2LogType, const string& strText) final;

    int WriteLog_r(uint16 u2LogType, const string& strLog) final;

    int WriteToMail_r(uint16 u2LogType, uint16 u2MailID, const string& strTitle, const string& strLog) final;

private:
    bool Dispose_Queue(shared_ptr<_LogBlockInfo> msg) const;
    int ProcessLog(shared_ptr<_LogBlockInfo> msg) const;

    //关闭消息队列条件变量
    bool                                     m_blRun          = false;               //日志系统是否启动
    bool                                     m_blIsNeedReset  = false;               //日志模块等级升级重置标志
    bool                                     m_blIsMail       = false;               //是否可以发送邮件
    int                                      m_nThreadCount   = 1;                   //记录日志线程个数，目前默认是1
    int                                      m_nQueueMax      = MAX_MSG_THREADQUEUE; //日志线程允许的最大队列个数
    shared_ptr<IServerLogger>                m_pServerLogger = nullptr;              //日志模块指针
    CMessageQueue<shared_ptr<_LogBlockInfo>> m_objThreadQueue;
    std::thread                              m_ttQueue;                              //消息队列线程
};

using AppLogManager = PSS_singleton<CLogManager>;

#endif
