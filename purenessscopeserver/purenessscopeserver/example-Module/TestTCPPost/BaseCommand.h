#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>

//定义客户端信令(TCP)
#define COMMAND_BASE            0x1000
#define COMMAND_RETURN_ALIVE    0xf001

using namespace std;

class CPostServerData : public IClientMessage
{
public:
	CPostServerData() 
	{ 
		m_pServerObject = NULL;
		m_u4ConnectID   = 0;
	};

	~CPostServerData() {};

	bool RecvData(ACE_Message_Block* mbRecv)
	{
		//判断返回数据块是否小于0
		if(mbRecv->length() <= 0)
		{
			OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get Data(%d).\n", mbRecv->length()));
		}
		
		OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get Data(%d).\n", mbRecv->length()));
		if(NULL != m_pServerObject &&  mbRecv->length() > 0)
		{
			uint16 u2RetCommand = 0x1010;
			char* pData = new char[mbRecv->length()];
			ACE_OS::memcpy(pData, mbRecv->rd_ptr(), mbRecv->length());
			
			m_pServerObject->GetConnectManager()->PostMessage(m_u4ConnectID, pData, mbRecv->length(), SENDMESSAGE_JAMPNOMAL, u2RetCommand, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
		}

		return true;
	};

	bool ConnectError(int nError)
	{
		OUR_DEBUG((LM_ERROR, "[CPostServerData::ConnectError]Get Error(%d).\n", nError));
		return true;
	};

	void ReConnect(int nServerID)
	{
		//数据重连成功接口
		OUR_DEBUG((LM_ERROR, "[CPostServerData::ReConnect]ReConnect(%d).\n", nServerID));
	}

	void SetServerObject(CServerObject* pServerObject)
	{
		m_pServerObject = pServerObject;
	}

	void SetConnectID(uint32 u4ConnectID)
	{
		m_u4ConnectID = u4ConnectID;
	}

private:
	CServerObject* m_pServerObject;
	uint32         m_u4ConnectID;
};

class CBaseCommand : public CClientCommand
{
public:
	CBaseCommand(void);
	~CBaseCommand(void);

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
	void SetServerObject(CServerObject* pServerObject);
	void InitServer();

private:
	CServerObject*   m_pServerObject;
	int              m_nCount;
	CPostServerData* m_pPostServerData1;  //中间服务器发送对象
};
