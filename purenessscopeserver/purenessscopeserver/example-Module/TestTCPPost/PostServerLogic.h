#ifndef POSTSERVERLOGIC_H
#define POSTSERVERLOGIC_H

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"

//此类处理所有透传数据相关逻辑

#define SEND_BUFF_SIZE  MAX_BUFF_1024      //发送缓冲大小
#define RECV_BUFF_SIZE  MAX_BUFF_1024      //接收缓冲大小

class CPostServerData : public IClientMessage
{
public:
    CPostServerData()
    {
        m_pServerObject      = NULL;
        m_u4ServerID         = 0;
        m_u4ConnectID        = 0;
        m_u2RecvBuffLength   = 0;
        m_u2SendBuffLength   = 0;
    }

    ~CPostServerData()
    {
        if(m_u4ServerID != 0)
        {
            Close(S2S_INNEED_CALLBACK);
        }
    }

    bool Need_Send_Format()
    {
        //如果需要调用发送组装函数，则这里返回true
        //不需要的话这里返回false，Send_Format_data函数就不会被调用
        return true;
    }

    //处理服务期间发送数据的组装
    bool Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)
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
            OUR_DEBUG((LM_INFO, "[CPostServerData::Send_Format_data](%d)Format Create Data NULL(%d) OK.\n", m_u4ConnectID, u4Len));
            return false;
        }

        //如果需要组装数据，在这里写你的代码
        //这里只是用例，只做简单的内存拷贝，演示完整的操作流程
        memcpy_safe(pData, u4Len, mbSend->wr_ptr(), u4Len);
        mbSend->wr_ptr(u4Len);
        return true;
    }

    //这里提供接受数据拼包算法，组成完整数据包后会调用RecvData方法
    bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv)
    {
        //OUR_DEBUG((LM_INFO, "[CPostServerData::Recv_Format_data]u4Len=%d.\n", mbRecv->length()));
        //OUR_DEBUG((LM_INFO, "[CPostServerData::Recv_Format_data]m_u2RecvBuffLength=%d.\n", m_u2RecvBuffLength));
        //判断返回数据块是否小于8或者超过最大缓冲大小
        if(m_u2RecvBuffLength + mbRecv->length() < 8 || mbRecv->length() >= RECV_BUFF_SIZE)
        {
            if(mbRecv->length() > 0)
            {
                ACE_OS::memcpy(&m_szRecvBuffData[m_u2RecvBuffLength], mbRecv->rd_ptr(), mbRecv->length());
                m_u2RecvBuffLength += mbRecv->length();
                mbRecv->rd_ptr(mbRecv->length());
            }

            OUR_DEBUG((LM_INFO, "[CPostServerData::Recv_Format_data]Get Data(%d).\n", mbRecv->length()));
            return false;
        }
        else
        {
            //统一贴入缓冲，再有缓冲切割发送数据
            ACE_OS::memcpy(&m_szRecvBuffData[m_u2RecvBuffLength], mbRecv->rd_ptr(), mbRecv->length());
            m_u2RecvBuffLength += mbRecv->length();
            mbRecv->rd_ptr(mbRecv->length());
        }

        //如果接收字节固定是8字节，则认为是一个完整包
        //这里可以添加算法

        //发现一个完整8字节长的数据包
        u2CommandID = 0x1010;
        mbFinishRecv = pMessageBlockManager->Create(8);

        //创建完整数据包
        memcpy_safe(m_szRecvBuffData, 8, mbFinishRecv->wr_ptr(), 8);
        mbFinishRecv->wr_ptr(8);

        //数据缓冲向前移位
        if(m_u2RecvBuffLength - 8 > 0)
        {
            memcpy_safe(&m_szRecvBuffData[8], m_u2RecvBuffLength - 8, m_szRecvBuffData, m_u2RecvBuffLength - 8);
            m_u2RecvBuffLength -= 8;
        }
        else
        {
            m_u2RecvBuffLength = 0;
        }

        return true;
    }

    bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv,  _ClientIPInfo objServerIPInfo)
    {
        //数据包已经收全.发送给客户端数据
        //OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get Data(%d).\n", mbRecv->length()));
        if(NULL != m_pServerObject &&  mbRecv->length() > 0)
        {
            uint16 u2RetCommand = u2CommandID;
            uint32 u4PacketLength = 8;      //固定包体8字节
            char* pData = new char[u4PacketLength + sizeof(uint32)];
            ACE_OS::memcpy(pData, (char* )&u4PacketLength, sizeof(uint32));
            ACE_OS::memcpy(&pData[sizeof(uint32)], m_szRecvBuffData, u4PacketLength);

            uint32 u4SendLength = u4PacketLength + sizeof(uint32);

            const char* ptrReturnData = reinterpret_cast<const char*>(pData);
            m_pServerObject->GetConnectManager()->PostMessage(m_u4ConnectID,ptrReturnData,(uint32)u4SendLength, SENDMESSAGE_JAMPNOMAL, u2RetCommand, PACKET_SEND_IMMEDIATLY, PACKET_IS_SELF_RECYC);
            OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData](%d)Send Data(%d) OK.\n", m_u4ConnectID, u4SendLength));
        }
        else
        {
            OUR_DEBUG((LM_INFO, "[CPostServerData::RecvData]Get packet error(%s:%d).\n", objServerIPInfo.m_szClientIP, objServerIPInfo.m_nPort));
        }

        return true;
    }

    //发送数据
    bool SendData(const char* pData, uint32 u4Size)
    {
        //如果超过最大发送长度
        if(m_u2SendBuffLength + u4Size >= MAX_BUFF_1024 || u4Size >= SEND_BUFF_SIZE)
        {
            OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) send buff full.\n", m_u4ServerID));
            return false;
        }

        //得到当前连接状态
        if(SERVER_CONNECT_OK == m_pServerObject->GetClientManager()->GetConnectState(m_u4ServerID))
        {
            //将数据放入缓冲，和之前的数据(如果有的话)一起发送
            ACE_OS::memcpy(&m_szSendBuffData[m_u2SendBuffLength], pData, u4Size);
            m_u2SendBuffLength += u4Size;

            //发送数据
            OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send [%d] Begin.\n", m_u4ServerID, m_u2SendBuffLength));

            char* ptrReturnData = const_cast<char*>(m_szSendBuffData);

            if(false == m_pServerObject->GetClientManager()->SendData((int)m_u4ServerID, ptrReturnData, m_u2SendBuffLength, false))
            {
                //发送失败，缓冲
                OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send [%d] End 1.\n", m_u4ServerID, m_u2SendBuffLength));
                return false;
            }
            else
            {
                //发送成功，清理缓冲
                OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send [%d] End.\n", m_u4ServerID, m_u2SendBuffLength));
                m_u2SendBuffLength = 0;
                return true;
            }
        }
        else
        {
            OUR_DEBUG((LM_ERROR, "[CPostServerData::SendData](%d) Send Buffer.", m_u4ServerID));
            //连接尚未建立好，放入缓冲，等连接建立后自动回调发
            ACE_OS::memcpy(&m_szSendBuffData[m_u2SendBuffLength], pData, u4Size);
            m_u2SendBuffLength += u4Size;
            return true;
        }
    }

    //关闭远程服务
    bool Close(EM_s2s ems2s)
    {
        m_pServerObject->GetClientManager()->Close(m_u4ServerID);
        m_u4ServerID = 0;
        return true;
    }

    bool ConnectError(int nError,  _ClientIPInfo objServerIPInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CPostServerData::ConnectError]Get Error[%s:%d](%d).\n", objServerIPInfo.m_szClientIP, objServerIPInfo.m_nPort, nError));
        return true;
    }

    void ReConnect(int nServerID)
    {
        //数据重连成功接口
        //m_u4ServerID = (uint32)nServerID;
        OUR_DEBUG((LM_ERROR, "[CPostServerData::ReConnect]ReConnect(%d).\n", nServerID));

        if(m_u2SendBuffLength > 0)
        {
            char* ptrReturnData = const_cast<char*>(m_szSendBuffData);

            //发送数据
            if(true == m_pServerObject->GetClientManager()->SendData((int)m_u4ServerID, ptrReturnData, m_u2SendBuffLength, false))
            {
                //发送成功，清理缓冲
                m_u2SendBuffLength = 0;
            }
        }
    }

    CServerObject* GetServerObject()
    {
        return m_pServerObject;
    }

    void SetServerObject(CServerObject* pServerObject)
    {
        m_pServerObject = pServerObject;
    }

    void SetConnectID(uint32 u4ConnectID)
    {
        m_u4ConnectID = u4ConnectID;
    }

    void SetServerID(uint32 u4ServerID)
    {
        m_u4ServerID = u4ServerID;
    }

    uint32 GetServerID()
    {
        return m_u4ServerID;
    }

private:
    ACE_Recursive_Thread_Mutex m_ThreadLock;
    CServerObject*             m_pServerObject;
    uint32                     m_u4ServerID;
    uint32                     m_u4ConnectID;
    char                       m_szRecvBuffData[RECV_BUFF_SIZE];  //接收缓冲池
    char                       m_szSendBuffData[SEND_BUFF_SIZE];  //发送缓冲池
    uint16                     m_u2RecvBuffLength;                //接收缓冲长度
    uint16                     m_u2SendBuffLength;                //发送缓冲长度
};

#endif
