#ifndef _SENDMESSAGE_H
#define _SENDMESSAGE_H

#include "define.h"
#include "IConnectManager.h"
#include "IBuffPacket.h"
#include "HashTable.h"
#include "ObjectArrayList.h"
#include "TimeStamp.hpp"
#include <string>
#include <sstream>

//���������巢�����ݽṹ
class CSendMessageInfo
{
public:
    uint32 u4ConnectID                  = 0;
    shared_ptr<IBuffPacket> pBuffPacket = nullptr;
    uint16 u2CommandID                  = 0;
    EM_Client_Send_Status emSendState   = EM_Client_Send_Status::CLIENT_SEND_IMMEDIATLY;
    EM_SEND_PACKET_PARSE emSendType     = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    PSS_Time_Point tvSendBegin          = CTimeStamp::Get_Time_Stamp();
    bool blDelete                       = true;
    int nMessageID                      = 0;
};

//����һ���������������������첽���Ͷ���
class _SendMessage final
{
public:
    uint32              m_u4ConnectID = 0;        //Ҫ���͵�Զ��ID
    int32               m_nMessageID  = 0;        //������Ϣ��ID
    int32               m_nHashID     = 0;        //��ǰ�����HashID
    uint16              m_u2CommandID = 0;        //Ҫ���͵�����ID������ͳ�ƹ���
    uint8               m_u1SendState = 0;        //Ҫ���͵�״̬��0���������ͣ�1���Ȼ��治���ͣ�2���������ͺ�ر�
    uint8               m_nEvents     = 0;        //�������ͣ�0��ʹ��PacketParse��֯�������ݣ�1����ʹ��PacketParse��֯����
    uint8               m_u1Type      = 0;        //���ݰ������ͣ�0:���ݰ���1:�����ر���Ϊ
    bool                m_blDelete    = true;     //������ɺ��Ƿ�ɾ����true��ɾ����false�ǲ�ɾ��
    IBuffPacket*        m_pBuffPacket = nullptr;  //���ݰ�����
    PSS_Time_Point      m_tvSend;                 //���ݰ����͵�ʱ���

    _SendMessage() = default;

    void Clear()
    {
        m_u1SendState = 0;
        m_blDelete    = true;
        m_pBuffPacket = nullptr;
        m_u4ConnectID = 0;
        m_nEvents     = 0;
        m_u2CommandID = 0;
        m_nMessageID  = 0;
        m_u1Type      = 0;
        m_nHashID     = 0;
    }

    void SetHashID(int32 nHashID)
    {
        m_nHashID = nHashID;
    }

    int32 GetHashID() const
    {
        return m_nHashID;
    }
};

class CSendMessagePool
{
public:
    CSendMessagePool(void) = default;

    void Init(int32 nObjcetCount = MAX_MSG_THREADQUEUE);
    void Close();

    _SendMessage* Create();
    bool Delete(_SendMessage* pObject);

    int32 GetUsedCount();
    int32 GetFreeCount();

private:
    CHashTable<_SendMessage>        m_objHashHandleList;
    CObjectArrayList<_SendMessage>  m_objSendMessageList;
    ACE_Recursive_Thread_Mutex      m_ThreadWriteLock;                     //���ƶ��߳���
};
#endif

