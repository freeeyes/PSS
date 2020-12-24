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
    CPostServerData() = default;

    ~CPostServerData() final = default;

    bool Need_Send_Format() final
    {
        //如果需要调用发送组装函数，则这里返回true
        //不需要的话这里返回false，Send_Format_data函数就不会被调用
        return true;
    }

    //处理服务期间发送数据的组装
    bool Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend) final
    {
        //判断缓冲池是否存在，如果不存在则返回失败
        if(nullptr == pMessageBlockManager)
        {
            return false;
        }

        //申请发送的内存
        mbSend = pMessageBlockManager->Create(u4Len);

        if(nullptr == mbSend)
        {
            PSS_LOGGER_DEBUG("[CPostServerData::Send_Format_data]({0})Format Create Data nullptr({1}) OK.", m_u4ServerID, u4Len);
            return false;
        }

        //如果需要组装数据，在这里写你的代码
        //这里只是用例，只做简单的内存拷贝，演示完整的操作流程
        memcpy_safe(pData, u4Len, mbSend->wr_ptr(), u4Len);
        mbSend->wr_ptr(u4Len);
        return true;
    }

    //这里提供接受数据拼包算法，组成完整数据包后会调用RecvData方法
    bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute) final
    {
        emPacketRoute = EM_PACKET_ROUTE::PACKET_ROUTE_SELF;
        u2CommandID = 0x1000;

        //判断返回数据块是否小于8或者超过最大缓冲大小
        if(m_u2RecvBuffLength + mbRecv->length() < 8 || mbRecv->length() >= RECV_BUFF_SIZE)
        {
            PSS_LOGGER_DEBUG("[CPostServerData::Recv_Format_data]Get Data Error({0}).", mbRecv->length());
            return false;
        }
        else
        {
            //统一贴入缓冲，再有缓冲切割发送数据
            m_u2RecvBuffLength += (uint16)mbRecv->length();
        }



        //这里添加完整数据包算法
        uint32 u4Count = 0;
        memcpy_safe(mbRecv->rd_ptr(), sizeof(uint32), (char*)&u4Count, sizeof(uint32));
        mbRecv->rd_ptr(sizeof(uint32));

        //记录数据包图
        uint32 u4RecvFinish = u4Count;

        if (u4RecvFinish <= m_u2RecvBuffLength)
        {
            //完整数据包处理
            mbFinishRecv = pMessageBlockManager->Create(u4RecvFinish);
            memcpy_safe(mbRecv->rd_ptr(), u4RecvFinish, mbFinishRecv->rd_ptr(), u4RecvFinish);
            mbFinishRecv->wr_ptr(u4RecvFinish);
            mbRecv->rd_ptr(u4RecvFinish);

            m_u2RecvBuffLength -= u4RecvFinish;
        }

        return true;
    }

    bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv,  const _ClientIPInfo& objServerIPInfo) final
    {
        //数据包已经收全，在这里处理数据
        ACE_UNUSED_ARG(u2CommandID);
        ACE_UNUSED_ARG(mbRecv);
        ACE_UNUSED_ARG(objServerIPInfo);

        return true;
    }

    void ReConnect(int nServerID) final
    {
        PSS_LOGGER_DEBUG("[CPostServerData::ReConnect]nServerID={0}.", nServerID);
    }

    bool ConnectError(int nError, _ClientIPInfo const& objServerIPInfo) final
    {
        ACE_UNUSED_ARG(objServerIPInfo);
        PSS_LOGGER_DEBUG("[CPostServerData::ConnectError]nServerID={0}, nError={1}.", m_u4ServerID, nError);
        return true;
    }

    void SetServerID(uint32 u4ServerID)
    {
        m_u4ServerID = u4ServerID;
    }

private:
    uint32                     m_u4ServerID                     = 0;
    uint16                     m_u2RecvBuffLength               = 0;       //接收缓冲长度
};

#endif
