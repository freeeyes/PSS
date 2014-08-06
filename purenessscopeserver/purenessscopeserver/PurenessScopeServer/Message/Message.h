#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"

#include "../Common/MessageBlockManager.h"
#include "../Common/BuffPacket.h"
#include "../IObject/IMessage.h"

#include <map>

using namespace std;

class CMessage : public IMessage
{
public:
	CMessage(void);
	~CMessage(void);

	void Close();
	void Clear();

	void SetMessageBase(_MessageBase* pMessageBase);
	void SetPacketHeadInfo(_PacketHeadInfo& objPacketHeadInfo);
	bool SetRecvPacket(IBuffPacket* pRecvPacket);

	ACE_Message_Block* GetMessageHead();
	ACE_Message_Block* GetMessageBody();

	_MessageBase* GetMessageBase();
	_PacketHeadInfo* GetPacketHeadInfo();

	bool GetPacketHead(_PacketInfo& PacketInfo);
	bool GetPacketBody(_PacketInfo& PacketInfo);
	bool SetPacketHead(ACE_Message_Block* pmbHead);
	bool SetPacketBody(ACE_Message_Block* pmbBody);

	const char* GetError();

private:
	char          m_szError[MAX_BUFF_500];
	_MessageBase* m_pMessageBase;

	ACE_Message_Block* m_pmbHead;             //包头部分
	ACE_Message_Block* m_pmbBody;             //包体部分
	_PacketHeadInfo    m_objPacketHeadInfo;   //包头协议信息
};


//Message对象池
class CMessagePool
{
public:
	CMessagePool();
	~CMessagePool();

	void Init(uint32 u4PacketCount);
	void Close();

	CMessage* Create();
	bool Delete(CMessage* pMakePacket);

	int GetUsedCount();
	int GetFreeCount();

private:
	typedef map<CMessage*, CMessage*> mapMessage;
	mapMessage                  m_mapMessageUsed;                      //已使用的
	mapMessage                  m_mapMessageFree;                      //没有使用的
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //控制多线程锁
};
typedef ACE_Singleton<CMessagePool, ACE_Null_Mutex> App_MessagePool; 

#endif
