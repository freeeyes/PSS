#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "ace/Date_Time.h"

#include "zkClient.h"

using namespace std;

//定义ZooKeeper服务器节点信息
#define ZOOKEEPER_ADDRESS "localhost:4181"

class CBaseCommand : public CClientCommand
{
public:
  CBaseCommand(void);
  ~CBaseCommand(void);

  void Init();
  
  void Fini();

  int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

  void SetServerObject(CServerObject* pServerObject);
  
  void Watch_Zoo_Path_Child();

private:
  CServerObject*             m_pServerObject;
  int                        m_nCount;
  CZKClient                  m_ZKClient;
  ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
};
