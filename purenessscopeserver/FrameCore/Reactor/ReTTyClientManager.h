#ifndef _PROTTYCLIENTMANAGER_H
#define _PROTTYCLIENTMANAGER_H

//TTy�豸��Proactorʵ��
//add by freeeyes

#include "ITTyClientManager.h"
#include "ReTTyHandler.h"
#include "HashTable.h"
#include "XmlConfig.h"
#include "TimerManager.h"

class CReTTyClientManager : public ITTyClientManager
{
public:
    CReTTyClientManager();
    virtual ~CReTTyClientManager();

    bool StartConnectTask();                                                                             //������ʱ��
    void CancelConnectTask();                                                                            //�رն�ʱ��

    bool Init(ACE_Reactor* pReactor, uint16 u2MaxTTyCount, uint16 u2TimeCheck);                          //��ʼ��������
    void Close();                                                                                        //�ر���������

    int Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv) final;  // ���ӣ��򿪣��˿�
    int ConnectFrame(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, uint32 u4PacketParseID) final;    // ���ӣ��򿪣��˿�

    bool GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam) final;                           // ��ȡ����������Ϣ
    bool IsConnect(uint16 u2ConnectID) final;                                                          // �Ƿ����ӣ��򿪣�״̬

    bool Close(uint16 u2ConnectID) final;                                                              // �رն˿�
    bool Pause(uint16 u2ConnectID) final;                                                              // ��ͣ�˿�
    bool Resume(uint16 u2ConnectID) final;                                                             // �ָ���ͣ�˿�

    bool SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len) final;                         // ��������

    int timer_task(brynet::TimerMgr::Ptr timerMgr);
    void start_new_task(brynet::TimerMgr::Ptr timerMgr);

private:
    CHashTable<CReTTyHandler> m_objTTyClientHandlerList;                         //�����豸�б�
    ACE_Recursive_Thread_Mutex m_ThreadWritrLock;                                //�߳���
    ACE_Reactor*               m_pReactor       = nullptr;                       //��Ӧ�����
    uint16                     m_u2MaxListCount = MAX_BUFF_100;                  //����豸����
    uint16                     m_u2TimeCheck    = 120;                           //��ʱ�����ʱ��
    bool                       m_blTimerState   = true;                          //��ʱ�����
};

typedef ACE_Singleton<CReTTyClientManager, ACE_Recursive_Thread_Mutex> App_ReTTyClientManager;


#endif
