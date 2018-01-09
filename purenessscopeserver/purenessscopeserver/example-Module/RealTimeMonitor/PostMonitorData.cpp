#include "PostMonitorData.h"

CPostServerData::CPostServerData()
{

}

CPostServerData::~CPostServerData()
{

}

void CPostServerData::Init(IMonitorCommand* pMonitorCommand)
{
    m_pMonitorCommand = pMonitorCommand;
}

bool CPostServerData::Need_Send_Format()
{
    return false;
}

bool CPostServerData::Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)
{
    //暂时不用此方法
    ACE_UNUSED_ARG(pData);
    ACE_UNUSED_ARG(u4Len);
    ACE_UNUSED_ARG(pMessageBlockManager);
    ACE_UNUSED_ARG(mbSend);
    return true;
}

bool CPostServerData::Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv)
{
    mbFinishRecv = mbRecv;
    return true;
}

void CPostServerData::ReConnect(int nServerID)
{
    //发生连接断开，需要重新发送注册包
    m_pMonitorCommand->SetState(MONITOR_STATE_CONNECT);
}

bool CPostServerData::RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo)
{
    //处理数据返回包
    if (mbRecv->length() == sizeof(uint16) + sizeof(uint32))
    {
        char szTemp[MAX_BUFF_50] = { '\0' };
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint32), szTemp, sizeof(uint32));
        szTemp[sizeof(uint32)] = '\0';
        mbRecv->rd_ptr(sizeof(uint32));
        uint32 u4PacketLength = ACE_OS::atoi(szTemp);
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint16), szTemp, sizeof(uint16));
        szTemp[sizeof(uint16)] = '\0';
        mbRecv->rd_ptr(sizeof(uint16));
        uint16 u2CommandID = ACE_OS::atoi(szTemp);
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint32), szTemp, sizeof(uint32));
        szTemp[sizeof(uint32)] = '\0';
        mbRecv->rd_ptr(sizeof(uint32));
        uint32 u4State = ACE_OS::atoi(szTemp);

        if (COMMAND_MONITOR_LOGIN_ACK == u2CommandID)
        {
            if (0 == u4State)
            {
                //登录验证回执成功
                m_pMonitorCommand->SetState(MONITOR_STATE_LOGINED);
            }
        }
    }

    return true;
}

bool CPostServerData::ConnectError(int nError, _ClientIPInfo objServerIPInfo)
{
    //处理连接断开请求
    m_pMonitorCommand->SetState(MONITOR_STATE_DISCONNECT);
    return true;
}

