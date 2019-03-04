#ifndef POSTSERVER_MESSAGE_H
#define POSTSERVER_MESSAGE_H

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "ClientReConnectManager.h"

//此类处理所有透传数据相关逻辑

#define RECV_BUFF_SIZE  MAX_BUFF_1024      //接收缓冲大小

class CPostServerData : public IClientMessage
{
public:
    CPostServerData()
    {
        m_u4ServerID         = 0;
        m_u2RecvBuffLength   = 0;
    }

    virtual ~CPostServerData()
    {
    }

    virtual bool Need_Send_Format()
    {
        //如果需要调用发送组装函数，则这里返回true
        //不需要的话这里返回false，Send_Format_data函数就不会被调用
        return true;
    }

    //处理服务期间发送数据的组装
    virtual bool Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)
    {
        //判断缓冲池是否存在，如果不存在则返回失败
        if(NULL == pMessageBlockManager)
        {
            return false;
        }

        //申请发送的内存
        mbSend = pMessageBlockManager->Create(u4Len);

        if(NULL == mbSend)
        {
            OUR_DEBUG((LM_INFO, "[CPostServerData::Send_Format_data](%d)Format Create Data NULL(%d) OK.\n", m_u4ServerID, u4Len));
            return false;
        }

        //如果需要组装数据，在这里写你的代码
        //这里只是用例，只做简单的内存拷贝，演示完整的操作流程
        memcpy_safe(pData, u4Len, mbSend->wr_ptr(), u4Len);
        mbSend->wr_ptr(u4Len);
        return true;
    }

    //这里提供接受数据拼包算法，组成完整数据包后会调用RecvData方法
    virtual bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute)
    {
        emPacketRoute = PACKET_ROUTE_SELF;
        u2CommandID = 0x1000;

        //判断返回数据块是否小于8或者超过最大缓冲大小
        if(m_u2RecvBuffLength + mbRecv->length() < 8 || mbRecv->length() >= RECV_BUFF_SIZE)
        {
            OUR_DEBUG((LM_INFO, "[CPostServerData::Recv_Format_data]Get Data Error(%d).\n", mbRecv->length()));
            return false;
        }
        else
        {
            //统一贴入缓冲，再有缓冲切割发送数据
            ACE_OS::memcpy(&m_szRecvBuffData[m_u2RecvBuffLength], mbRecv->rd_ptr(), mbRecv->length());
            m_u2RecvBuffLength += (uint16)mbRecv->length();
            mbRecv->rd_ptr(mbRecv->length());
        }


				
        //这里添加完整数据包算法
        uint32 u4Count = 0;
        memcpy_safe(&m_szRecvBuffData[0], sizeof(uint32), (char*)&u4Count, sizeof(uint32));
        
        uint32 u4RecvFinish = u4Count + sizeof(uint32);

        if (u4RecvFinish <= m_u2RecvBuffLength)
        {
            //完整数据包处理
            mbFinishRecv = pMessageBlockManager->Create(u4RecvFinish);
            memcpy_safe(&m_szRecvBuffData[0], u4RecvFinish, mbFinishRecv->rd_ptr(), u4RecvFinish);
            mbFinishRecv->wr_ptr(u4RecvFinish);

            m_u2RecvBuffLength -= u4RecvFinish;
        }
        
        return true;
    }

    virtual bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv,  _ClientIPInfo objServerIPInfo)
    {
        //数据包已经收全，在这里处理数据
        ACE_UNUSED_ARG(u2CommandID);
        ACE_UNUSED_ARG(mbRecv);
        ACE_UNUSED_ARG(objServerIPInfo);

        return true;
    }

    virtual void ReConnect(int nServerID)
    {
        OUR_DEBUG((LM_INFO, "[CPostServerData::ReConnect]nServerID=%d.\n", nServerID));
    }

    virtual bool ConnectError(int nError, _ClientIPInfo objServerIPInfo)
    {
        ACE_UNUSED_ARG(objServerIPInfo);
        OUR_DEBUG((LM_INFO, "[CPostServerData::ConnectError]nServerID=%d, nError=%d.\n", m_u4ServerID, nError));
        return true;
    }

    void SetServerID(uint32 u4ServerID)
    {
        m_u4ServerID = u4ServerID;
    }

private:
    uint32                     m_u4ServerID;
    char                       m_szRecvBuffData[RECV_BUFF_SIZE];  //接收缓冲池
    uint16                     m_u2RecvBuffLength;                //接收缓冲长度
};

#endif
