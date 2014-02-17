/*
 * 来自服务器的消息处理
 * by w1w
 */
#pragma once

#include "IBuffPacket.h"
#include "IObject.h"
#include "CommandDefine.h"

#include <string>
using namespace std;
class CPostServerCommand : public IClientMessage
{
public:
	CPostServerCommand();
	~CPostServerCommand();

	bool RecvData(ACE_Message_Block* mbRecv);
	bool ConnectError(int nError);

	void SetServerObject(CServerObject* pServerObject);
private:
	CServerObject* m_pServerObject;
};
