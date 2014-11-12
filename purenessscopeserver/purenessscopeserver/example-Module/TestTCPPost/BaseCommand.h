#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

#include <string>
#include <map>

//定义客户端信令(TCP)
#define COMMAND_BASE            0x1000
#define COMMAND_RETURN_ALIVE    0xf001

using namespace std;

class CPostServerData : public IClientMessage
{
public:
	CPostServerData() 
	{ 
		m_pServerObject      = NULL;
		m_u4ServerID         = 0;
		m_u4ConnectID        = 0;
		m_u2BufferDataLength = 0;
	};

	~CPostServerData() {};

	bool RecvData(ACE_Message_Block* mbRecv,  _ClientIPInfo objServerIPInfo)
	{
		//判断返回数据块是否小于0
		if(mbRecv->length() <= 0)
		{
			OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get Data(%d).\n", mbRecv->length()));
			return false;
		}

		//统一贴入缓冲，再有缓冲切割发送数据
		ACE_OS::memcpy(&m_szBufferData[m_u2BufferDataLength], mbRecv->rd_ptr(), mbRecv->length());
		m_u2BufferDataLength += mbRecv->length();

		//判断缓冲是否存有数据
		if(m_u2BufferDataLength < 4)
		{
			//缓冲内连包头都不齐，不发送
			return true;
		}
		else
		{
			while(true)
			{
				//切包，得到包长度
				uint32 u4PacketLength = 0;
				ACE_OS::memcpy(&u4PacketLength, m_szBufferData, sizeof(uint32));

				uint16 u2SendPacketLength = (uint16)u4PacketLength + (uint16)sizeof(uint32);
				if(u2SendPacketLength > m_u2BufferDataLength)
				{
					//数据包没收全，不做处理
					return true;
				}
				else
				{
					//数据包已经收全.发送数据
					OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get Data(%d).\n", mbRecv->length()));
					if(NULL != m_pServerObject &&  mbRecv->length() > 0)
					{
						uint16 u2RetCommand = 0x1010;
						char* pData = new char[u4PacketLength + sizeof(uint32)];
						ACE_OS::memcpy(pData, m_szBufferData, u4PacketLength + sizeof(uint32));

						uint32 u4SendLength = u4PacketLength + sizeof(uint32);
						m_pServerObject->GetConnectManager()->PostMessage(m_u4ConnectID, pData, u4SendLength, SENDMESSAGE_JAMPNOMAL, u2RetCommand, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);

						//缓冲前移，如果有多个继续处理
						m_u2BufferDataLength -= u2SendPacketLength;
						if(m_u2BufferDataLength > 0)
						{
							ACE_OS::memcpy(&m_szBufferData, (char* )&m_szBufferData[u2SendPacketLength], m_u2BufferDataLength);
						}
					}
				}
			}

		}

		return true;
	};

	bool ConnectError(int nError,  _ClientIPInfo objServerIPInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CPostServerData::ConnectError]Get Error(%d).\n", nError));
		return true;
	};

	void ReConnect(int nServerID)
	{
		//数据重连成功接口
		m_u4ServerID = (uint32)nServerID;
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

	void SetServerID(uint32 u4ServerID)
	{
		m_u4ServerID = u4ServerID;
	}

	uint32 GetServerID()
	{
		return m_u4ServerID;
	}

private:
	CServerObject* m_pServerObject;
	uint32         m_u4ServerID;
	uint32         m_u4ConnectID;
	char           m_szBufferData[MAX_BUFF_1024];    //缓冲数据
	uint16         m_u2BufferDataLength;             //缓冲长度
};

typedef map<uint32, CPostServerData*> mapc2s;

class CBaseCommand : public CClientCommand
{
public:
	CBaseCommand(void);
	~CBaseCommand(void);

	int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
	void SetServerObject(CServerObject* pServerObject);
	void InitServer();

private:
	void AddClient2Server(uint32 u4ClientID);
	void DelClient2Server(uint32 u4ClientID);
	CPostServerData* GetClient2Server_ServerID(uint32 u4ClientID);
	void CloseClient2Server();

private:
	CServerObject*   m_pServerObject;
	int              m_nCount;
	mapc2s           m_mapC2S;                 //客户端和服务器映射关系 
};
