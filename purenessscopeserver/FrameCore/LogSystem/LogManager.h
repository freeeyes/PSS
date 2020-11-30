#ifndef _LOGMANAGER_H
#define _LOGMANAGER_H

//������־��ĳ�
//һ��ʹ�������飬����־���÷ּ�����
//Ҳ������־���������
//ʹ��C11�̶߳����滻ACE_Task,���ִ�����д��
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

    //��ά���ӿ�
    uint32 GetLogCount() const;
    uint32 GetCurrLevel() const;

    uint16 GetLogID(uint16 u2Index) const;
    const char*  GetLogInfoByServerName(uint16 u2LogID) const;
    const char*  GetLogInfoByLogName(uint16 u2LogID) const;
    int    GetLogInfoByLogDisplay(uint16 u2LogID) const;
    uint16 GetLogInfoByLogLevel(uint16 u2LogID) const;

    //����д��־�Ľӿ�
    int WriteLog_i(uint16 u2LogType, string strLog);

    int WriteToMail_i(uint16 u2LogType, uint16 u2MailID, string strTitle, string strLog);

    //����д��־�Ľӿ�
    int WriteLogBinary(uint16 u2LogType, string strLog) final;

    int WriteLog_r(uint16 u2LogType, string strLog) final;

    int WriteToMail_r(uint16 u2LogType, uint16 u2MailID, string strTitle, string strLog) final;

private:
    bool Dispose_Queue(shared_ptr<_LogBlockInfo> msg) const;
    int ProcessLog(shared_ptr<_LogBlockInfo> msg) const;

    //�ر���Ϣ������������
    bool                                     m_blRun          = false;               //��־ϵͳ�Ƿ�����
    bool                                     m_blIsNeedReset  = false;               //��־ģ��ȼ��������ñ�־
    bool                                     m_blIsMail       = false;               //�Ƿ���Է����ʼ�
    int                                      m_nThreadCount   = 1;                   //��¼��־�̸߳�����ĿǰĬ����1
    int                                      m_nQueueMax      = MAX_MSG_THREADQUEUE; //��־�߳�����������и���
    shared_ptr<IServerLogger>                m_pServerLogger = nullptr;              //��־ģ��ָ��
    CMessageQueue<shared_ptr<_LogBlockInfo>> m_objThreadQueue;
    std::thread                              m_ttQueue;                              //��Ϣ�����߳�
};

using AppLogManager = ACE_Singleton<CLogManager, ACE_Recursive_Thread_Mutex>;

#endif
