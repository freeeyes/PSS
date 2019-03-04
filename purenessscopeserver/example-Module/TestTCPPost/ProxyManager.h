#ifndef PROXYMANAGER_H
#define PROXYMANAGER_H

//管理所有的透传连接

#include "PostServerLogic.h"

#include <string>
#include <map>

using namespace std;

#define REMOTE_SERVER_IP    "172.21.0.41"
#define REMOTE_SERVER_PORT  10040

typedef map<uint32, CPostServerData*> mapc2s;
typedef vector<mapc2s*> vecc2sGroup;

class CProxyManager
{
public:
    CProxyManager();
    ~CProxyManager();

    void Close();
    void Init(int nCount);

    void AddClient2Server(uint32 u4ClientID, CServerObject* pServerObject);
    void DelClient2Server(uint32 u4ClientID);
    CPostServerData* GetClient2Server_ServerID(uint32 u4ClientID);

    void Display(uint32 u4ClientID);

private:
    vecc2sGroup m_objc2sGroup;
    uint16      m_u2WorkCount;
};

#endif
