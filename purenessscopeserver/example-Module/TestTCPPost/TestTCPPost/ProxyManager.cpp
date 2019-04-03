#include "ProxyManager.h"

CProxyManager::CProxyManager()
{
    m_u2WorkCount = 0;
}

CProxyManager::~CProxyManager()
{
    Close();
}

void CProxyManager::Close()
{
    for(int i = 0; i < (int)m_objc2sGroup.size(); i++)
    {
        //断开所有服务器间连接
        mapc2s* pmapc2s = (mapc2s* )m_objc2sGroup[i];

        for(mapc2s::iterator b = pmapc2s->begin(); b != pmapc2s->end(); b++)
        {
            CPostServerData* pPostServerData = (CPostServerData* )b->second;
            pPostServerData->Close(S2S_INNEED_CALLBACK);
            SAFE_DELETE(pPostServerData);
        }

        pmapc2s->clear();
        SAFE_DELETE(pmapc2s);
    }

    m_objc2sGroup.clear();
    m_u2WorkCount = 0;
}

void CProxyManager::Init(int nCount)
{
    Close();
    OUR_DEBUG((LM_ERROR, "[CProxyManager::Init]WorkThread nCount=%d.\n", nCount));

    for(int i = 0; i < nCount; i++)
    {
        mapc2s* pmapc2s = new mapc2s();
        m_objc2sGroup.push_back(pmapc2s);
    }

    m_u2WorkCount = (uint16)nCount;
}

void CProxyManager::AddClient2Server(uint32 u4ClientID, CServerObject* pServerObject)
{
    int nIndex = u4ClientID % m_u2WorkCount;
    mapc2s* pmapc2s = (mapc2s* )m_objc2sGroup[nIndex];

    if(NULL != pmapc2s)
    {
        mapc2s:: iterator f = pmapc2s->find(u4ClientID);

        if(f != pmapc2s->end())
        {
            return;
        }

        CPostServerData* pPostServerData = new CPostServerData();

        //设置返回客户端需要的发送对象
        pPostServerData->SetServerObject(pServerObject);
        pPostServerData->SetConnectID(u4ClientID);
        pPostServerData->SetServerID(u4ClientID);

        //初始化连接关系
        pServerObject->GetClientManager()->Connect(u4ClientID, REMOTE_SERVER_IP, REMOTE_SERVER_PORT, TYPE_IPV4, (IClientMessage* )pPostServerData);

        pmapc2s->insert(mapc2s::value_type(u4ClientID, pPostServerData));
    }
}

void CProxyManager::DelClient2Server(uint32 u4ClientID)
{
    int nIndex = u4ClientID % m_u2WorkCount;
    mapc2s* pmapc2s = (mapc2s* )m_objc2sGroup[nIndex];

    if(NULL != pmapc2s)
    {
        mapc2s::iterator f = pmapc2s->find(u4ClientID);

        if(f == pmapc2s->end())
        {
            return;
        }

        CPostServerData* pPostServerData = (CPostServerData* )f->second;

        if(NULL != pPostServerData)
        {
            pPostServerData->Close(S2S_INNEED_CALLBACK);
            SAFE_DELETE(pPostServerData);
        }

        pmapc2s->erase(f);
    }
}

CPostServerData* CProxyManager::GetClient2Server_ServerID(uint32 u4ClientID)
{
    int nIndex = u4ClientID % m_u2WorkCount;
    mapc2s* pmapc2s = (mapc2s* )m_objc2sGroup[nIndex];

    if(NULL != pmapc2s)
    {
        mapc2s::iterator f = pmapc2s->find(u4ClientID);

        if(f == pmapc2s->end())
        {
            return NULL;
        }
        else
        {
            return (CPostServerData* )f->second;
        }
    }
    else
    {
        return NULL;
    }
}

void CProxyManager::Display(uint32 u4ClientID)
{
    int nIndex = u4ClientID % m_u2WorkCount;
    mapc2s* pmapc2s = (mapc2s* )m_objc2sGroup[nIndex];

    if(NULL != pmapc2s)
    {
        OUR_DEBUG((LM_ERROR, "[CProxyManager::Display]size=%d.", pmapc2s->size()));

        for(mapc2s::iterator b = pmapc2s->begin(); b != pmapc2s->end(); b++)
        {
            uint32 u4ConnectID = (uint32)b->first;
            CPostServerData* pPostServerData = (CPostServerData* )b->second;
            OUR_DEBUG((LM_ERROR, "[CProxyManager::Display]ServerID=%d, pPostServerData=0x%08x.\n", u4ConnectID, pPostServerData));
        }
    }
}
