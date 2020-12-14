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

//发送数据体发的数据结构
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

//定义一个发送数据容器，用于异步发送队列
class _SendMessage final
{
public:
    uint32              m_u4ConnectID = 0;        //要发送的远程ID
    int32               m_nMessageID  = 0;        //发送消息的ID
    int32               m_nHashID     = 0;        //当前对象的HashID
    uint16              m_u2CommandID = 0;        //要发送的命令ID，用于统计功能
    uint8               m_u1SendState = 0;        //要发送的状态，0是立即发送，1是先缓存不发送，2是立即发送后关闭
    uint8               m_nEvents     = 0;        //发送类型，0：使用PacketParse组织发送数据，1：不使用PacketParse组织数据
    uint8               m_u1Type      = 0;        //数据包的类型，0:数据包，1:主动关闭行为
    bool                m_blDelete    = true;     //发送完成后是否删除，true是删除，false是不删除
    IBuffPacket*        m_pBuffPacket = nullptr;  //数据包内容
    PSS_Time_Point      m_tvSend;                 //数据包发送的时间戳

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
    ACE_Recursive_Thread_Mutex      m_ThreadWriteLock;                     //控制多线程锁
};
#endif

