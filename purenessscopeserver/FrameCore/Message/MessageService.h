#ifndef _MESSAGESERVICE_H
#define _MESSAGESERVICE_H

#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "BaseTask.h"
#include "Message.h"
#include "MessageManager.h"
#include "LogManager.h"
#include "ThreadInfo.h"
#include "BuffPacket.h"
#include "TimerManager.h"
#include "WorkThreadAI.h"
#include "CommandAccount.h"
#include "MessageDyeingManager.h"
#include "ObjectLru.h"

#if PSS_PLATFORM == PLATFORM_WIN
#include "ProConnectHandler.h"
#include "WindowsCPU.h"
#else
#include "ConnectHandler.h"
#include "LinuxCPU.h"
#endif

enum MESSAGE_SERVICE_THREAD_STATE
{
    THREAD_RUN = 0,               //�߳���������
    THREAD_MODULE_UNLOAD,         //ģ�����أ���Ҫ�߳�֧�ִ˹���
    THREAD_STOP,                  //�߳�ֹͣ
};

//�����߳�ʱ��ģʽ
class CMessageService : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMessageService();

    virtual int handle_signal (int signum,
                               siginfo_t*  = 0,
                               ucontext_t* = 0);

    int open();
    virtual int svc (void);
    int Close();

    void Init(uint32 u4ThreadID, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK, bool blIsCpuAffinity = false);

    bool Start();

    bool PutMessage(CMessage* pMessage);
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex);

    _ThreadInfo* GetThreadInfo();
    bool SaveThreadInfoData(const ACE_Time_Value& tvNow);   //��¼��ǰ�����߳�״̬��Ϣ��־

    void GetAIInfo(_WorkThreadAIInfo& objAIInfo) const;           //�õ����й����̵߳�AI����
    void GetAITO(vecCommandTimeout& objTimeout) const;            //�õ����е�AI��ʱ���ݰ���Ϣ
    void GetAITF(vecCommandTimeout& objTimeout) const;            //�õ����е�AI������ݰ���Ϣ
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);  //����AI

    _CommandData* GetCommandData(uint16 u2CommandID);                      //�õ����������Ϣ
    void GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut);                 //�õ����������Ϣ
    void GetCommandAlertData(vecCommandAlertData& CommandAlertDataList);   //�õ����г����澯��ֵ������
    void SaveCommandDataLog();                                             //�洢ͳ����־
    void SetThreadState(MESSAGE_SERVICE_THREAD_STATE emState);             //�����߳�״̬
    MESSAGE_SERVICE_THREAD_STATE GetThreadState() const;                   //�õ���ǰ�߳�״̬
    THREADSTATE GetStepState() const;                                      //�õ���ǰ���������Ϣ
    uint32 GetUsedMessageCount();                                          //�õ�����ʹ�õ�Message�������

    uint32 GetThreadID() const;

    void CopyMessageManagerList();                                         //��MessageManager�л�������б���

    CMessage* CreateMessage();
    void DeleteMessage(CMessage* pMessage);

    void GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account);  //�õ���ǰ�б�������Ϣ

private:
    bool ProcessMessage(CMessage* pMessage, uint32 u4ThreadID);
    void CloseCommandList();                                                //����ǰ�����б���
    CClientCommandList* GetClientCommandList(uint16 u2CommandID);
    bool DoMessage(const ACE_Time_Value& tvBegin, IMessage* pMessage, uint16& u2CommandID, uint32& u4TimeCost, uint16& u2Count, bool& bDeleteFlag);

    virtual int CloseMsgQueue();

    void UpdateCommandList(const ACE_Message_Block* pmb);        //����ָ���б�
    bool Dispose_Queue();                                  //��������

    uint32                         m_u4ThreadID         = 0;                     //��ǰ�߳�ID
    uint32                         m_u4MaxQueue         = MAX_MSG_THREADQUEUE;   //�߳��������Ϣ�������
    uint32                         m_u4HighMask         = 0;        
    uint32                         m_u4LowMask          = 0;
    uint32                         m_u4WorkQueuePutTime = 0;                     //��ӳ�ʱʱ��
    uint16                         m_u2ThreadTimeOut    = 0;
    bool                           m_blRun              = false;                 //�߳��Ƿ�������
    bool                           m_blIsCpuAffinity    = false;                 //�Ƿ�CPU��

    MESSAGE_SERVICE_THREAD_STATE   m_emThreadState      = THREAD_STOP;           //��ǰ�����߳�״̬

    _ThreadInfo                    m_ThreadInfo;           //��ǰ�߳���Ϣ
    CWorkThreadAI                  m_WorkThreadAI;         //�߳����Ҽ�ص�AI�߼�
    CCommandAccount                m_CommandAccount;       //��ǰ�߳�����ͳ������
    CMessagePool                   m_MessagePool;          //��Ϣ��

    CHashTable<CClientCommandList>                      m_objClientCommandList;  //��ִ�е������б�

    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;
};

//add by freeeyes
//����̹߳����û����Դ������ɸ�ACE_Task��ÿ��Task��Ӧһ���̣߳�һ��Connectidֻ��Ӧһ���̡߳�
class CMessageServiceGroup : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMessageServiceGroup();

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);

    bool Init(uint32 u4ThreadCount = MAX_MSG_THREADCOUNT, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK);
    bool PutMessage(CMessage* pMessage);                                                     //���͵���Ӧ���߳�ȥ����
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex);                                      //������Ϣͬ�����еĹ����߳������
    void Close();

    bool Start();
    CThreadInfoList* GetThreadInfo();
    uint32 GetUsedMessageCount();

    uint32 GetWorkThreadCount() const;                                                        //�õ���ǰ�����̵߳�����
    uint32 GetWorkThreadIDByIndex(uint32 u4Index);                                            //�õ�ָ�������̵߳��߳�ID
    void GetWorkThreadAIInfo(vecWorkThreadAIInfo& objvecWorkThreadAIInfo);                    //�õ��̹߳���AI������Ϣ
    void GetAITO(vecCommandTimeout& objTimeout);                                              //�õ����е�AI��ʱ���ݰ���Ϣ
    void GetAITF(vecCommandTimeout& objTimeout);                                              //�õ����е�AI������ݰ���Ϣ
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);  //����AI

    void GetCommandData(uint16 u2CommandID, _CommandData& objCommandData) const;              //���ָ������ͳ����Ϣ
    void GetFlowInfo(uint32& u4FlowIn, uint32& u4FlowOut);                                    //���ָ�����������Ϣ

    void GetCommandAlertData(vecCommandAlertData& CommandAlertDataList);                      //�õ����г����澯��ֵ������
    void SaveCommandDataLog();                                                                //�洢ͳ����־

    CMessage* CreateMessage(uint32 u4ConnectID, EM_CONNECT_IO_TYPE u1PacketType);             //�����߳��л�ȡһ��Message����
    void DeleteMessage(uint32 u4ConnectID, CMessage* pMessage);                               //�����߳��л���һ��Message����

    void CopyMessageManagerList();                                                            //��MessageManager�л�������б���

    void AddDyringIP(const char* pClientIP, uint16 u2MaxCount);                               //Ⱦɫָ����IP
    bool AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount);                             //Ⱦɫָ����CommandID
    void GetDyeingCommand(vec_Dyeing_Command_list& objList) const;                            //��õ�ǰ����Ⱦɫ״̬

    void GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account);                  //�õ���ǰ�б�������Ϣ
    bool CheckCPUAndMemory(bool blTest = false);                                              //���CPU���ڴ�

private:
    bool StartTimer();
    bool KillTimer();

    bool CheckWorkThread(const ACE_Time_Value& tvNow);                                                 //������еĹ����߳�״̬
    bool CheckPacketParsePool() const;                                                                 //�������ʹ�õ���Ϣ��������
    bool CheckPlugInState() const;                                                                     //������в��״̬
    int32 GetWorkThreadID(uint32 u4ConnectID, EM_CONNECT_IO_TYPE u1PacketType);                        //���ݲ������ͺ�ConnectID������Ǹ������߳�ID

	typedef vector<CMessageService*> vecMessageService;
	vecMessageService                                   m_vecMessageService;
    uint32                                              m_u4MaxQueue           = 0;              //�߳��������Ϣ�������
    uint32                                              m_u4HighMask           = 0;              //�̸߳�ˮλ
    uint32                                              m_u4LowMask            = 0;              //�̵߳�ˮλ
    uint32                                              m_u4TimerID            = 0;              //��ʱ��ID
    uint16                                              m_u2ThreadTimeCheck    = 0;              //�߳��Լ�ʱ��
    uint16                                              m_u2CurrThreadID       = 0;              //��ǰ��ѯ�����߳�ID
    uint16                                              m_u2CpuNumber          = 0;              //��ǰCPU�ĺ���
    CThreadInfoList                                     m_objAllThreadInfo;        //��ǰ�����߳���Ϣ
    CMessageDyeingManager                               m_objMessageDyeingManager; //����Ⱦɫ��
    ACE_Recursive_Thread_Mutex                          m_ThreadLock;              //�����̲߳������߳�������֤CurrThreadID����������
};

typedef ACE_Singleton<CMessageServiceGroup, ACE_Null_Mutex> App_MessageServiceGroup;
#endif
