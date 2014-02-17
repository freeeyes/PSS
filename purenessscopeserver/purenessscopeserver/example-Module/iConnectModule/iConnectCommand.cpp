/*
 * 消息处理
 * by w1w
 */
#include "iConnectCommand.h"
#include "ServerConfig.h"

CiConnectCommand::CiConnectCommand(void)
{
	m_pServerObject = NULL;
	m_nTimerID     = -1;
	m_nCount        = 0;
}

CiConnectCommand::~CiConnectCommand(void)
{
}

void CiConnectCommand::SetServerObject(CServerObject* pServerObject)
{
	m_pServerObject = pServerObject;
}


/************************************************************************/
/*  初始化                                                              */
/************************************************************************/
void CiConnectCommand::InitConfig()
{
	if(!icServerConfig::instance()->Init())
	{
		OUR_DEBUG((LM_INFO, "[main]%s\n", icServerConfig::instance()->GetError()));
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CiConnectCommand]服务ID %d\n", icServerConfig::instance()->m_Server.m_Serverid));
		OUR_DEBUG((LM_INFO, "[CiConnectCommand]服务器名 %s\n", icServerConfig::instance()->m_Server.m_szServerName));
		OUR_DEBUG((LM_INFO, "[CiConnectCommand]服务器版本 %s\n", icServerConfig::instance()->m_Server.m_szServerVersion));
		icServerConfig::instance()->Display();
		icServerConfig::instance()->SetClientManager(m_pServerObject->GetClientManager());
	}
}

void CiConnectCommand::InitServer()
{
	CPostServerCommand* pPostServerCommand = &m_PostServerCommand;
	pPostServerCommand->SetServerObject(m_pServerObject);
	StartCheckServerConnect(10); // 30秒心跳
	if(NULL != pPostServerCommand)
	{
		//连接到远程服务器
		for(mapServerInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
			iter != icServerConfig::instance()->m_mapServerInfo.end();
			++iter)
		{
			m_pServerObject->GetClientManager()->Connect(iter->second.m_Serverid, iter->second.m_szServerIP, iter->second.m_nPort, TYPE_IPV4, pPostServerCommand);
		}
	}
}

/************************************************************************/
/*  常用方法                                                            */
/************************************************************************/
void CiConnectCommand::PostMessage(uint32 u4ConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState)
{
	if(NULL != m_pServerObject->GetConnectManager())
	{
		m_pServerObject->GetConnectManager()->PostMessage(u4ConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CiConnectCommand::PostMessage][ConnectID:%d ] m_pConnectManager = NULL",u4ConnectID));
	}
}
void CiConnectCommand::PostMessage(vector<uint32> vecConnectID, IBuffPacket* pBuffPacket, uint8 u1SendType, uint16 u2CommandID, bool blSendState)
{
	if(NULL != m_pServerObject->GetConnectManager())
	{
		m_pServerObject->GetConnectManager()->PostMessage(vecConnectID, pBuffPacket, u1SendType, u2CommandID, blSendState);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CiConnectCommand::PostMessage] m_pConnectManager = NULL"));
	}
}
void CiConnectCommand::SendData(int nServerID, const char* pData, int nSize, bool blIsDelete)
{
	if(false == m_pServerObject->GetClientManager()->SendData(nServerID, pData, nSize ,blIsDelete))
	{
		OUR_DEBUG((LM_ERROR, "[CiConnectCommand::SendData] Send Post Data Error.\n"));
	}
}
bool CiConnectCommand::CheckpMessage(IMessage* pMessage)
{	
	if(m_pServerObject == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[包处理] m_pServerObject is NULL.\n"));
		return false;
	}

	if(pMessage == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[包处理] pMessage is NULL.\n"));
		return false;
	}
	return true;
}
uint32 CiConnectCommand::getAppconnect( uint16 DestAppID )
{
	for(mapConnectInfo::iterator iter = icServerConfig::instance()->m_mapConnectInfo.begin();
		iter != icServerConfig::instance()->m_mapConnectInfo.end();
		++ iter)
	{
		if (iter->second.m_Appid == DestAppID) return iter->second.m_ConnectID;
	}
	return 0;
}

void CiConnectCommand::BroadCastServerState()
{
	for(mapConnectInfo::iterator iter = icServerConfig::instance()->m_mapConnectInfo.begin();
		iter != icServerConfig::instance()->m_mapConnectInfo.end();
		++ iter)
	{
		OUR_DEBUG((LM_ERROR, "[ CiConnectCommand::BroadCastServerState] 广播服务器状态.\n"));

		IBuffPacket* pResponsesPacket = MakeQueryServerRespPacket();
		uint16 u2PostCommandID = COMMAND_QUERY_SERVER_RESPONSE;
		PostMessage(iter->second.m_ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
	}
}

/************************************************************************/
/*  包格式生成                                                          */
/************************************************************************/
//App查询包
int CiConnectCommand::MakeQueryAppPacket(char*& pdata, uint16 Serverid)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_QUERY_APP;
	uint32 packetsize = 4;
	(*pResponsesPacket) << packetsize;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << Serverid;

	uint32 PacketLen = pResponsesPacket->GetPacketLen();
	pdata = new char[PacketLen];
	memcpy(pdata,pResponsesPacket->GetData(),PacketLen);

	m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	return PacketLen;
}
IBuffPacket* CiConnectCommand::MakeQueryAppPacket(uint16 Serverid)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_QUERY_APP;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << Serverid;

	return pResponsesPacket;
}
//App回应包
int CiConnectCommand::MakeQueryAppRespPacket(char*& pdata)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_QUERY_APP_RESPONSE;
	uint16 u2AppCount = icServerConfig::instance()->m_Server.m_mapAppInfo.size();
	uint16 AppBlocksize = 3;
	uint32 packetsize = u2AppCount * AppBlocksize + 6;
	uint8 isadd = 1;
	(*pResponsesPacket) << packetsize;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << icServerConfig::instance()->m_Server.m_Serverid;
	(*pResponsesPacket) << u2AppCount;    // 总App数量

	for(mapAppInfo::iterator iter = icServerConfig::instance()->m_Server.m_mapAppInfo.begin();
		iter != icServerConfig::instance()->m_Server.m_mapAppInfo.end();
		++ iter)
	{
		(*pResponsesPacket) << iter->second.m_Appid;  //Appid
		(*pResponsesPacket) << isadd;                 //增加App
	}

	uint32 PacketLen = pResponsesPacket->GetPacketLen();
	pdata = new char[PacketLen];
	memcpy(pdata,pResponsesPacket->GetData(),PacketLen);

	m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	return PacketLen;
}
IBuffPacket* CiConnectCommand::MakeQueryAppRespPacket()
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_QUERY_APP_RESPONSE;
	uint16 u2AppCount = icServerConfig::instance()->m_Server.m_mapAppInfo.size();
	uint8 isadd = 1;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << icServerConfig::instance()->m_Server.m_Serverid;
	(*pResponsesPacket) << u2AppCount;    // 总App数量

	for(mapAppInfo::iterator iter = icServerConfig::instance()->m_Server.m_mapAppInfo.begin();
		iter != icServerConfig::instance()->m_Server.m_mapAppInfo.end();
		++ iter)
	{
		(*pResponsesPacket) << iter->second.m_Appid;  //Appid
		(*pResponsesPacket) << isadd;                 //增加App
	}

	return pResponsesPacket;
}
IBuffPacket* CiConnectCommand::MakeQueryAppRespPacket(uint16 Serverid)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_QUERY_APP_RESPONSE;
	uint16 u2AppCount = icServerConfig::instance()->m_mapServerInfo[Serverid].m_mapAppInfo.size();
	uint8 isadd = 1;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << Serverid;
	(*pResponsesPacket) << u2AppCount;    // 总App数量

	for(mapAppInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo[Serverid].m_mapAppInfo.begin();
		iter != icServerConfig::instance()->m_mapServerInfo[Serverid].m_mapAppInfo.end();
		++ iter)
	{
		(*pResponsesPacket) << iter->second.m_Appid;  //Appid
		(*pResponsesPacket) << isadd;                 //增加App
	}

	return pResponsesPacket;
}
//注册回应包
IBuffPacket* CiConnectCommand::MakeRegAcceptRespPacket()
{
	uint8 RegResult = 1;
	return MakeRegRespPacket(RegResult);
}
IBuffPacket* CiConnectCommand::MakeRegRejectRespPacket()
{
	uint8 RegResult = 0;
	return MakeRegRespPacket(RegResult);
}
IBuffPacket* CiConnectCommand::MakeRegRespPacket(uint8 RegResult)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_REG_RESPONSE;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << RegResult;
	return pResponsesPacket;
}
//查询服务器回应包
IBuffPacket* CiConnectCommand::MakeQueryServerRespPacket()
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_QUERY_SERVER_RESPONSE;
	uint16 u2ServerCount = icServerConfig::instance()->m_mapServerInfo.size() + 1;
	uint8 u1online = 1;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << u2ServerCount;

	(*pResponsesPacket) << icServerConfig::instance()->m_Server.m_Serverid;
	pResponsesPacket->WriteStream(icServerConfig::instance()->m_Server.m_szStationID , 33);
	(*pResponsesPacket) << u1online;

	for(mapServerInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
		iter != icServerConfig::instance()->m_mapServerInfo.end();
		++ iter)
	{
		u1online = iter->second.isonline() ? 1 : 0;
		(*pResponsesPacket) << iter->second.m_Serverid;
		pResponsesPacket->WriteStream(iter->second.m_szStationID , 33);
		(*pResponsesPacket) << u1online;
	}

	return pResponsesPacket;
}
//App同步包
int CiConnectCommand::MakeSynAppPacket(char*& pdata,uint16 Appid, bool isdelete)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_SYN_APP;
	uint16 u2AppCount = 1;
	uint16 AppBlocksize = 3;
	uint32 packetsize = u2AppCount * AppBlocksize + 6;
	uint8 isadd = isdelete ? 0 : 1;
	(*pResponsesPacket) << packetsize;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << icServerConfig::instance()->m_Server.m_Serverid;
	(*pResponsesPacket) << u2AppCount;    // 总App数量
	(*pResponsesPacket) << Appid;  //Appid
	(*pResponsesPacket) << isadd;                 //增加App

	uint32 PacketLen = pResponsesPacket->GetPacketLen();
	pdata = new char[PacketLen];
	memcpy(pdata,pResponsesPacket->GetData(),PacketLen);

	m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	return PacketLen;
}
IBuffPacket* CiConnectCommand::MakeSynAppPacket(uint16 Serverid, uint16 Appid, bool isdelete)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_SYN_APP;
	uint16 u2AppCount = 1;
	uint8 isadd = isdelete ? 0 : 1;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << Serverid;
	(*pResponsesPacket) << u2AppCount;    // 总App数量
	(*pResponsesPacket) << Appid;  //Appid
	(*pResponsesPacket) << isadd;                 //增加App

	return pResponsesPacket;
}
//心跳包
int CiConnectCommand::MakeHeartPacket(char*& pdata)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_KEEP_ALIVE;
    uint32 packetsize = 2;
	(*pResponsesPacket) << packetsize;
	(*pResponsesPacket) << u2PostCommandID;


	uint32 PacketLen = pResponsesPacket->GetPacketLen();
	pdata = new char[PacketLen];
	memcpy(pdata,pResponsesPacket->GetData(),PacketLen);

    m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	return PacketLen;
}
IBuffPacket* CiConnectCommand::MakeHeartRespPacket()
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_KEEP_ALIVE_RESPONSE;
	(*pResponsesPacket) << u2PostCommandID;
	return pResponsesPacket;
}

//包发送错误回应包
IBuffPacket* CiConnectCommand::MakeServerDownPacket( uint16 DestServerID ,uint32 dataid)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_PACKET_SERVERDOWN;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << DestServerID;
	(*pResponsesPacket) << dataid;
	return pResponsesPacket;
}

IBuffPacket* CiConnectCommand::MakeAppDownPacket( uint16 DestServerID,uint16 SourAppID, uint16 DestAppID, uint32 dataid )
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_PACKET_APPDOWN;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << DestServerID;
	(*pResponsesPacket) << SourAppID;
	(*pResponsesPacket) << DestAppID;
	(*pResponsesPacket) << dataid;
	return pResponsesPacket;
}
int  CiConnectCommand::MakeAppDownPacket(char*& pdata, uint16 DestServerID,uint16 SourAppID, uint16 DestAppID, uint32 dataid )
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_PACKET_APPDOWN;
	uint32 packetsize = 12;
	(*pResponsesPacket) << packetsize;
	(*pResponsesPacket) << u2PostCommandID;
	(*pResponsesPacket) << DestServerID;
	(*pResponsesPacket) << SourAppID;
	(*pResponsesPacket) << DestAppID;
	(*pResponsesPacket) << dataid;

	uint32 PacketLen = pResponsesPacket->GetPacketLen();
	pdata = new char[PacketLen];
	memcpy(pdata,pResponsesPacket->GetData(),PacketLen);

	m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	return PacketLen;
}

/************************************************************************/
/*  消息处理                                                            */
/************************************************************************/
void CiConnectCommand::BroadcastAppChange2Server(uint16 Appid, bool isdelete)
{
	for(mapServerInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
		iter != icServerConfig::instance()->m_mapServerInfo.end();
		++iter)
	{
		if (iter->second.isonline())
		{
			char* Packet = NULL;
			int size = MakeSynAppPacket(Packet,Appid,isdelete);
			SendData(iter->second.m_Serverid, Packet, size ,true);
		}
		else
		{
			iter->second.m_NeedRefeshApp = true;
		}
	}
}
void CiConnectCommand::BroadcastAppChange2LocalApp(uint16 Serverid, uint16 Appid, bool isdelete)
{
	OUR_DEBUG((LM_ERROR, "广播 %d 号服务器 App （%d）状态变更.\n",Serverid,Appid));
	uint16 u2PostCommandID = COMMAND_SYN_APP;
	for(mapAppInfo::iterator iter = icServerConfig::instance()->m_Server.m_mapAppInfo.begin();
		iter != icServerConfig::instance()->m_Server.m_mapAppInfo.end();
		++iter)
	{
		uint16 desAppid = iter->second.m_Appid;
		uint32 Connectid = getAppconnect(desAppid);
		IBuffPacket* pResponsesPacket = MakeSynAppPacket(Serverid,Appid,isdelete);
		if (Connectid != 0)
		{
			OUR_DEBUG((LM_ERROR, "广播至Connectid %d Appid %d.\n",Connectid,desAppid));
			PostMessage(Connectid, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
		}
	}
}
bool CiConnectCommand::handle_AppReg(uint32 ConnectID,uint16 AppID,VCHARB_STR strsModule)
{
	mapAppInfo::iterator iter = icServerConfig::instance()->m_Server.m_mapAppInfo.find(AppID);
	if(iter!= icServerConfig::instance()->m_Server.m_mapAppInfo.end()) 
	{
		//发送注册失败回应包
		IBuffPacket* pResponsesPacket = MakeRegRejectRespPacket();
		uint16 u2PostCommandID = COMMAND_REG_RESPONSE;
		PostMessage(ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
		return false;
	}

	else
	{
		//本地处理，加入列表，同步
		_icConnectInfo connectInfo;
		connectInfo.m_ConnectID = ConnectID;
		connectInfo.m_Appid = AppID;
		connectInfo.m_atvTime = ACE_OS::gettimeofday();
		icServerConfig::instance()->m_mapConnectInfo[ConnectID] = connectInfo; //加入连接

		_icAppInfo appinfo;
		appinfo.m_Appid = AppID;
		memcpy(&appinfo.m_AppName[0], strsModule.text, strsModule.u4Len);
		appinfo.m_atvTime = ACE_OS::gettimeofday();
		icServerConfig::instance()->m_Server.m_mapAppInfo[AppID]=appinfo; //加入App
		//同步所有服务器
		BroadcastAppChange2Server(AppID,false);
		BroadcastAppChange2LocalApp(icServerConfig::instance()->m_Server.m_Serverid,AppID,false);

		//发送注册成功回应包
		IBuffPacket* pRegResponsesPacket = MakeRegAcceptRespPacket();
		uint16 u2PostCommandID = COMMAND_REG_RESPONSE;
		PostMessage(ConnectID, pRegResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);

		//发送App查询回应包

		for(mapServerInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
			iter != icServerConfig::instance()->m_mapServerInfo.end();
			++iter)
		{
			IBuffPacket* pResponsesPacket = MakeQueryAppRespPacket(iter->first);
			uint16 u2PostCommandID = COMMAND_QUERY_APP_RESPONSE;
			PostMessage(ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
		}
		IBuffPacket* pResponsesPacket = MakeQueryAppRespPacket();
		u2PostCommandID = COMMAND_QUERY_APP_RESPONSE;
		PostMessage(ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
		return true;
	}
}

void CiConnectCommand::handle_ClientDisConnect(uint32 ConnectID)
{
	int appid = icServerConfig::instance()->m_mapConnectInfo[ConnectID].m_Appid;
	int appcount = 0;
	for (mapConnectInfo::iterator iter = icServerConfig::instance()->m_mapConnectInfo.begin();
		iter != icServerConfig::instance()->m_mapConnectInfo.end();
		++iter)
	{
		if (iter->second.m_Appid == appid) ++appcount;
	}
	if(1 == appcount)
	{
		//删除本地app，并发送同步数据包
		icServerConfig::instance()->m_Server.m_mapAppInfo.erase(appid);
		BroadcastAppChange2Server(appid,true);
		BroadcastAppChange2LocalApp(icServerConfig::instance()->m_Server.m_Serverid,appid,true);
	}
	icServerConfig::instance()->m_mapConnectInfo.erase(ConnectID);
}

int CiConnectCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
	if (!CheckpMessage(pMessage)) return -1;
	//基本数据
    uint16 Cmd = pMessage->GetMessageBase()->m_u2Cmd;
    uint32 ConnectID = pMessage->GetMessageBase()->m_u4ConnectID;
	_ClientIPInfo ClientIPInfo = m_pServerObject->GetConnectManager()->GetClientIPInfo(ConnectID);

	//读取数据
	IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);
	pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

	//处理链接建立信息
	if(Cmd == CLIENT_LINK_CONNECT)
	{
		OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] CLIENT 已连接.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort));
	}
	//处理链接断开信息
	else if(Cmd == CLIENT_LINK_CDISCONNET)
	{
		OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] CLIENT 已断开.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort));
		if (icServerConfig::instance()->m_mapConnectInfo.find(ConnectID) != icServerConfig::instance()->m_mapConnectInfo.end())
		{
			handle_ClientDisConnect(ConnectID);
		}
	}
	//处理注册包
	else if(Cmd == COMMAND_REG)
	{
		uint16     u2CommandID  = 0;
		uint16     AppID  = 0;
		uint16     AppType  = 0;
		VCHARB_STR strsModule;

		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> AppID;   
		(*pBodyPacket) >> AppType;   
		(*pBodyPacket) >> strsModule;      //从流里面读出Module字段。


		//处理数据
		if (handle_AppReg(ConnectID,AppID,strsModule))
		{
			char strModule[MAX_BUFF_1024 ] = {'\0'};
			memcpy(&strModule[0], strsModule.text, strsModule.u4Len);
			OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] (%s) 注册成功.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort,(char* )strModule));
		}
		else
		{
			char strModule[MAX_BUFF_1024 ] = {'\0'};
			memcpy(&strModule[0], strsModule.text, strsModule.u4Len);
			OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] (%s) 注册失败，不允许重复注册.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort,(char* )strModule));
		}
	}
	//查询服务器
	else if(Cmd == COMMAND_QUERY_SERVER)
	{
		OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] 查询服务器.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort));

		IBuffPacket* pResponsesPacket = MakeQueryServerRespPacket();
		uint16 u2PostCommandID = COMMAND_QUERY_SERVER_RESPONSE;
		PostMessage(ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
	}
	//查询APP
	else if(Cmd == COMMAND_QUERY_APP)
	{
		OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] 查询APP状态.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort));

		uint16 u2CommandID  = 0;
		uint16 ServerID  = 0;
		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> ServerID;

		char* Packet = NULL;
		int size = MakeQueryAppRespPacket(Packet);
		SendData(ServerID, Packet, size ,true);
	}
	//查询APP结果包
	else if(Cmd == COMMAND_QUERY_APP_RESPONSE)
	{
		OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] 更新APP.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort));

		uint16 u2CommandID  = 0;
		uint16 ServerID  = 0;
		uint16 u2AppCount = 0;
		uint16 Appid = 0;
		uint8 isadd = 0;

		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> ServerID;   
		(*pBodyPacket) >> u2AppCount; 
		icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo.clear();
		for (int i = 0; i<u2AppCount; ++i)
		{
			(*pBodyPacket) >> Appid;
			(*pBodyPacket) >> isadd;
			_icAppInfo appinfo;
			appinfo.m_Appid = Appid;
			icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo[Appid]=appinfo;
			BroadcastAppChange2LocalApp(ServerID,Appid,(isadd==0));
		}
		if (u2CommandID == COMMAND_QUERY_APP_RESPONSE) icServerConfig::instance()->m_mapServerInfo[ServerID].m_NeedRefeshApp = false;
	}
	//同步APP结果包
	else if(Cmd == COMMAND_SYN_APP)
	{
		OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] 同步APP.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort));

		uint16 u2CommandID  = 0;
		uint16 ServerID  = 0;
		uint16 u2AppCount = 0;
		uint16 Appid = 0;
		uint8 isadd = 0;

		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> ServerID;   
		(*pBodyPacket) >> u2AppCount; 
		for (int i = 0; i<u2AppCount; ++i)
		{
			(*pBodyPacket) >> Appid;   
			(*pBodyPacket) >> isadd;
			mapAppInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo.find(Appid);
			if (0 == isadd)
			{
			 	if(iter!=icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo.end()) 
					icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo.erase(iter);
			}
			else
			{
				if(iter==icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo.end()) 
				{
					_icAppInfo appinfo;
					appinfo.m_Appid = Appid;
				    icServerConfig::instance()->m_mapServerInfo[ServerID].m_mapAppInfo[Appid]=appinfo;
				}
			}
			BroadcastAppChange2LocalApp(ServerID,Appid,(isadd==0));
		}
	}
	//处理心跳包
	else if(Cmd == COMMAND_KEEP_ALIVE)
	{
		//OUR_DEBUG((LM_ERROR, "[包处理][ConnectID:%d ip:%s port:%d.] 心跳.\n",ConnectID,ClientIPInfo.m_szClientIP,ClientIPInfo.m_nPort));

		//本地处理，更新刷新时间
		if(icServerConfig::instance()->m_mapConnectInfo.find(ConnectID) != icServerConfig::instance()->m_mapConnectInfo.end())
		{
			uint32 AppID = icServerConfig::instance()->m_mapConnectInfo[ConnectID].m_Appid;
			if(icServerConfig::instance()->m_Server.m_mapAppInfo.find(AppID)!=icServerConfig::instance()->m_Server.m_mapAppInfo.end())
			{
				icServerConfig::instance()->m_Server.m_mapAppInfo[AppID].m_atvTime = ACE_OS::gettimeofday();
			}
			icServerConfig::instance()->m_mapConnectInfo[ConnectID].m_atvTime = ACE_OS::gettimeofday();
		}

		IBuffPacket* pResponsesPacket = MakeHeartRespPacket();
		uint16 u2PostCommandID = COMMAND_KEEP_ALIVE_RESPONSE;
		PostMessage(ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
	}
	else if(Cmd == COMMAND_PACKET_APPDOWN)
	{
		uint16 u2CommandID  = 0;
		uint16 Serverid  = 0;
		uint16 SourAppID  = 0;
		uint16 DestAppID  = 0;
		uint32 dataid = 0;

		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> Serverid;
		(*pBodyPacket) >> SourAppID;
		(*pBodyPacket) >> DestAppID;
		(*pBodyPacket) >> dataid;

		uint32 connectid = getAppconnect(SourAppID);
		if (connectid != 0)
		{
			IBuffPacket* pResponsesPacket = MakeAppDownPacket(Serverid,SourAppID, DestAppID, dataid);
			uint16 u2PostCommandID = COMMAND_PACKET_APPDOWN;
			PostMessage(connectid, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
		}
	}

	//处理数据包
	else if(Cmd == COMMAND_PACKET)
	{
		uint16 u2CommandID  = 0;
		uint16 SourServerID  = 0;
		uint16 DestServerID  = 0;
		uint16 SourAppID  = 0;
		uint16 DestAppID  = 0;

		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> SourServerID;
		(*pBodyPacket) >> DestServerID;
		(*pBodyPacket) >> SourAppID;
		(*pBodyPacket) >> DestAppID;
		if(DestServerID == icServerConfig::instance()->m_Server.m_Serverid)
		{
			uint32 connectid = getAppconnect(DestAppID);
			if (connectid != 0)
			{
				IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
				uint16 u2PostCommandID = COMMAND_PACKET;
				pResponsesPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);
				PostMessage(connectid, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
			}
			else
			{
				uint32 connectid = 0;
				uint32 dataid = 0;
				(*pBodyPacket) >> connectid;
				(*pBodyPacket) >> dataid;
				if(SourServerID == DestServerID)
				{
					IBuffPacket* pResponsesPacket = MakeAppDownPacket(DestServerID,SourAppID, DestAppID, dataid);
					uint16 u2PostCommandID = COMMAND_PACKET_APPDOWN;
					PostMessage(ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
				}
				else
				{
					char* Packet = NULL;
					int size = MakeAppDownPacket(Packet,DestServerID,SourAppID, DestAppID, dataid);
					SendData(SourServerID, Packet, size ,true);	
				}
			}
		}
		else
		{
			if(icServerConfig::instance()->m_mapServerInfo[DestServerID].isonline())
			{
				uint32 PacketLen = pBodyPacket->GetPacketLen();
				char* pdata = new char[PacketLen+4];
				memcpy(pdata,&PacketLen,4);
				memcpy(pdata+4,pBodyPacket->GetData(),PacketLen);

				SendData(DestServerID,pdata,PacketLen+4,true);
			}
			else
			{
				uint32 connectid = 0;
				uint32 dataid = 0;
				(*pBodyPacket) >> connectid;
				(*pBodyPacket) >> dataid;

				IBuffPacket* pResponsesPacket = MakeServerDownPacket(DestServerID, dataid);
				uint16 u2PostCommandID = COMMAND_PACKET_SERVERDOWN;
				PostMessage(ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
			}
		}
	}
	else if((Cmd == COMMAND_PACKET_RECEIVED) || (Cmd == COMMAND_PACKET_ERR))
	{
		uint16 u2CommandID  = 0;
		uint16 SourServerID  = 0;
		uint16 DestServerID  = 0;
		uint16 SourAppID  = 0;
		uint16 DestAppID  = 0;

		(*pBodyPacket) >> u2CommandID;   //从流里面读出u2CommandID数值
		(*pBodyPacket) >> SourServerID;
		(*pBodyPacket) >> DestServerID;
		(*pBodyPacket) >> SourAppID;
		(*pBodyPacket) >> DestAppID;
		if(DestServerID == icServerConfig::instance()->m_Server.m_Serverid)
		{
			uint32 connectid = getAppconnect(DestAppID);
			IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
			uint16 u2PostCommandID = u2CommandID;
			pResponsesPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);
			PostMessage(connectid, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, true);
		}
		else
		{
			uint32 PacketLen = pBodyPacket->GetPacketLen();
			char* pdata = new char[PacketLen+4];
			memcpy(pdata,&PacketLen,4);
			memcpy(pdata+4,pBodyPacket->GetData(),PacketLen);

			SendData(DestServerID,pdata,PacketLen+4,true);
		}
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[iConnectCommand::DoMessage][未处理的命令]%d",Cmd));
	}


	m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

	return 0;
}





/************************************************************************/
/*  服务器定时系统                                                      */
/************************************************************************/

void CiConnectCommand::CancelCheckServerConnect()
{
	if(m_nTimerID != -1)
	{
		m_ActiveTimer.cancel(m_nTimerID);
		m_nTimerID = -1;
	}
}

bool CiConnectCommand::StartCheckServerConnect(int nIntervalTime)
{
	CancelCheckServerConnect();
	m_nTimerID = m_ActiveTimer.schedule(this,NULL,ACE_OS::gettimeofday() + ACE_Time_Value(nIntervalTime), ACE_Time_Value(nIntervalTime));
	if(m_nTimerID == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CiConnectCommand::StartCheckServerConnect].定时器启动失败, 间隔时间 (%d) 秒.\n", nIntervalTime));
		return false;
	}
	m_nTimerID = m_ActiveTimer.activate();
	return true;
}
int CiConnectCommand::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
	vector<int> outtimeConnect;
	vector<int> outtimeApp;

	for(mapServerInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
		iter != icServerConfig::instance()->m_mapServerInfo.end();
		++iter)
	{
		_ClientConnectInfo ClientConnectInfo;
		_ClientConnectInfo* pClientConnectInfo = &ClientConnectInfo;
		bool online = iter->second.isonline();
		if (online)
		{
			//往中间服务器发送心跳消息
			char* PostData = NULL;
			int size = MakeHeartPacket(PostData);
			SendData(iter->second.m_Serverid, PostData, size ,true);
			//往中间服务器请求App状态
			if(iter->second.m_NeedRefeshApp)
			{
				char* Packet = NULL;
				int size = MakeQueryAppPacket(Packet,icServerConfig::instance()->m_Server.m_Serverid);
				SendData(iter->second.m_Serverid, Packet, size ,true);
			}
		}
		else
		{
			iter->second.m_NeedRefeshApp = true;
			iter->second.m_mapAppInfo.clear();
		}
		if (iter->second.m_preonline != online)
		{
			iter->second.m_preonline = online;
			BroadCastServerState();
		}
	}

	for(mapConnectInfo::iterator iter = icServerConfig::instance()->m_mapConnectInfo.begin();
		iter != icServerConfig::instance()->m_mapConnectInfo.end();
		++ iter)
	{
		if ((ACE_OS::gettimeofday().sec() - iter->second.m_atvTime.sec()) > 60) outtimeConnect.push_back(iter->first);
		OUR_DEBUG((LM_ERROR, "[服务器当前连接]ConnectID =(%d), Appid =(%d) .\n", iter->second.m_ConnectID,iter->second.m_Appid));
	}
	//删除超时的连接
	for(int i = 0; i < (int)outtimeConnect.size(); i++)
	{
        icServerConfig::instance()->m_mapConnectInfo.erase(outtimeConnect[i]);
	}
	for(mapAppInfo::iterator iter = icServerConfig::instance()->m_Server.m_mapAppInfo.begin();
		iter != icServerConfig::instance()->m_Server.m_mapAppInfo.end();
		++ iter)
	{
		if ((ACE_OS::gettimeofday().sec() - iter->second.m_atvTime.sec()) > 60) outtimeApp.push_back(iter->first);
		OUR_DEBUG((LM_ERROR, "[服务器当前APP]Appid =(%d), AppName =(%s) .\n", iter->second.m_Appid, iter->second.m_AppName));
	}
	//删除超时的App
	for(int i = 0; i < (int)outtimeApp.size(); i++)
	{
		icServerConfig::instance()->m_Server.m_mapAppInfo.erase(outtimeApp[i]);
	}

	// 状态显示
	for(mapServerInfo::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
		iter != icServerConfig::instance()->m_mapServerInfo.end();
		++iter)
	{
		OUR_DEBUG((LM_ERROR, "[远程服务器]Serverid =(%d), Stationid=(%s) \n", iter->second.m_Serverid, iter->second.m_szStationID));
		for(mapAppInfo::iterator iter2 = iter->second.m_mapAppInfo.begin();
			iter2 != iter->second.m_mapAppInfo.end();
			++ iter2)
		{
			OUR_DEBUG((LM_ERROR, "Appid =(%d)\n,", iter2->second.m_Appid));
		}
	}



	return 0;
}

