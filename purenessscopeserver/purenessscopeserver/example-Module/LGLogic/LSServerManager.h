#ifndef _LSSERVERMANAGER_H
#define _LSSERVERMANAGER_H

#include "IBuffPacket.h"
#include "IObject.h"

#define SEND_BUFF_SIZE  MAX_BUFF_1024              //发送缓冲大小
#define RECV_BUFF_SIZE  MAX_BUFF_1024              //接收缓冲大小

#define SERVER_PROTOCAL_VERSION       100          //服务器通讯协议版本号

#define COMMAND_LOGIC_LG_LOGIN        0x2000       //LG注册 
#define COMMAND_LOGIC_ALIVE		      0x2001       //LG心跳  
#define COMMAND_LOGIC_LG_LOGIN_R      0xe000       //LG注册回应
#define COMMAND_LOGIC_LG_KEY_R        0xe002       //LG下发列表

#define SESSION_KEY                   "LogicSever" //数据包Session KEY

class CLSServerManager : public IClientMessage
{
public:
	CLSServerManager();
	~CLSServerManager();

	void Init(uint32 u4ServerID, const char* pIP, uint32 u4Port, CServerObject* pServerObject);

	void Set_LG_Info(const char* pLGIP, uint32 u4LGPort, uint32 u4LGID);

	bool Connect();

	bool RecvData(ACE_Message_Block* mbRecv,  _ClientIPInfo objServerIPInfo);

	bool ConnectError(int nError,  _ClientIPInfo objServerIPInfo);

	void ReConnect(int nServerID);

	char* Get_LS_Key();

	void Send_LG_Login();

	void Send_LG_Alive();

private:
	void Recv_LS_Login(const char* pRecvBuff, uint32 u4Len);
	void Recv_LS_Key_Update(const char* pRecvBuff, uint32 u4Len);

private:


private:
	uint32         m_u4ServerID;
	char           m_szServerIP[50];
	uint32         m_u4Port;
	CServerObject* m_pServerObject;
	char           m_szLSKey[50];
	char           m_szSessionKey[32];

	char           m_szLGIP[50];
	uint32         m_u4LGPort;
	uint32         m_u4LGID;
};

#endif
