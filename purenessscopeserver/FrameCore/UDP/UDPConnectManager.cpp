#include "UDPConnectManager.h"

uint32 CUDPConnectIDManager::GetConnetID(string strRemoteIP, uint16 u2RemotePort, bool& blNew)
{
	//×é³É×Ö·û´®
	stringstream ss_format;
	ss_format << strRemoteIP << ":" << u2RemotePort;

	string strKey = ss_format.str();

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

		//²ð½â×Ö·û´®
		vector<string> vecIPInfo;
		str_split(strKey, ":", vecIPInfo);

		objClientInfo.m_strClientIP = vecIPInfo[0];
		objClientInfo.m_u2Port = (uint16)ACE_OS::atoi(vecIPInfo[1].c_str());
	}

	return objClientInfo;
}

