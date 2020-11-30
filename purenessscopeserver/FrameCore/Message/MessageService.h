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
#include "MessageDyeingManager.h"
#include "ObjectLru.h"
#include "PerformanceCounter.h"
#include "BuffPacketManager.h"
#include "PacketParsePool.h"

#if PSS_PLATFORM == PLATFORM_WIN
#include "WindowsCPU.h"
#else
#include "LinuxCPU.h"
#endif

enum class MESSAGE_SERVICE_THREAD_STATE
{
    THREAD_RUN = 0,               //�߳���������
    THREAD_MODULE_UNLOAD,         //ģ�����أ���Ҫ�߳�֧�ִ˹���
    THREAD_STOP,                  //�߳�ֹͣ
};

class CWorkThread_Packet_Info
{
public:
    uint8  m_u1ThreadID  = 0;
    uint32 m_u4PacketIn  = 0;
    uint32 m_u4PacketOut = 0;
    uint32 m_u4RecvSize  = 0;
    uint32 m_u4SendSize  = 0;
    
    uint32 m_u4Minute    = 0;    //��ǰʱ�䣬���շ���ͳ��

    void Clear()
    {
        m_u4PacketIn  = 0;
        m_u4PacketOut = 0;
        m_u4RecvSize  = 0;
        m_u4SendSize  = 0;
    }
};

//�����߳����ݰ���������ͳ��
class CWorkThread_Process
{
public:
    CWorkThread_Packet_Info m_objPacketCounter;

    void Clear(uint32 u4NowMinite)
    {
        if (u4NowMinite != m_objPacketCounter.m_u4Minute)
        {
            m_objPacketCounter.Clear();
            m_objPacketCounter.m_u4Minute = u4NowMinite;
        }
    }

    void AddPacketIn(uint32 m_u4PacketIn, const ACE_Time_Value& tvNow)
    {
        auto u4NowMinite = (uint32)(tvNow.sec() / 60);
        Clear(u4NowMinite);
        
        m_objPacketCounter.m_u4PacketIn++;
        m_objPacketCounter.m_u4RecvSize = m_u4PacketIn;
    }

    void AddPacketOut(uint32 m_u4PacketOut, const ACE_Time_Value& tvNow)
    {
        auto u4NowMinite = (uint32)(tvNow.sec() / 60);
        Clear(u4NowMinite);

        m_objPacketCounter.m_u4PacketOut++;
        m_objPacketCounter.m_u4SendSize = m_u4PacketOut;
    }

    CWorkThread_Packet_Info GetCurrInfo(const ACE_Time_Value& tvNow)
    {
        auto u4NowMinite = (uint32)(tvNow.sec() / 60);
        Clear(u4NowMinite);

        return m_objPacketCounter;
    }
};

//�����߳�ʱ��ģʽ
class CMessageService : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMessageService();

    int open();
    int svc (void) final;
    int Close();

    void Init(uint32 u4ThreadID, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK, bool blIsCpuAffinity = false);

    bool Start();

    bool PutMessage(CWorkThreadMessage* pMessage);
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex);

    _ThreadInfo* GetThreadInfo();
    bool SaveThreadInfoData(const ACE_Time_Value& tvNow);   //��¼��ǰ�����߳�״̬��Ϣ��־

    void GetAIInfo(_WorkThreadAIInfo& objAIInfo) const;           //�õ����й����̵߳�AI����
    void GetAITO(vecCommandTimeout& objTimeout) const;            //�õ����е�AI��ʱ���ݰ���Ϣ
    void GetAITF(vecCommandTimeout& objTimeout) const;            //�õ����е�AI������ݰ���Ϣ
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime);  //����AI

    void SetThreadState(MESSAGE_SERVICE_THREAD_STATE emState);             //�����߳�״̬
    MESSAGE_SERVICE_THREAD_STATE GetThreadState() const;                   //�õ���ǰ�߳�״̬
    THREADSTATE GetStepState() const;                                      //�õ���ǰ���������Ϣ
    uint32 GetUsedMessageCount();                                          //�õ�����ʹ�õ�Message�������

    uint32 GetThreadID() const;

    uint32 GetHandlerCount() const;                                        //�õ���ǰ�߳���Handler�ĸ���

    void CopyMessageManagerList();                                         //��MessageManager�л�������б���

    CWorkThreadMessage* CreateMessage();
    void DeleteMessage(CWorkThreadMessage* pMessage);

    void GetFlowPortList(const ACE_Time_Value& tvNow, vector<CWorkThread_Packet_Info>& vec_Port_Data_Account); //�õ���ǰ�б�������Ϣ

    bool Synchronize_SendPostMessage(shared_ptr<CWorkThread_Handler_info> pHandlerInfo, const ACE_Time_Value& tvMessage);
    bool SendPostMessage(const CSendMessageInfo& objSendMessageInfo);          //��������
    bool SendCloseMessage(uint32 u4ConnectID);                                //�ر�ָ������ 

    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                        //�õ��ͻ��˽���˿�
    _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                         //�õ��ͻ��˼����˿�

    void Check_Handler_Recv_Timeout();                                        //����ն�����ʱ�䳬ʱ
    EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);             //�õ���ǰ����״̬ 

    uint32 GetThreadID();

private:
    bool ProcessRecvMessage(CWorkThreadMessage* pMessage, uint32 u4ThreadID); //��������¼�
    bool ProcessSendMessage(CWorkThreadMessage* pMessage, uint32 u4ThreadID); //�������¼�
    bool ProcessSendClose(CWorkThreadMessage* pMessage, uint32 u4ThreadID);   //�������¼�
    bool ProcessSendIsLog(CWorkThreadMessage* pMessage, uint32 u4ThreadID);   //�������¼�
    void CloseCommandList();                                                  //����ǰ�����б���
    shared_ptr<CClientCommandList> GetClientCommandList(uint16 u2CommandID);
    bool DoMessage(IMessage* pMessage, uint16& u2CommandID, uint16& u2Count, bool& bDeleteFlag);

    virtual int CloseMsgQueue();

    void UpdateCommandList(const ACE_Message_Block* pmb);        //����ָ���б�
    bool Dispose_Queue();                                  //��������

    uint32                         m_u4ThreadID         = 0;                     //��ǰ�߳�ID
    uint32                         m_u4MaxQueue         = MAX_MSG_THREADQUEUE;   //�߳��������Ϣ�������
    uint32                         m_u4HighMask         = 0;        
    uint32                         m_u4LowMask          = 0;
    uint32                         m_u4WorkQueuePutTime = 0;                     //��ӳ�ʱʱ��
    uint32                         m_u4MaxRecvWait      = 0;                     //���ȴ�ʱ��  
    uint16                         m_u2ThreadTimeOut    = 0;
    uint8                          m_u1PacketCounter    = 0;                     //�Ƿ�������ͳ����,0�ǹرգ�1�ǿ���
    bool                           m_blRun              = false;                 //�߳��Ƿ�������
    bool                           m_blIsCpuAffinity    = false;                 //�Ƿ�CPU��
    bool                           m_blOverload         = false;                 //�Ƿ����   

    MESSAGE_SERVICE_THREAD_STATE   m_emThreadState      = MESSAGE_SERVICE_THREAD_STATE::THREAD_STOP; //��ǰ�����߳�״̬

    CBuffPacket                    m_objBuffSendPacket;

    _ThreadInfo                    m_ThreadInfo;           //��ǰ�߳���Ϣ
    CWorkThreadAI                  m_WorkThreadAI;         //�߳����Ҽ�ص�AI�߼�
    CWorkThread_Process            m_objWorkThreadProcess; //ͳ���߳�һ���ӵ����ݰ�������

    CDeviceHandlerPool             m_DeviceHandlerPool;    //����� 

    using hashmapClientCommandList = unordered_map<uint16, shared_ptr<CClientCommandList>>;
    using hashmapHandlerInfoList   = unordered_map<uint32, shared_ptr<CWorkThread_Handler_info>>;
    hashmapClientCommandList                            m_objClientCommandList;  //��ִ�е������б�
    hashmapHandlerInfoList                              m_objHandlerList;        //��Ӧ��Handler�б�

    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;
    CPerformanceCounter m_PerformanceCounter;
};

//add by freeeyes
//����̹߳����û����Դ������ɸ�ACE_Task��ÿ��Task��Ӧһ���̣߳�һ��Connectidֻ��Ӧһ���̡߳�
class CMessageServiceGroup : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CMessageServiceGroup();

    int handle_timeout(const ACE_Time_Value& tv, const void* arg) final;

    bool Init(uint32 u4ThreadCount = MAX_MSG_THREADCOUNT, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK);
    bool PutMessage(CWorkThreadMessage* pMessage);                                                     //���͵���Ӧ���߳�ȥ����
    bool PutUpdateCommandMessage(uint32 u4UpdateIndex);                                      //������Ϣͬ�����еĹ����߳������
    void Close();

    bool Start();
    void GetThreadInfo(vector<_ThreadInfo>& vecWorkThreadList) const;
    uint32 GetUsedMessageCount() const;

    uint32 GetWorkThreadCount() const;                                                        //�õ���ǰ�����̵߳�����
    uint32 GetWorkThreadIDByIndex(uint32 u4Index);                                            //�õ�ָ�������̵߳��߳�ID
    void GetWorkThreadAIInfo(vecWorkThreadAIInfo& objvecWorkThreadAIInfo) const;              //�õ��̹߳���AI������Ϣ
    void GetAITO(vecCommandTimeout& objTimeout) const;                                        //�õ����е�AI��ʱ���ݰ���Ϣ
    void GetAITF(vecCommandTimeout& objTimeout) const;                                        //�õ����е�AI������ݰ���Ϣ
    void SetAI(uint8 u1AI, uint32 u4DisposeTime, uint32 u4WTCheckTime, uint32 u4WTStopTime) const;  //����AI

    CWorkThreadMessage* CreateMessage(uint32 u4ConnectID, EM_CONNECT_IO_TYPE u1PacketType);   //�����߳��л�ȡһ��Message����
    void DeleteMessage(CWorkThreadMessage* pMessage);                                         //�����߳��л���һ��Message����

    void CopyMessageManagerList() const;                                                      //��MessageManager�л�������б���

    void AddDyringIP(const char* pClientIP, uint16 u2MaxCount);                               //Ⱦɫָ����IP
    bool AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount);                             //Ⱦɫָ����CommandID
    void GetDyeingCommand(vec_Dyeing_Command_list& objList) const;                            //��õ�ǰ����Ⱦɫ״̬

    void GetFlowPortList(vector<CWorkThread_Packet_Info>& vec_Port_Data_Account) const;       //�õ���ǰ�б�������Ϣ
    bool CheckCPUAndMemory(bool blTest = false);                                              //���CPU���ڴ�

    bool Send_Post_Message(const CSendMessageInfo& objSendMessageInfo);                       //����������Ϣ
    bool Send_Close_Message(uint32 u4ConnectID);                                              //�رտͻ�������
    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                        //�õ�ָ�����ӵĿͻ���ID 
    _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                                         //�õ�������IP��Ϣ
    uint32 GetHandlerCount() const;                                                           //�õ���ǰ��������
    EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);                             //�õ���ǰ����״̬ 

private:
    uint32 GetWorkThreadID(uint32 u4ConnectID, EM_CONNECT_IO_TYPE emPacketType) const;        //���ݲ������ͺ�ConnectID������Ǹ������߳�ID

    bool StartTimer();
    bool KillTimer();

    bool CheckRecvTimeout() const;                                                            //������еĳ�ʱ����
    bool CheckWorkThread(const ACE_Time_Value& tvNow) const;                                  //������еĹ����߳�״̬
    bool CheckPacketParsePool() const;                                                        //�������ʹ�õ���Ϣ��������
    bool CheckPlugInState() const;                                                            //������в��״̬
   
	using vecMessageService = vector<shared_ptr<CMessageService>>;
	vecMessageService                                   m_vecMessageService;
    uint32                                              m_u4MaxQueue           = 0;              //�߳��������Ϣ�������
    uint32                                              m_u4HighMask           = 0;              //�̸߳�ˮλ
    uint32                                              m_u4LowMask            = 0;              //�̵߳�ˮλ
    uint32                                              m_u4TimerID            = 0;              //��ʱ��ID
    uint16                                              m_u2ThreadTimeCheck    = 0;              //�߳��Լ�ʱ��
    uint16                                              m_u2CurrThreadID       = 0;              //��ǰ��ѯ�����߳�ID
    uint16                                              m_u2CpuNumber          = 0;              //��ǰCPU�ĺ���
    CMessageDyeingManager                               m_objMessageDyeingManager; //����Ⱦɫ��
    ACE_Recursive_Thread_Mutex                          m_ThreadLock;              //�����̲߳������߳�������֤CurrThreadID����������
};

using App_MessageServiceGroup = ACE_Singleton<CMessageServiceGroup, ACE_Null_Mutex>;
#endif
