#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "MessageBlockManager.h"
#include "BuffPacket.h"
#include "IMessage.h"
#include "ObjectPoolManager.h"

class CMessage : public IMessage
{
public:
    CMessage(void);

    void Close();
    void Clear();

    void SetHashID(int nHasnID);
    int  GetHashID();

    void SetMessageBase(_MessageBase* pMessageBase);

    ACE_Message_Block* GetMessageHead();
    ACE_Message_Block* GetMessageBody();

    _MessageBase* GetMessageBase();

    bool GetPacketHead(_PacketInfo& PacketInfo);
    bool GetPacketBody(_PacketInfo& PacketInfo);
    bool SetPacketHead(ACE_Message_Block* pmbHead);
    bool SetPacketBody(ACE_Message_Block* pmbBody);

    const char* GetError();

    ACE_Message_Block*  GetQueueMessage();

private:
    int           m_nHashID               = 0;
    char          m_szError[MAX_BUFF_500] = {'\0'};
    _MessageBase* m_pMessageBase          = NULL;

    ACE_Message_Block* m_pmbHead          = NULL;    //包头部分
    ACE_Message_Block* m_pmbBody          = NULL;   //包体部分

    ACE_Message_Block*  m_pmbQueuePtr     = NULL;   //消息队列指针块
};


//Message对象池
class CMessagePool : public CObjectPoolManager<CMessage, ACE_Recursive_Thread_Mutex>
{
public:
    CMessagePool();

    static void Init_Callback(int nIndex, CMessage* pMessage);
    static void Close_Callback(int nIndex, CMessage* pMessage);

    CMessage* Create();
    bool Delete(CMessage* pMakePacket);

    int GetUsedCount();
    int GetFreeCount();

    void GetCreateInfoList(vector<_Object_Create_Info>& objCreateList);
};

#endif
