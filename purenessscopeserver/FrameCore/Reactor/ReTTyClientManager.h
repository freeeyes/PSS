#ifndef _PROTTYCLIENTMANAGER_H
#define _PROTTYCLIENTMANAGER_H

//TTy�豸��Proactorʵ��
//add by freeeyes

#include "ITTyClientManager.h"
#include "ReTTyHandler.h"
#include "HashTable.h"
#include "XmlConfig.h"
#include "TimerManager.h"

class CReTTyClientManager : public ACE_Event_Handler, public ITTyClientManager
{
public:
    CReTTyClientManager();
    virtual ~CReTTyClientManager();

    bool StartConnectTask();                                                                             //������ʱ��
    void CancelConnectTask();                                                                            //�رն�ʱ��

    bool Init(ACE_Reactor* pReactor, uint16 u2MaxTTyCount, uint16 u2TimeCheck);                          //��ʼ��������
    void Close();                                                                                        //�ر���������

    virtual int Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv);  // ���ӣ��򿪣��˿�
    virtual int ConnectFrame(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, uint32 u4PacketParseID);    // ���ӣ��򿪣��˿�

    virtual bool GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam);                           // ��ȡ����������Ϣ
    virtual bool IsConnect(uint16 u2ConnectID);                                                          // �Ƿ����ӣ��򿪣�״̬

    virtual bool Close(uint16 u2ConnectID);                                                              // �رն˿�
    virtual bool Pause(uint16 u2ConnectID);                                                              // ��ͣ�˿�
    virtual bool Resume(uint16 u2ConnectID);                                                             // �ָ���ͣ�˿�

    virtual bool SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len);                         // ��������

    virtual int handle_timeout(const ACE_Time_Value& current_time, const void* act = 0);                 //��ʱ��ִ��
private:
    CHashTable<CReTTyHandler> m_objTTyClientHandlerList;                         //�����豸�б�
    ACE_Recursive_Thread_Mutex m_ThreadWritrLock;                                //�߳���
    ACE_Reactor*               m_pReactor       = nullptr;                          //��Ӧ�����
    uint16                     m_u2MaxListCount = MAX_BUFF_100;                  //����豸����
    uint16                     m_u2TimeCheck    = 120;                           //��ʱ�����ʱ��
    int                        m_nTaskID        = -1;                            //��ʱ��ID
};

typedef ACE_Singleton<CReTTyClientManager, ACE_Recursive_Thread_Mutex> App_ReTTyClientManager;


#endif
