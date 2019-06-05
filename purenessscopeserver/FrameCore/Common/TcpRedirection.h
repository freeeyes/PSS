#ifndef _TCPREDIRECTION_H
#define _TCPREDIRECTION_H

//负责数据透传转发的数据接口。
//这里可以做成一个框架功能
//add by freeeyes

#include "define.h"
#include "XmlConfig.h"
#include "HashTable.h"
#include "BuffPacketManager.h"
#include "IMessageBlockManager.h"
#include "ClientCommand.h"
#include "IClientManager.h"
#include "IConnectManager.h"

#define MAX_CONNECT_REDIRECTION_COUNT  5

class CRedirectionData : public IClientMessage
{
public:
    CRedirectionData() : m_u4ServerID(0), m_u1Mode(0), m_u1ConnectState(0), m_pConnectManager(NULL)
    {
    }

    virtual ~CRedirectionData()
    {
    }

    CRedirectionData(const CRedirectionData& ar)
    {
        (*this) = ar;
    };

    CRedirectionData& operator = (const CRedirectionData& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    void SetMode(uint8 u1Mode)
    {
        m_u1Mode = u1Mode;
    }

    uint8 GetMode()
    {
        return m_u1Mode;
    }

    void SetConnectState(uint8 u1ConnectState)
    {
        m_u1ConnectState = u1ConnectState;
    }

    void SetConnectManager(IConnectManager* pConnectManager)
    {
        m_pConnectManager = pConnectManager;
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
        if (NULL == pMessageBlockManager)
        {
            return false;
        }

        //申请发送的内存
        mbSend = pMessageBlockManager->Create(u4Len);
        memcpy_safe(pData, u4Len, mbSend->wr_ptr(), u4Len);
        mbSend->wr_ptr(u4Len);
        return true;
    }

    //这里提供接受数据拼包算法，组成完整数据包后会调用RecvData方法
    virtual bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute)
    {
        ACE_UNUSED_ARG(pMessageBlockManager);
        emPacketRoute = PACKET_ROUTE_SELF;

        if (mbRecv->length() == 0)
        {
            return false;
        }

        u2CommandID = 0x1000;

        mbFinishRecv = pMessageBlockManager->Create((uint32)mbRecv->length());
        memcpy_safe(mbRecv->rd_ptr(), (uint32)mbRecv->length(), mbFinishRecv->wr_ptr(), (uint32)mbRecv->length());
        mbFinishRecv->wr_ptr(mbRecv->length());
        mbRecv->rd_ptr(mbRecv->length());

        return true;
    }

    virtual bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo const& objServerIPInfo)
    {
        //数据包已经收全，在这里处理数据
        ACE_UNUSED_ARG(u2CommandID);
        ACE_UNUSED_ARG(objServerIPInfo);

        if (NULL == m_pConnectManager || 0 == m_u1Mode)
        {
            return true;
        }

        IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        pBuffPacket->WriteStream((char* )mbRecv->rd_ptr(), (uint32)mbRecv->length());

        //将数据转发给指定的ConnectID
        return m_pConnectManager->PostMessage(m_u4ServerID,
                                              pBuffPacket,
                                              SENDMESSAGE_JAMPNOMAL,
                                              0,
                                              PACKET_SEND_IMMEDIATLY,
                                              PACKET_IS_FRAMEWORK_RECYC,
                                              0);
    }

    virtual void ReConnect(int nServerID)
    {
        OUR_DEBUG((LM_INFO, "[CRedirectionData::ReConnect]nServerID=%d.\n", nServerID));
    }

    virtual bool ConnectError(int nError, _ClientIPInfo const& objServerIPInfo)
    {
        ACE_UNUSED_ARG(objServerIPInfo);
        OUR_DEBUG((LM_INFO, "[CRedirectionData::ConnectError]nServerID=%d, nError=%d.\n", m_u4ServerID, nError));

        if (1 == m_u1ConnectState)
        {
            //断开客户端
            m_pConnectManager->CloseConnect(m_u4ServerID);
        }

        return true;
    }

    void SetServerID(uint32 u4ServerID)
    {
        m_u4ServerID = u4ServerID;
    }

private:
    uint32                     m_u4ServerID;
    uint8                      m_u1Mode;            //0 不回应数据, 1 回应数据
    uint8                      m_u1ConnectState;    //0 不和远程连接状态同步 1 和远程连接状态同步
    IConnectManager*           m_pConnectManager;

};

class CTcpRedirection
{
public:
    CTcpRedirection();
    ~CTcpRedirection();

    CTcpRedirection(const CTcpRedirection& ar);

    CTcpRedirection& operator = (const CTcpRedirection& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    void Close();

    void Init(std::vector<xmlTcpRedirection::_RedirectionInfo> vec, uint32 u4MaxHandlerCount, IClientManager* pClientManager, IConnectManager* pConnectManager);

    void ConnectRedirect(uint32 u4SrcPort, uint32 u4ConnectID);

    void DataRedirect(uint32 u4ConnectID, ACE_Message_Block* mb);

    void CloseRedirect(uint32 u4ConnectID);

    bool GetMode(uint32 u4LocalPort);

private:
    CHashTable<xmlTcpRedirection::_RedirectionInfo> m_objRedirectList;          //转发服务器接口列表
    CHashTable<CRedirectionData>                    m_objRedirectConnectList;   //转发服务器对应连接结构
    uint16                                          m_u2Count;
    IClientManager*                                 m_pClientManager;
    IConnectManager*                                m_pConnectManager;
};

typedef ACE_Singleton<CTcpRedirection, ACE_Null_Mutex> App_TcpRedirection;

#endif
