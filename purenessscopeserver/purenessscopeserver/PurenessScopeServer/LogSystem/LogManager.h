#ifndef _LOGMANAGER_H
#define _LOGMANAGER_H

#include "ace/Task.h"
#include <stdio.h>
#include <stdarg.h>
#include "define.h"
#include "ILogObject.h"
#include "ILogManager.h"

//管理日志块的池
//一个使用者提议，对日志采用分级管理。
//也就是日志里面包含了
//add by freeeyes

class CLogBlockPool
{
public:
    CLogBlockPool();
    ~CLogBlockPool();

    void Init(uint32 u4BlockSize, uint32 u4PoolCount);
    void Close();

    _LogBlockInfo* GetLogBlockInfo();                       //得到一个空余的日志块
    void ReturnBlockInfo(_LogBlockInfo* pLogBlockInfo);     //归还一个用完的日志块

    uint32 GetBlockSize();

private:
    _LogBlockInfo* m_pLogBlockInfo;       //日志池
    uint32         m_u4MaxBlockSize;      //日志池单块最大上限
    uint32         m_u4PoolCount;         //日志池中的日志块个数
    uint32         m_u4CurrIndex;         //日志池中当前已用到的日志块ID
};

class CLogManager : public ACE_Task<ACE_MT_SYNCH>, public ILogManager
{
public:
    CLogManager(void);
    ~CLogManager(void);

    virtual int open (void* args = 0);
    virtual int svc(void);
    int Close();

    void Init(int nThreadCount = 1, int nQueueMax = MAX_MSG_THREADQUEUE, uint32 u4MailID = 0);
    int Start();
    int Stop();
    bool IsRun();

    int PutLog(_LogBlockInfo* pLogBlockInfo);
    int RegisterLog(CServerLogger* pServerLogger);
    int UnRegisterLog();

    void SetReset(bool blReset);

    void ResetLogData(uint16 u2LogLevel);

    //对维护接口
    uint32 GetLogCount();
    uint32 GetCurrLevel();

    uint16 GetLogID(uint16 u2Index);
    char*  GetLogInfoByServerName(uint16 u2LogID);
    char*  GetLogInfoByLogName(uint16 u2LogID);
    int    GetLogInfoByLogDisplay(uint16 u2LogID);
    uint16 GetLogInfoByLogLevel(uint16 u2LogID);

    //对外写日志的接口
    int WriteLog(int nLogType, const char* fmt, ...);

    int WriteLogBinary(int nLogType, const char* pData, int nLen);

    int WriteToMail(int nLogType, uint32 u4MailID, char* pTitle, const char* fmt, ...);

private:
    int ProcessLog(_LogBlockInfo* pLogBlockInfo);
    virtual int CloseMsgQueue();

private:
    //关闭消息队列条件变量
    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;
private:
    bool                              m_blRun;                    //日志系统是否启动
    bool                              m_blIsNeedReset;            //日志模块等级升级重置标志
    bool                              m_blIsMail;                 //是否可以发送邮件
    int                               m_nThreadCount;             //记录日志线程个数，目前默认是1
    int                               m_nQueueMax;                //日志线程允许的最大队列个数
    CLogBlockPool                     m_objLogBlockPool;          //日志块池
    ACE_Recursive_Thread_Mutex        m_Logger_Mutex;             //线程锁
    CServerLogger*                    m_pServerLogger;            //日志模块指针
};

typedef ACE_Singleton<CLogManager, ACE_Recursive_Thread_Mutex> AppLogManager;

#endif
