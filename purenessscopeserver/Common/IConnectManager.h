#ifndef _ICONNECTMANAGER_H
#define _ICONNECTMANAGER_H

#include "IBuffPacket.h"
#include <vector>

enum class EM_Client_Connect_status
{
    CLIENT_CONNECT_EXIST = 0,
    CLIENT_CONNECT_NO_EXIST,
};

enum class EM_Client_Send_Status
{
    CLIENT_SEND_IMMEDIATLY = 0,   //���̷���
    CLIENT_SEND_CACHE,            //���͵�������
    CLIENT_CLOSE_SENDOK,          //������ɹر�����
};

//���Ͳ���������
class CSend_Param
{
public:
    EM_SEND_PACKET_PARSE   m_emSendType  = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    EM_Client_Send_Status m_emSendState = EM_Client_Send_Status::CLIENT_SEND_IMMEDIATLY;
    int                    m_nMessageID  = 0;
    bool                   m_blDlete     = true;
};

class IConnectManager
{
public:
    virtual ~IConnectManager() {}
    /*
    ����˵��
    u4ConnectID ��Ҫ�·���Ӧ��ConnectID
    pBuffPacket һ�����������ݣ�pDataͬ��
    u2CommandID ��Ҫ���Ͱ���Ӧ������ID�����ڿ�ܶ����ݰ���ͳ�ơ�
    CSend_Param ���Ͳ����б�
    */
    //�첽����ָ�������ݰ�(����PacketParse�����Ͱ�) IBuffPacket �����ڲ�������������
    virtual bool PostMessage(uint32 u4ConnectID, uint16 u2CommandID, IBuffPacket*& pBuffPacket, CSend_Param objSendParam) = 0;

    //�������ر�����
    virtual bool CloseConnect(uint32 u4ConnectID)                                                                = 0;
    //�õ���ǰ������ϢIP�Ͷ˿�
    virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)                                                    = 0;
    //�õ���ǰ���Ӽ���IP�Ͷ˿�
    virtual _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID)                                                     = 0;
    //�õ���ǰ��������
    virtual uint32 GetCount()                                                                                    = 0;
    //�������ConnectID�Ƿ����־
    virtual bool SetIsLog(uint32 u4ConnectID, bool blIsLog)                                                      = 0;
    //�õ�ָ���Ŀͻ�������״̬,falseΪ���Ӳ����ڣ�trueΪ���Ӵ���
    virtual EM_Client_Connect_status GetConnectState(uint32 u4ConnectID)                                         = 0;
};

#endif
