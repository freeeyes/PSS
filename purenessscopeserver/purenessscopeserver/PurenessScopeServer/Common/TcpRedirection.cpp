#include "TcpRedirection.h"

CTcpRedirection::CTcpRedirection() : m_u2Count(0), m_pClientManager(NULL), m_pConnectManager(NULL)
{

}

CTcpRedirection::~CTcpRedirection()
{
    Close();
}

void CTcpRedirection::Close()
{
    //删除hash表空间
    m_objRedirectList.Close();

    m_u2Count = 0;
}

void CTcpRedirection::Init(xmlTcpRedirection& objCXmlTcpRedirection, uint32 u4MaxHandlerCount, IClientManager* pClientManager, IConnectManager* pConnectManager)
{
    //设置Hash表最大的数量是100
    m_objRedirectList.Init(MAX_BUFF_100);
    m_objRedirectConnectList.Init(u4MaxHandlerCount);
    m_pClientManager  = pClientManager;
    m_pConnectManager = pConnectManager;

    for (int i = 0; i < objCXmlTcpRedirection.vec.size(); i++)
    {
        xmlTcpRedirection::_RedirectionInfo* pRedirectionInfo = &objCXmlTcpRedirection.vec[i];

        m_objRedirectList.Add_Hash_Data_By_Key_Unit32(pRedirectionInfo->SrcPort, pRedirectionInfo);
    }

    m_u2Count = (uint16)objCXmlTcpRedirection.vec.size();
}

void CTcpRedirection::ConnectRedirect(uint32 u4SrcPort, uint32 u4ConnectID)
{
    if (m_u2Count == 0 || NULL == m_pClientManager)
    {
        return;
    }

    xmlTcpRedirection::_RedirectionInfo* pRedirectionInfo = m_objRedirectList.Get_Hash_Box_Data_By_Uint32(u4SrcPort);

    if (NULL != pRedirectionInfo)
    {
        //连接远程服务器
        CRedirectionData* pRedirectionData = new CRedirectionData();
        pRedirectionData->SetServerID(u4ConnectID);
        pRedirectionData->SetConnectManager(m_pConnectManager);
        pRedirectionData->SetMode(pRedirectionInfo->Mode);

        m_objRedirectConnectList.Add_Hash_Data_By_Key_Unit32(u4ConnectID, pRedirectionData);

        m_pClientManager->Connect(u4ConnectID,
                                  pRedirectionInfo->RedirectionIP.c_str(),
                                  pRedirectionInfo->RedirectionPort,
                                  TYPE_IPV4,
                                  (IClientMessage*)pRedirectionData);
    }
}

void CTcpRedirection::DataRedirect(uint32 u4ConnectID, ACE_Message_Block* mb)
{
    if (m_u2Count == 0 || NULL == m_pClientManager)
    {
        return;
    }

    CRedirectionData* pRedirectionData = m_objRedirectConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if (NULL != pRedirectionData)
    {
        char* pData = (char*)mb->rd_ptr();
        m_pClientManager->SendData(u4ConnectID, pData, (int)mb->length(), false);
    }
}

void CTcpRedirection::CloseRedirect(uint32 u4ConnectID)
{
    if (m_u2Count == 0 || NULL == m_pClientManager)
    {
        return;
    }

    //回收内存数据
    CRedirectionData* pRedirectionData = m_objRedirectConnectList.Get_Hash_Box_Data_By_Uint32(u4ConnectID);

    if (NULL != pRedirectionData)
    {
        SAFE_DELETE(pRedirectionData);
        m_objRedirectConnectList.Del_Hash_Data_By_Unit32(u4ConnectID);
        m_pClientManager->Close(u4ConnectID);
    }
}
