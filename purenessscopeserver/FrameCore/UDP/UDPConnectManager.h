#ifndef PSS_UDP_CONNECT_MANAGER_H
#define PSS_UDP_CONNECT_MANAGER_H

//分配UDP ConnectID的信息
//add freeeyes

#include "define.h"
#include "IpCheck.h"
#include <sstream>
#include <unordered_map>
#include "ConnectCounter.h"

using namespace std;

class CUDPConnectIDManager
{
public:
	uint32 GetConnetID(string strRemoteIP, uint16 u2RemotePort, bool& blNew);

	void DeleteConnectID(uint32 u4ConnectID);

	_ClientIPInfo GetConnectIP(uint32 u4ConnectID);

	void GetClientConnectInfo(vecClientConnectInfo& VecClientConnectInfo);

private:
	typedef unordered_map<string, uint32> mapConnectManager;
	typedef unordered_map<uint32, string> mapConnectID;
	mapConnectManager m_mapConnectManager;
	mapConnectID      m_mapConnectID;
};

typedef ACE_Singleton<CUDPConnectIDManager, ACE_Null_Mutex> App_UDPConnectIDManager;

#endif
