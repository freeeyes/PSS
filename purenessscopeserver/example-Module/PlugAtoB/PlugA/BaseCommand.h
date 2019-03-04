#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_PLUGA           0x2201
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

  void PlugInit();

  void SetServerObject(CServerObject* pServerObject);

private:
	void Do_PlugA_DATA(IMessage* pMessage);        //处理PlugA

private:
  CServerObject* m_pServerObject;
  int            m_nCount;
};
