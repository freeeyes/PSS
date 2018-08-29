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

class CRedirectionData : public IClientMessage
{
public:
    CRedirectionData() : m_u4ServerID(0), m_u1Mode(0), m_pConnectManager(NULL)
    {
    }

    virtual ~CRedirectionData()
    {
    }

    void SetMode(uint8 u1Mode)
    {
        m_u1Mode = u1Mode;
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

        if (NULL == mbSend)
        {
            OUR_DEBUG((LM_INFO, "[CRedirectionData::Send_Format_data](%d)Format Create Data NULL(%d) OK.\n", m_u4ServerID, u4Len));
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
        ACE_UNUSED_ARG(pMessageBlockManager);
        emPacketRoute = PACKET_ROUTE_SELF;

        u2CommandID = 0x1000;

        mbFinishRecv = mbRecv;

        return true;
    }

    virtual bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo)
    {
        //数据包已经收全，在这里处理数据
        ACE_UNUSED_ARG(u2CommandID);

        if (NULL == m_pConnectManager || 1 == m_u1Mode)
        {
            return true;
        }

        IBuffPacket* pBuffPacket = App_BuffPacketManager::instance()->Create(__FILE__, __LINE__);

        pBuffPacket->WriteStream((char* )mbRecv->rd_ptr(), (uint32)mbRecv->length());

        //将数据转发给指定的ConnectID
        m_pConnectManager->PostMessage(m_u4ServerID,
                                       pBuffPacket,
                                       SENDMESSAGE_JAMPNOMAL,
                                       0,
                                       PACKET_SEND_IMMEDIATLY,
                                       PACKET_IS_FRAMEWORK_RECYC,
                                       0);
        return true;
    }

    virtual void ReConnect(int nServerID)
    {
        OUR_DEBUG((LM_INFO, "[CRedirectionData::ReConnect]nServerID=%d.\n", nServerID));
    }

    virtual bool ConnectError(int nError, _ClientIPInfo objServerIPInfo)
    {
        ACE_UNUSED_ARG(objServerIPInfo);
        OUR_DEBUG((LM_INFO, "[CRedirectionData::ConnectError]nServerID=%d, nError=%d.\n", m_u4ServerID, nError));
        return true;
    }

    void SetServerID(uint32 u4ServerID)
    {
        m_u4ServerID = u4ServerID;
    }

private:
    uint32                     m_u4ServerID;
    uint8                      m_u1Mode;            //0 不回应数据, 1 回应数据
    IConnectManager*           m_pConnectManager;

};

class CTcpRedirection
{
public:
    CTcpRedirection();
    ~CTcpRedirection();

    void Close();

    void Init(xmlTcpRedirection& objCXmlTcpRedirection, uint32 u4MaxHandlerCount, IClientManager* pClientManager, IConnectManager* pConnectManager);

    void ConnectRedirect(uint32 u4SrcPort, uint32 u4ConnectID);

    void DataRedirect(uint32 u4ConnectID, ACE_Message_Block* mb);

    void CloseRedirect(uint32 u4ConnectID);

private:
    CHashTable<xmlTcpRedirection::_RedirectionInfo> m_objRedirectList;          //转发服务器接口列表
    CHashTable<CRedirectionData>                    m_objRedirectConnectList;   //转发服务器对应连接结构
    uint16                                          m_u2Count;
    IClientManager*                                 m_pClientManager;
    IConnectManager*                                m_pConnectManager;
};

#endif
