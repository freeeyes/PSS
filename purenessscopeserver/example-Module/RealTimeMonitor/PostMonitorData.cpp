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

bool CPostServerData::Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute)
{
    OUR_DEBUG((LM_INFO, "[CPostServerData::Recv_Format_data]=====.\n"));
    ACE_UNUSED_ARG(emPacketRoute);

    if (mbRecv->length() > 0)
    {
        //这里必须把内存拷贝出来，因为mbRecv不在pMessageBlockManager里面。
        ACE_Message_Block* pmb = pMessageBlockManager->Create((uint32)mbRecv->length());
        if (NULL != pmb)
        {
            memcpy_safe(mbRecv->rd_ptr(), (uint32)mbRecv->length(), pmb->wr_ptr(), (uint32)mbRecv->length());
            pmb->wr_ptr(mbRecv->length());
            mbRecv->rd_ptr(mbRecv->length());
            mbFinishRecv = pmb;
            return true;
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CPostServerData::Recv_Format_data]pMessageBlockManager Create NULL.\n"));
            return false;
        }
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CPostServerData::Recv_Format_data]mbRecv->length() is 0.\n"));
        return false;
    }
}

void CPostServerData::ReConnect(int nServerID)
{
    //发生连接断开，需要重新发送注册包
    m_pMonitorCommand->SetState(MONITOR_STATE_CONNECT);
}

bool CPostServerData::RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo)
{
    //处理数据返回包
    if (mbRecv->length() == sizeof(uint32) + sizeof(uint16) + sizeof(uint32))
    {
        uint32 u4PacketLength = 0;
        uint16 u2CommandID    = 0;
        uint32 u4State        = 0;

        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint32), (char* )&u4PacketLength, sizeof(uint32));
        mbRecv->rd_ptr(sizeof(uint32));
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint16), (char* )&u2CommandID, sizeof(uint16));
        mbRecv->rd_ptr(sizeof(uint16));
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint32), (char*)&u4State, sizeof(uint32));
        mbRecv->rd_ptr(sizeof(uint32));

        OUR_DEBUG((LM_INFO, "[CPostServerData::RecvDat]Get Recv u2CommandID=0x%04x.\n", u2CommandID));

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

