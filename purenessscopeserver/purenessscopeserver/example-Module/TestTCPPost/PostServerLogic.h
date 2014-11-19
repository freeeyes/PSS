#ifndef POSTSERVERLOGIC_H
#define POSTSERVERLOGIC_H

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

//此类处理所有透传数据相关逻辑

#define SEND_BUFF_SIZE  MAX_BUFF_1024      //发送缓冲大小
#define RECV_BUFF_SIZE  MAX_BUFF_1024      //接收缓冲大小

class CPostServerData : public IClientMessage
{
public:
	CPostServerData() 
	{ 
		m_pServerObject      = NULL;
		m_u4ServerID         = 0;
		m_u4ConnectID        = 0;
		m_u2RecvBuffLength   = 0;
		m_u2SendBuffLength   = 0;
	};

	~CPostServerData() 
	{
		if(m_u4ServerID != 0)
		{
			Close(S2S_INNEED_CALLBACK);
		}
	};

	bool RecvData(ACE_Message_Block* mbRecv,  _ClientIPInfo objServerIPInfo)
	{
		//判断返回数据块是否小于0或者超过最大缓冲大小
		if(mbRecv->length() <= 0 || mbRecv->length() >= RECV_BUFF_SIZE)
		{
			OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get Data(%d).\n", mbRecv->length()));
			return false;
		}

		//统一贴入缓冲，再有缓冲切割发送数据
		ACE_OS::memcpy(&m_szRecvBuffData[m_u2RecvBuffLength], mbRecv->rd_ptr(), mbRecv->length());
		m_u2RecvBuffLength += mbRecv->length();

		//判断缓冲是否存有数据
		if(m_u2RecvBuffLength < 4)
		{
			//缓冲内连包头都不齐，不发送
			return true;
		}
		else
		{
			while(true)
			{
				//如果接收字节是8字节，则认为是一个完整包
				//这里可以添加算法

				if(m_u2RecvBuffLength < 8)
				{
					//数据包没收全，不做处理
					return true;
				}
				else
				{
					//数据包已经收全.发送给客户端数据
					//OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get Data(%d).\n", mbRecv->length()));
					if(NULL != m_pServerObject &&  mbRecv->length() > 0)
					{
						uint16 u2RetCommand = 0x1010;
						uint32 u4PacketLength = 8;      //固定包体8字节
						char* pData = new char[u4PacketLength + sizeof(uint32)];
						ACE_OS::memcpy(pData, (char* )&u4PacketLength, sizeof(uint32));
						ACE_OS::memcpy(&pData[sizeof(uint32)], m_szRecvBuffData, u4PacketLength);

						uint32 u4SendLength = u4PacketLength + sizeof(uint32);
						m_pServerObject->GetConnectManager()->PostMessage(m_u4ConnectID, pData, u4SendLength, SENDMESSAGE_JAMPNOMAL, u2RetCommand, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
						OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData](%d)Send Data(%d) OK.\n", m_u4ConnectID, u4SendLength));

						//缓冲前移，如果有多个继续处理
						m_u2RecvBuffLength -= u4PacketLength;
						if(m_u2RecvBuffLength > 0)
						{
							ACE_OS::memcpy(&m_szRecvBuffData, (char* )&m_szRecvBuffData[m_u2RecvBuffLength], u4PacketLength);
						}
					}
				}
			}

		}

		return true;
	};

	//发送数据
	bool SendData(const char* pData, uint32 u4Size)
	{
		//如果超过最大发送长度
		if(m_u2SendBuffLength + u4Size >= MAX_BUFF_1024 || u4Size >= SEND_BUFF_SIZE)
		{
			OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) send buff full.\n", m_u4ServerID));
			return false;
		}

		//得到当前连接状态
		if(SERVER_CONNECT_OK == m_pServerObject->GetClientManager()->GetConnectState(m_u4ServerID))
		{
			//将数据放入缓冲，和之前的数据(如果有的话)一起发送
			ACE_OS::memcpy(&m_szSendBuffData[m_u2SendBuffLength], pData, u4Size);
			m_u2SendBuffLength += u4Size;

			//发送数据
			OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send [%d] Begin.", m_u4ServerID, m_u2SendBuffLength));
			if(false == m_pServerObject->GetClientManager()->SendData((int)m_u4ServerID, m_szSendBuffData, m_u2SendBuffLength, false))
			{
				//发送失败，缓冲
				OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send [%d] End 1.", m_u4ServerID, m_u2SendBuffLength));
				return false;
			}
			else
			{
				//发送成功，清理缓冲
				OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send [%d] End.", m_u4ServerID, m_u2SendBuffLength));
				m_u2SendBuffLength = 0;
				return true;
			}
		}
		else
		{
			OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send Buffer.", m_u4ServerID));
			//连接尚未建立好，放入缓冲，等连接建立后自动回调发
			ACE_OS::memcpy(&m_szSendBuffData[m_u2SendBuffLength], pData, u4Size);
			m_u2SendBuffLength += u4Size;
			return true;
		}
	}

	//关闭远程服务
	bool Close(EM_s2s ems2s)
	{
		m_pServerObject->GetClientManager()->Close(m_u4ServerID, ems2s);
		m_u4ServerID = 0;
		return true;
	}

	bool ConnectError(int nError,  _ClientIPInfo objServerIPInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CPostServerData::ConnectError]Get Error(%d).\n", nError));
		return true;
	};

	void ReConnect(int nServerID)
	{
		//数据重连成功接口
		//m_u4ServerID = (uint32)nServerID;
		OUR_DEBUG((LM_ERROR, "[CPostServerData::ReConnect]ReConnect(%d).\n", m_u4ServerID));
		if(m_u2SendBuffLength > 0)
		{
			//发送数据
			if(true == m_pServerObject->GetClientManager()->SendData((int)m_u4ServerID, m_szSendBuffData, m_u2SendBuffLength, false))
			{
				//发送成功，清理缓冲
				m_u2SendBuffLength = 0;
			}
		}
	}

	CServerObject* GetServerObject()
	{
		return m_pServerObject; 
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
	ACE_Recursive_Thread_Mutex m_ThreadLock;
	CServerObject*             m_pServerObject;
	uint32                     m_u4ServerID;
	uint32                     m_u4ConnectID;
	char                       m_szRecvBuffData[RECV_BUFF_SIZE];  //接收缓冲池
	char                       m_szSendBuffData[SEND_BUFF_SIZE];  //发送缓冲池 
	uint16                     m_u2RecvBuffLength;                //接收缓冲长度
	uint16                     m_u2SendBuffLength;                //发送缓冲长度
};

#endif
