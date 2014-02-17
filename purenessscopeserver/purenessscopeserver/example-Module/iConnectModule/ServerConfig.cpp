/*
 * 服务器列表的相关管理功能
 * by w1w
 */
#include "ServerConfig.h"

CServerConfig::CServerConfig()
{
	m_szError[0] = '\0';
}
CServerConfig::~CServerConfig()
{
}
bool CServerConfig::Init()
{
	char* pData = NULL;
	OUR_DEBUG((LM_INFO, "[CServerConfig::Init]Filename = %s.\n", "main.xml"));
	if(false == m_ServerConfig.Init("main.xml"))
	{
		OUR_DEBUG((LM_INFO, "[CServerConfig::Init]File Read Error = %s.\n", "main.xml"));
		return false;
	}
	pData = m_ServerConfig.GetData("ServerID", "id");
	if(NULL != pData)
	{
		m_Server.m_Serverid = ACE_OS::atoi(pData);
	}
	pData = m_ServerConfig.GetData("ServerName", "name");
	if(NULL != pData)
	{
		sprintf_safe(m_Server.m_szServerName, MAX_BUFF_20, "%s", pData);
	}
	pData = m_ServerConfig.GetData("ServerVersion", "Version");
	if(NULL != pData)
	{
		sprintf_safe(m_Server.m_szServerVersion, MAX_BUFF_20, "%s", pData);
	}


	OUR_DEBUG((LM_INFO, "[CServerConfig::Init]Filename = %s.\n", "servers.xml"));
	if(false == m_ServerConfig.Init("servers.xml"))
	{
		OUR_DEBUG((LM_INFO, "[CServerConfig::Init]File Read Error = %s.\n", "servers.xml"));
		return false;
	}

	//读取服务器列表

	m_mapServerInfo.clear();
	TiXmlElement* pNextTiXmlElementID = NULL;
	TiXmlElement* pNextTiXmlElementIP = NULL;
	TiXmlElement* pNextTiXmlElementPort = NULL;
	TiXmlElement* pNextTiXmlElementSID = NULL;
	while(true)
	{
		_icServerInfo serverinfo;
		pData = m_ServerConfig.GetData("Server", "id", pNextTiXmlElementID);
		if(pData != NULL)
		{
			serverinfo.m_Serverid = ACE_OS::atoi(pData);
		}
		else
		{
			break;
		}

		pData = m_ServerConfig.GetData("Server", "ip", pNextTiXmlElementIP);
		if(pData != NULL)
		{
			sprintf_safe(serverinfo.m_szServerIP, MAX_BUFF_20, "%s", pData);
		}
		else
		{
			break;
		}

		pData = m_ServerConfig.GetData("Server", "stationid", pNextTiXmlElementSID);
		if(pData != NULL)
		{
			sprintf_safe(serverinfo.m_szStationID, 33, "%s", pData);
		}
		else
		{
			break;
		}

		pData = m_ServerConfig.GetData("Server", "port", pNextTiXmlElementPort);
		if(pData != NULL)
		{
			serverinfo.m_nPort = ACE_OS::atoi(pData);
		}
		else
		{
			break;
		}
		if (serverinfo.m_Serverid != m_Server.m_Serverid)
			m_mapServerInfo[serverinfo.m_Serverid]=serverinfo;
		else
		{
			m_Server.m_nPort = serverinfo.m_nPort;
			sprintf_safe(m_Server.m_szServerIP, MAX_BUFF_20, "%s", serverinfo.m_szServerIP);
			sprintf_safe(m_Server.m_szStationID, 33, "%s", serverinfo.m_szStationID);
		}
	}
	return true;
}

const char* CServerConfig::GetError()
{
	return m_szError;
}


void CServerConfig::Display()
{
	for(map<int, _icServerInfo>::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
		iter != icServerConfig::instance()->m_mapServerInfo.end();
		++iter)
	{
		OUR_DEBUG((LM_INFO, "[CServerConfig::Display]ServerID = %d.\n", iter->second.m_Serverid));
		OUR_DEBUG((LM_INFO, "[CServerConfig::Display]ServerIP = %s.\n", iter->second.m_szServerIP));
		OUR_DEBUG((LM_INFO, "[CServerConfig::Display]ServerPort = %d.\n",  iter->second.m_nPort));
		OUR_DEBUG((LM_INFO, "[CServerConfig::Display]StationID = %s.\n", iter->second.m_szStationID));
	}
}

void CServerConfig::SetClientManager(IClientManager* pClientManager)
{
	for(map<int, _icServerInfo>::iterator iter = icServerConfig::instance()->m_mapServerInfo.begin();
		iter != icServerConfig::instance()->m_mapServerInfo.end();
		++iter)
	{
		iter->second.SetClientManager(pClientManager);
	}
}