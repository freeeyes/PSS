#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "ace/Date_Time.h"

#include <string>

//定义客户端信令(TCP)
#define MODULE_RETURN_ERROR    1
#define MODULE_RETURN_SUCCESS  0

#define COMMAND_RETURN_PLUGA    0xf201

#define MODULE_COMMAND_INIT     0xe000
#define MODULE_COMMAND_PLUGB    0xe001

using namespace std;

class CBaseCommand : public CClientCommand
{
public:
  CBaseCommand(void);
  ~CBaseCommand(void);

  int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

  void DoModuleMessage(IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
  void DoModuleInit(IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);

  void SetServerObject(CServerObject* pServerObject);

private:
  CServerObject*             m_pServerObject;
  int                        m_nCount;
  ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
};
