#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "QueueDefine.h"

//定义客户端信令(TCP)
#define COMMAND_LINUXQUEUE         0x2200     //队列信息命令
#define COMMAND_RETURN_LINUXQUEUE  0xe200     //返回队列处理信息

using namespace std;

class CBaseCommand : public CClientCommand
{
public:
  CBaseCommand(void);
  ~CBaseCommand(void);

  int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
  void SetServerObject(CServerObject* pServerObject);

  bool InitQueue();          //初始化线程队列
  int  GetMutiQueueID();     //得到MutiQueueID

  void SendData(_Muti_QueueData& objMutiQueueData);

private:
	void Do_Linux_Queue(IMessage* pMessage);        //处理登陆

private:
  CServerObject*    m_pServerObject;
  int               m_nPssQueueID;
  int               m_nMutiQueueID;
};


