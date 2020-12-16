#ifndef PSS_UDP_CONNECT_MANAGER_H
#define PSS_UDP_CONNECT_MANAGER_H

//分配UDP ConnectID的信息
//add freeeyes

#include "define.h"
#include "IpCheck.h"
#include <sstream>
#include <unordered_map>
#include "ConnectCounter.h"

class CUDPConnectIDManager
{
public:
	uint32 GetConnetID(string strRemoteIP, uint16 u2RemotePort, bool& blNew);

	void DeleteConnectID(uint32 u4ConnectID);

	_ClientIPInfo GetConnectIP(uint32 u4ConnectID);

	void GetClientConnectInfo(vecClientConnectInfo& VecClientConnectInfo) const;

private:
	using mapConnectManager = unordered_map<string, uint32>;
	using mapConnectID = unordered_map<uint32, string>;
	mapConnectManager m_mapConnectManager;
	mapConnectID      m_mapConnectID;
};

using App_UDPConnectIDManager = PSS_singleton<CUDPConnectIDManager>;

#endif
