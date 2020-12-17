#include "UDPConnectManager.h"

uint32 CUDPConnectIDManager::GetConnetID(string strRemoteIP, uint16 u2RemotePort, bool& blNew)
{
	//组成字符串
	string strKey = fmt::format("{0}:{1}", strRemoteIP, u2RemotePort);

	mapConnectManager::iterator f = m_mapConnectManager.find(strKey);

	if (f == m_mapConnectManager.end())
	{
		uint32 u4NewConnectID = App_ConnectCounter::instance()->CreateCounter();
		m_mapConnectManager.insert(make_pair(strKey, u4NewConnectID));
		blNew = true; 
		m_mapConnectID[u4NewConnectID] = strKey;
		return u4NewConnectID;
	}
	else
	{
		blNew = false;
		return f->second;
	}
}

void CUDPConnectIDManager::DeleteConnectID(uint32 u4ConnectID)
{
	mapConnectID::iterator f = m_mapConnectID.find(u4ConnectID);
	if (f != m_mapConnectID.end())
	{
		string strKey = f->second;
		m_mapConnectID.erase(f);

		mapConnectManager::iterator d = m_mapConnectManager.find(strKey);
		if (d != m_mapConnectManager.end())
		{
			m_mapConnectManager.erase(d);
		}
	}
}

_ClientIPInfo CUDPConnectIDManager::GetConnectIP(uint32 u4ConnectID)
{
	_ClientIPInfo objClientInfo;
	mapConnectID::iterator f = m_mapConnectID.find(u4ConnectID);
	if (f != m_mapConnectID.end())
	{
		string strKey = f->second;

		//拆解字符串
		vector<string> vecIPInfo;
		str_split(strKey, ":", vecIPInfo);

		objClientInfo.m_strClientIP = vecIPInfo[0];
		objClientInfo.m_u2Port = (uint16)ACE_OS::atoi(vecIPInfo[1].c_str());
	}

	return objClientInfo;
}

void CUDPConnectIDManager::GetClientConnectInfo(vecClientConnectInfo& VecClientConnectInfo) const
{
	_ClientConnectInfo objClientConnectInfo;

	//遍历所有的UDP客户端地址信息
	for (const auto& kv : m_mapConnectManager) 
	{
		string strKey = kv.first;
		uint32 u4ConnectID = kv.second;

		//拆解字符串
		_ClientIPInfo objClientInfo;
		vector<string> vecIPInfo;
		str_split(strKey, ":", vecIPInfo);

		objClientInfo.m_strClientIP = vecIPInfo[0];
		objClientInfo.m_u2Port = (uint16)ACE_OS::atoi(vecIPInfo[1].c_str());

		objClientConnectInfo.m_u4ConnectID = u4ConnectID;
		objClientConnectInfo.m_addrRemote.set(objClientInfo.m_u2Port,
			objClientInfo.m_strClientIP.c_str());

		VecClientConnectInfo.push_back(objClientConnectInfo);
	}
}

