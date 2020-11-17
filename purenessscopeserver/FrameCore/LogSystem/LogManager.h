#ifndef _LOGMANAGER_H
#define _LOGMANAGER_H

#include <stdio.h>
#include "ILogObject.h"
#include "ILogManager.h"
#include "BaseTask.h"

//������־��ĳ�
//һ��ʹ�������飬����־���÷ּ�����
//Ҳ������־���������
//add by freeeyes

class CLogBlockPool
{
public:
    CLogBlockPool();

    void Init(uint32 u4BlockSize, uint32 u4PoolCount);
    void Close();

    _LogBlockInfo* GetLogBlockInfo();                       //�õ�һ���������־��
    void ReturnBlockInfo(_LogBlockInfo* pLogBlockInfo) const;     //�黹һ���������־��

    uint32 GetBlockSize() const;

private:
    _LogBlockInfo* m_pLogBlockInfo    = nullptr;      //��־��
    uint32         m_u4MaxBlockSize   = 0;         //��־�ص����������
    uint32         m_u4PoolCount      = 0;         //��־���е���־�����
    uint32         m_u4CurrIndex      = 0;         //��־���е�ǰ���õ�����־��ID
};

class CLogManager : public ACE_Task<ACE_MT_SYNCH>, public ILogManager
{
public:
    CLogManager(void);

    int open ();
    virtual int svc(void);
    int Close();

    void Init(int nThreadCount = 1, int nQueueMax = MAX_MSG_THREADQUEUE, uint32 u4MailID = 0);
    int Start();
    int Stop();
    bool IsRun() const;

    int PutLog(_LogBlockInfo* pLogBlockInfo);
    int RegisterLog(shared_ptr<IServerLogger> pServerLogger);
    int UnRegisterLog();

    void SetReset(bool blReset);

    void ResetLogData(uint16 u2LogLevel);

    //��ά���ӿ�
    uint32 GetLogCount();
    uint32 GetCurrLevel();

    uint16 GetLogID(uint16 u2Index);
    const char*  GetLogInfoByServerName(uint16 u2LogID);
    const char*  GetLogInfoByLogName(uint16 u2LogID);
    int    GetLogInfoByLogDisplay(uint16 u2LogID);
    uint16 GetLogInfoByLogLevel(uint16 u2LogID);

    //����д��־�Ľӿ�
    template <class... Args>
    int WriteLog_i(int nLogType, const char* fmt, Args&& ... args)
    {
        //����־��������ҵ�һ��������־��
        int nRet = 0;

        m_Logger_Mutex.acquire();
        _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();

        if (nullptr != pLogBlockInfo)
        {
            ACE_OS::snprintf(pLogBlockInfo->m_pBlock, m_objLogBlockPool.GetBlockSize() - 1, fmt, convert(std::forward<Args>(args))...);
            nRet = Update_Log_Block(nLogType, nullptr, nullptr, pLogBlockInfo);
        }

        m_Logger_Mutex.release();
        return nRet;
    };

    template <class... Args>
    int WriteToMail_i(int nLogType, uint16 u2MailID, const char* pTitle, const char* fmt, Args&& ... args)
    {
        int nRet = 0;
        m_Logger_Mutex.acquire();
        _LogBlockInfo* pLogBlockInfo = m_objLogBlockPool.GetLogBlockInfo();

        if (nullptr != pLogBlockInfo)
        {
            ACE_OS::snprintf(pLogBlockInfo->m_pBlock, m_objLogBlockPool.GetBlockSize() - 1, fmt, convert(std::forward<Args>(args))...);
            nRet = Update_Log_Block(nLogType, &u2MailID, pTitle, pLogBlockInfo);
        }

        m_Logger_Mutex.release();
        return nRet;
    };

    //����д��־�Ľӿ�
    virtual int WriteLogBinary(int nLogType, const char* pData, int nLen);

    virtual int WriteLog_r(int nLogType, const char* fmt, uint32 u4Len);

    virtual int WriteToMail_r(int nLogType, uint16 u2MailID, const char* pTitle, const char* fmt, uint32 u4Len);

private:
    bool Dispose_Queue();
    int ProcessLog(_LogBlockInfo* pLogBlockInfo);
    virtual int CloseMsgQueue();
    int Update_Log_Block(int nLogType, const uint16* pMailID, const char* pTitle, _LogBlockInfo* pLogBlockInfo);

    //�ر���Ϣ������������
    ACE_Thread_Mutex                  m_mutex;
    ACE_Condition<ACE_Thread_Mutex>   m_cond;
    bool                              m_blRun          = false;               //��־ϵͳ�Ƿ�����
    bool                              m_blIsNeedReset  = false;               //��־ģ��ȼ��������ñ�־
    bool                              m_blIsMail       = false;               //�Ƿ���Է����ʼ�
    int                               m_nThreadCount   = 1;                   //��¼��־�̸߳�����ĿǰĬ����1
    int                               m_nQueueMax      = MAX_MSG_THREADQUEUE; //��־�߳�����������и���
    CLogBlockPool                     m_objLogBlockPool;                      //��־���
    ACE_Recursive_Thread_Mutex        m_Logger_Mutex;                         //�߳���
    shared_ptr<IServerLogger>         m_pServerLogger = nullptr;                 //��־ģ��ָ��
};

typedef ACE_Singleton<CLogManager, ACE_Recursive_Thread_Mutex> AppLogManager;

#endif
